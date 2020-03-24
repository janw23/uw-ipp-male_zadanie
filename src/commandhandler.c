#include "commandhandler.h"
#include "commandinterpreter.h"

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

    int argsCount = 0;

    while (argsCount < COMMAND_ARGS_COUNT_MAX &&
           strcmp(args[argsCount], "") != 0) {
        argsCount++;
    }

    if (argsCount > COMMAND_ARGS_COUNT_MAX)
        return COMMAND_ERR_CMD_TOO_MANY_ARGS;

    if (argsCount == 0) {
        dataHolderRemoveAllEntries(dataHolder);
        return COMMAND_ERR_SUCCESS;
    }

    DataHolder currentHolder = dataHolder;

    for (int i = 0; i < argsCount - 1; i++) {
        currentHolder = dataHolderFindEntry(currentHolder, args[i]);

        if (currentHolder == NULL)
            return COMMAND_ERR_SUCCESS;
    }

    dataHolderRemoveEntry(currentHolder, args[argsCount - 1]);

    return COMMAND_ERR_SUCCESS;
}

CommandError handleCmdPrint
        (char **args, DataHolder dataHolder, char **resultMsg) {
    *resultMsg = "";

    int argsCount = 0;

    while (argsCount < COMMAND_ARGS_COUNT_MAX &&
           strcmp(args[argsCount], "") != 0) {
        argsCount++;
    }

    if (argsCount >= COMMAND_ARGS_COUNT_MAX)
        return COMMAND_ERR_CMD_TOO_MANY_ARGS;

    DataHolder currentHolder = dataHolder;

    for (int i = 0; i < argsCount; i++) {
        currentHolder = dataHolderFindEntry(currentHolder, args[i]);

        if (currentHolder == NULL)
            return COMMAND_ERR_SUCCESS;
    }

    dataHolderPrintAllEntriesOrdered(currentHolder);

    return COMMAND_ERR_SUCCESS;
}

int cmdCheckCaseMatchAnyForBothArgs
        (char **args, int argsCount, DataHolder dataHolder) {

    //tablica z wpisami pierwszego poziomu
    DataHolder *holdersArray = NULL;
    unsigned int holdersArraySize = 0;

    holdersArraySize = dataHolderGetAllEntries(dataHolder, &holdersArray);

    for (unsigned int i = 0; i < holdersArraySize; i++) {

        //tablica z holderami drugiego poziomu
        DataHolder *deepHoldersArray = NULL;
        unsigned int deepHoldersArraySize = 0;

        deepHoldersArraySize = dataHolderGetAllEntries
                (holdersArray[i], &deepHoldersArray);

        for (unsigned int k = 0; k < deepHoldersArraySize; k++) {
            DataHolder found =
                    dataHolderFindEntry(deepHoldersArray[k], args[2]);

            if (found != NULL || argsCount == 2) {
                free(deepHoldersArray);
                free(holdersArray);
                return 1;
            }
        }

        free(deepHoldersArray);
    }

    free(holdersArray);
    return 0;
}

int cmdCheckCaseMatchAnyForFirstArg
        (char **args, int argsCount, DataHolder dataHolder) {

    DataHolder *holdersArray = NULL;
    unsigned int holdersArraySize = 0;

    holdersArraySize = dataHolderGetAllEntries(dataHolder, &holdersArray);

    if (argsCount == 1) {
        free(holdersArray);
        return holdersArraySize > 0;
    }

    for (unsigned int i = 0; i < holdersArraySize; i++) {
        DataHolder found = dataHolderFindEntry(holdersArray[i], args[1]);

        if (found == NULL)
            continue;

        if (argsCount == 2) {
            free(holdersArray);
            return 1;

        } else { //argsCount == 3
            found = dataHolderFindEntry(found, args[2]);

            if (found != NULL) {
                free(holdersArray);
                return 1;
            }
        }
    }

    free(holdersArray);
    return 0;
}

int cmdCheckCaseMatchAnyForSecondArg
        (char **args, int argsCount, DataHolder dataHolder) {

    DataHolder found = dataHolderFindEntry(dataHolder, args[0]);

    if (found == NULL)
        return 0;

    DataHolder *deepHoldersArray = NULL;
    unsigned int deepHoldersArraySize = 0;

    deepHoldersArraySize =
            dataHolderGetAllEntries(found, &deepHoldersArray);

    if (argsCount == 2) {
        free(deepHoldersArray);
        return deepHoldersArraySize > 0;
    }

    for (unsigned int i = 0; i < deepHoldersArraySize; i++) {
        found = dataHolderFindEntry(deepHoldersArray[i], args[2]);

        if (found != NULL) {
            free(deepHoldersArray);
            return 1;
        }
    }

    free(deepHoldersArray);
    return 0;
}

//obsługuje przypadki, gdy w argumencie komendy występuje znak
//dopasowania dowolnej nazwy
//zwraca -1 jeśli taki znak nie wystąpił, 0 jeśli wystąpił
//ale nie znaleziono dopasowania, 1 jeśli wystąpił i znaleziono dopasowanie

int handleCmdCheckCaseMatchAny
        (char **args, int argsCount, DataHolder dataHolder) {

    assert(argsCount <= 3);

    int firstMatchAny = strcmp(args[0], COMMAND_CHAR_MATCH_ANY) == 0;
    int secondMatchAny = strcmp(args[1], COMMAND_CHAR_MATCH_ANY) == 0;

    if (firstMatchAny && secondMatchAny) {
        return cmdCheckCaseMatchAnyForBothArgs(args, argsCount, dataHolder);

    } else if (secondMatchAny) {
        return cmdCheckCaseMatchAnyForSecondArg(args, argsCount, dataHolder);

    } else if (firstMatchAny) {
        return cmdCheckCaseMatchAnyForFirstArg(args, argsCount, dataHolder);
    }

    return -1;
}

CommandError handleCmdCheck
        (char **args, DataHolder dataHolder, char **resultMsg) {

    int argsCount = 0;

    while (argsCount < COMMAND_ARGS_COUNT_MAX &&
           strcmp(args[argsCount], "") != 0) {
        argsCount++;
    }

    if (argsCount == 0)
        return COMMAND_ERR_CMD_TOO_FEW_ARGS;

    if (argsCount > COMMAND_ARGS_COUNT_MAX)
        return COMMAND_ERR_CMD_TOO_MANY_ARGS;

    if (strcmp(args[argsCount - 1], COMMAND_CHAR_MATCH_ANY) == 0)
        return COMMAND_ERR_CMD_MATCH_ANY_AS_LAST_ARG;

    int caseMatchAnyResult =
            handleCmdCheckCaseMatchAny(args, argsCount, dataHolder);

    if (caseMatchAnyResult >= 0) {
        *resultMsg = caseMatchAnyResult ? "YES" : "NO";
        return COMMAND_ERR_SUCCESS;
    }

    DataHolder currentHolder = dataHolder;

    for (int i = 0; i < argsCount; i++) {
        currentHolder = dataHolderFindEntry(currentHolder, args[i]);

        if (currentHolder == NULL) {
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

    return "ERROR";
}