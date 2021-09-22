/* Copyright (C) 2020-2021 Open Information Security Foundation
 *
 * You can copy, redistribute or modify this Program under the terms of
 * the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>

#include "suricata-plugin.h"
#include "util-mem.h"
#include "util-debug.h"

#define OUTPUT_NAME "eve-filetype"

static int ThreadInit(void *ctx, int thread_id, void **thread_data);
static int ThreadDeinit(void *ctx, void *thread_data);

/**
 * Per thread context data for each logging thread.
 */
typedef struct ThreadData_ {
    /** The thread ID, for demonstration purposes only. */
    int thread_id;

    /** The number of records logged on this thread. */
    uint64_t count;
} ThreadData;

/**
 * A context object for each eve logger using this output.
 */
typedef struct Context_ {
    /** Verbose, or print to stdout. */
    int verbose;

    /** A thread context to use when not running in threaded mode. */
    ThreadData *thread;
} Context;

/**
 * This function is called when the output is closed.
 *
 * This will be called after ThreadDeinit is called for each thread.
 *
 * \param data The data allocated in TemplateOpen. It should be cleaned up and
 *      deallocated here.
 */
static void TemplateClose(void *data)
{
    printf("TemplateClose\n");
    Context *ctx = data;
    if (ctx != NULL) {
        if (ctx->thread) {
            ThreadDeinit(ctx, (void *)ctx->thread);
        }
        SCFree(ctx);
    }
}

/**
 * This function is called to initialize the output, it can be somewhat thought
 * of like opening a file.
 *
 * \param conf The EVE configuration node using this output.
 *
 * \param threaded If true the EVE subsystem is running in threaded mode.
 *
 * \param data A pointer where context data can be stored relevant to this
 *      output.
 *
 * Eve output plugins need to be thread aware as the threading happens at lower
 * level than the EVE output, so a flag is provided here to notify the plugin if
 * threading is enabled or not.
 *
 * If the plugin does not work with threads disabled, or enabled, this function
 * should return -1.
 *
 * Note for upgrading a plugin from 6.0 to 7.0: The ConfNode in 7.0 is the
 * configuration for the eve instance, not just a node named after the plugin.
 * This allows the plugin to get more context about what it is logging.
 */
static int TemplateInitOutput(ConfNode *conf, bool threaded, void **data)
{
    SCLogNotice("Initializing template eve output plugin: threaded=%d", threaded);
    Context *context = SCCalloc(1, sizeof(Context));
    if (context == NULL) {
        return -1;
    }

    /* Verbose by default. */
    int verbose = 1;

    if (conf && (conf = ConfNodeLookupChild(conf, "eve-template")) != NULL) {
        if (!ConfGetChildValueBool(conf, "verbose", &verbose)) {
            verbose = 1;
        } else {
            SCLogNotice("Read verbose configuration value of %d", verbose);
        }
    }
    context->verbose = verbose;

    if (!threaded) {
        /* We're not running in threaded mode so allocate a thread context here
         * to avoid duplication of context data such as file pointers, database
         * connections, etc. */
        if (ThreadInit(context, 0, (void **)&context->thread) != 0) {
            SCFree(context);
            return -1;
        }
    }
    *data = context;
    return 0;
}

/**
 * Initialize per thread context.
 *
 * \param ctx The context created in TemplateInitOutput.
 *
 * \param thread_id An identifier for this thread.
 *
 * \param thread_data Pointer where thread specific context can be stored.
 *
 * When the EVE output is running in threaded mode this will be called once for
 * each output thread with a unique thread_id. For regular file logging in
 * threaded mode Suricata uses the thread_id to construct the files in the form
 * of "eve.<thread_id>.json". This plugin may want to do similar, or open
 * multiple connections to whatever the final logging location might be.
 *
 * In the case of non-threaded EVE logging this function is NOT called by
 * Suricata, but instead this plugin chooses to use this method to create a
 * default (single) thread context.
 */
static int ThreadInit(void *ctx, int thread_id, void **thread_data)
{
    ThreadData *tdata = SCCalloc(1, sizeof(ThreadData));
    if (tdata == NULL) {
        SCLogError(SC_ERR_MEM_ALLOC, "Failed to allocate thread data");
        return -1;
    }
    tdata->thread_id = thread_id;
    *thread_data = tdata;
    SCLogNotice("Initialized thread %d", tdata->thread_id);
    return 0;
}

/**
 * Deinitialize a thread.
 *
 * This is where any cleanup per thread should be done including free'ing of the
 * thread_data if needed.
 */
static int ThreadDeinit(void *ctx, void *thread_data)
{
    if (thread_data == NULL) {
        // Nothing to do.
        return 0;
    }

    ThreadData *tdata = thread_data;
    SCLogNotice(
            "Deinitializing thread %d: records written: %" PRIu64, tdata->thread_id, tdata->count);
    SCFree(tdata);
    return 0;
}

/**
 * This method is called with formatted Eve JSON data.
 *
 * \param buffer Formatted JSON buffer \param buffer_len Length of formatted
 * JSON buffer \param data Data set in Init callback \param thread_data Data set
 * in ThreadInit callbacl
 *
 * Do not block in this thread, it will cause packet loss. Instead of outputting
 * to any resource that may block it might be best to enqueue the buffers for
 * further processing which will require copying of the provided buffer.
 */
static int TemplateWrite(const char *buffer, int buffer_len, void *data, void *thread_data)
{
    Context *ctx = data;
    ThreadData *thread = thread_data;

    /* The thread_data could be null which is valid, or it could be that we are
     * in single threaded mode. */
    if (thread == NULL) {
        thread = ctx->thread;
    }

    thread->count++;

    if (ctx->verbose) {
        SCLogNotice("Received write with thread_data %p: %s", thread_data, buffer);
    }
    return 0;
}

/**
 * Called by Suricata to initialize the module. This module registers
 * new file type to the JSON logger.
 */
void TemplateInit(void)
{
    SCEveFileType *my_output = SCCalloc(1, sizeof(SCEveFileType));
    my_output->name = OUTPUT_NAME;
    my_output->Init = TemplateInitOutput;
    my_output->Deinit = TemplateClose;
    my_output->ThreadInit = ThreadInit;
    my_output->ThreadDeinit = ThreadDeinit;
    my_output->Write = TemplateWrite;
    if (!SCRegisterEveFileType(my_output)) {
        FatalError(SC_ERR_PLUGIN, "Failed to register filetype plugin: %s", OUTPUT_NAME);
    }
}

const SCPlugin PluginRegistration = {
    .name = OUTPUT_NAME,
    .author = "Jason Ish",
    .license = "GPLv2",
    .Init = TemplateInit,
};

const SCPlugin *SCPluginRegister()
{
    return &PluginRegistration;
}
