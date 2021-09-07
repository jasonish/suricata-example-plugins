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

#define OUTPUT_NAME "template-filetype-plugin"

typedef struct Context_ {
} Context;

typedef struct ThreadData_ {
    int thread_id;
} ThreadData;

static void TemplateClose(void *data)
{
    printf("TemplateClose\n");
    Context *ctx = data;
    if (ctx != NULL) {
        SCFree(ctx);
    }
}

static int TemplateInitOutput(ConfNode *conf, bool threaded, void **data)
{
    SCLogNotice("Initializing template eve output plugin: threaded=%d", threaded);
    Context *context = SCCalloc(1, sizeof(Context));
    if (context == NULL) {
        return -1;
    }
    *data = context;
    return 0;
}

/**
 * If in threaded mode this method will be called once for each logging thread
 * for thread specific initialization. In regular file output this is where a
 * file would be opened that is prefixed with the thread id. In a plugin this
 * could be a database connection or something (but note that we don't think
 * logging directly to a database is a good idea).
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

static int ThreadDeinit(void *ctx, void *thread_data)
{
    if (thread_data == NULL) {
        // Nothing to do.
        return 0;
    }

    ThreadData *tdata = thread_data;
    SCLogNotice("Deinitializing thread %d", tdata->thread_id);
    SCFree(tdata);
    return 0;
}

/**
 * This method is called with formatted Eve JSON data.
 * 
 * \param buffer Formatted JSON buffer
 * \param buffer_len Length of formatted JSON buffer
 * \param data Data set in Init callback
 * \param thread_data Data set in ThreadInit callbacl
 */
static int TemplateWrite(const char *buffer, int buffer_len, void *data, void *thread_data)
{
    SCLogNotice("Received write with thread_data %p: %s", thread_data, buffer);
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
