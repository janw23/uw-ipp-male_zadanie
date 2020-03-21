#include "commandinterpreter.h" //zawiera "commandhandler.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#define COMMAND_CODE_ADD 0
#define COMMAND_CODE_DEL 1
#define COMMAND_CODE_PRINT 2
#define COMMAND_CODE_CHECK 3

const Command knownCommands[KNOWN_COMMANDS_COUNT] =
        {{"ADD",   COMMAND_CODE_ADD,   NULL},
         {"DEL",   COMMAND_CODE_DEL,   NULL},
         {"PRINT", COMMAND_CODE_PRINT, NULL},
         {"CHECK", COMMAND_CODE_CHECK, NULL}};

const char commandSeparators[KNOWN_COMMAND_SEPARATORS_COUNT] =
        {' ', '\t', '\v', '\f', '\r'};

void mallocCommand(Command **cmd) {
    *cmd = malloc(sizeof(Command));

    (*cmd)->name = NULL;
    (*cmd)->args = NULL;
}

void freeCommand(Command *cmd) {
    if (cmd->name != NULL)
        free(cmd->name);

    if (cmd->args != NULL)
        freeStringArray(cmd->args, COMMAND_ARGS_COUNT_MAX);

    free(cmd);
}

CommandError printInCaseOfError(CommandError err) {
    if (err != COMMAND_ERR_SUCCESS)
        fprintf(stderr, "CommandHandler error occurred: code-%d\n", err);

    return err;
}

CommandError handleCmdAdd
        (char **args, DataHolder dataHolder, char **resultMsg) {
    if (strcmp(args[0], "") == 0)
        return COMMAND_ERR_CMD_TOO_FEW_ARGS;

    DataHolder currentDepthHolder = dataHolder;

    for (int i = 0; i < COMMAND_ARGS_COUNT_MAX; i++) {
        if (strcmp(args[i], "") == 0)
            break;

        currentDepthHolder =
                dataHolderAddEntry(currentDepthHolder, args[i]);
    }

    *resultMsg = "OK";
    return COMMAND_ERR_SUCCESS;
}

CommandError handleCmdDel
        (char **args, DataHolder dataHolder, char **resultMsg) {
    *resultMsg = "OK";

    if (strcmp(args[0], "") == 0) {
        dataHolderRemoveAllEntries(dataHolder);
        return COMMAND_ERR_SUCCESS;
    }

    DataHolder previousDepthHolder, currentDepthHolder = dataHolder;
    char *lastArg;

    for (int i = 0; i < COMMAND_ARGS_COUNT_MAX; i++) {
        if (strcmp(args[i], "") == 0)
            break;

        lastArg = args[i];

        previousDepthHolder = currentDepthHolder;
        currentDepthHolder = dataHolderFindEntry(previousDepthHolder, args[i]);

        if (currentDepthHolder == NULL)
            return COMMAND_ERR_SUCCESS;
    }

    dataHolderRemoveEntry(previousDepthHolder, lastArg);

    return COMMAND_ERR_SUCCESS;
}

CommandError handleCmdPrint
        (char **args, DataHolder dataHolder, char **resultMsg) {
    *resultMsg = "";

    DataHolder currentDepthHolder = dataHolder;

    for (int i = 0; i < COMMAND_ARGS_COUNT_MAX; i++) {
        if (strcmp(args[i], "") == 0)
            break;

        if (i == COMMAND_ARGS_COUNT_MAX - 1)
            return COMMAND_ERR_CMD_TOO_MANY_ARGS;

        currentDepthHolder = dataHolderFindEntry(currentDepthHolder, args[i]);

        if (currentDepthHolder == NULL)
            return COMMAND_ERR_SUCCESS;
    }

    dataHolderPrintAllEntries(currentDepthHolder);

    return COMMAND_ERR_SUCCESS;
}

CommandError handleCmdCheck
        (char **args, DataHolder dataHolder, char **resultMsg) {
    if (strcmp(args[0], "") == 0)
        return COMMAND_ERR_CMD_TOO_FEW_ARGS;

    DataHolder currentDepthHolder = dataHolder;

    for (int i = 0; i < COMMAND_ARGS_COUNT_MAX; i++) {
        if (strcmp(args[i], "") == 0)
            break;

        currentDepthHolder = dataHolderFindEntry(currentDepthHolder, args[i]);

        if (currentDepthHolder == NULL) {
            *resultMsg = "NO";
            return COMMAND_ERR_SUCCESS;
        }
    }

    *resultMsg = "YES";

    return COMMAND_ERR_SUCCESS;
}

CommandError chooseCorrectHandling
        (Command cmd, DataHolder dataHolder, char **resultMsg) {
    switch (cmd.code) {
        case COMMAND_CODE_ADD:
            return handleCmdAdd(cmd.args, dataHolder, resultMsg);

        case COMMAND_CODE_DEL:
            return handleCmdDel(cmd.args, dataHolder, resultMsg);

        case COMMAND_CODE_PRINT:
            return handleCmdPrint(cmd.args, dataHolder, resultMsg);

        case COMMAND_CODE_CHECK:
            return handleCmdCheck(cmd.args, dataHolder, resultMsg);
    }

    return COMMAND_ERR_CMD_CODE_NOT_RECOGNIZED;
}

char *handleCommand(char *cmdText, DataHolder dataHolder) {
    CommandError err = COMMAND_ERR_SUCCESS;

    Command *cmdPtr = NULL;
    mallocCommand(&cmdPtr);

    err = interpretCommand(cmdText, cmdPtr);

    if (err != COMMAND_ERR_SUCCESS)
        goto handleError;

    char *resultMsg = NULL;

    err = chooseCorrectHandling(*cmdPtr, dataHolder, &resultMsg);
    if (err != COMMAND_ERR_SUCCESS)
        goto handleError;

    freeCommand(cmdPtr);

    assert(resultMsg != NULL);
    return resultMsg;

    handleError:
    freeCommand(cmdPtr);

    if (err == COMMAND_ERR_IGNORE_LINE)
        return "";

    //printInCaseOfError(err);
    return "ERROR";
}