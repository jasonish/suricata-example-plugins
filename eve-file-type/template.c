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

static int TemplateWrite(const char *buffer, int buffer_len, void *data, void *thread_data)
{
    SCLogNotice("Received write with thread_data %p: %s", thread_data, buffer);
    return 0;
}

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
    SCLogNotice("threaded=%d", threaded);
    Context *context = SCCalloc(1, sizeof(Context));
    if (context == NULL) {
        return -1;
    }
    *data = context;
    return 0;
}

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
