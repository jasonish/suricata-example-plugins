#include <stdio.h>
#include <stdlib.h>

#include "suricata-plugin.h"
#include "util-mem.h"
#include "util-debug.h"

#define OUTPUT_NAME "eve-template"

typedef struct Context_ {
    /* A count of the number of records written. */
    uint64_t count;
    /* Verbose, or print to stdout. */
    int verbose;
} Context;

/**
 * This function is called when the output is to be "opened".
 *
 * \param conf The EVE configuration node using this output type.
 * \param data A pointer where context data specific to this instance of the
 *      plugin can be stored.
 *
 * \retval 0 on success, -1 on failure.
 */
static int TemplateOpen(ConfNode *conf, void **data) {
    printf("TemplateOpen\n");

    Context *context = SCCalloc(1, sizeof(Context));
    if (context == NULL) {
        return -1;
    }

    /* An example of getting at plugin configuration data. */
    int verbose = 1;
    if (conf && !ConfGetChildValueBool(conf, "verbose", &verbose)) {
        verbose = 1;
    }
    context->verbose = verbose;

    *data = context;
    return 0;
}

/**
 * This function is call for each EVE record that needs to be logged.
 *
 * \param buffer Pointer to the formatted EVE (JSON) record.
 * \param buffer_len The length of the buffer.
 * \param data The optional context data provided in the Open callback.
 *
 * \retval Not used.
 */
static int TemplateWrite(const char *buffer, int buffer_len, void *data) {
    Context *ctx = data;
    if (ctx->verbose) {
        printf("TemplateWrite: %s\n", buffer);
    }
    ctx->count++;
    return 0;
}

/**
 * This function is called when the output is closed.
 *
 * \param data The data allocated in Open. It should be cleaned up here.
 */
static void TemplateClose(void *data) {
    Context *context = data;
    printf("TemplateClose: records written=%"PRIu64"\n", context->count);
    Context *ctx = data;
    if (ctx != NULL) {
        SCFree(ctx);
    }
}

/**
 * Called by Suricata to initialize the module. This module registers
 * new file type to the JSON logger.
 */
void TemplateInit(void)
{
    SCPluginFileType *my_output = SCCalloc(1, sizeof(SCPluginFileType));
    my_output->name = OUTPUT_NAME;
    my_output->Open = TemplateOpen;
    my_output->Write = TemplateWrite;
    my_output->Close = TemplateClose;
    if (!SCPluginRegisterFileType(my_output)) {
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