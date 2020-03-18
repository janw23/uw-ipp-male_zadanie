#include "commandinterpreter.h" //zawiera "commandhandler.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

const Command knownCommands[KNOWN_COMMANDS_COUNT] =
        {{"ADD",   COMMAND_ADD,   NULL},
         {"DEL",   COMMAND_DEL,   NULL},
         {"PRINT", COMMAND_PRINT, NULL},
         {"CHECK", COMMAND_CHECK, NULL}};

const char commandSeparators[KNOWN_COMMAND_SEPARATORS_COUNT] =
        {' ', '\t', '\v', '\f', '\r'};

void mallocCommand(Command **cmd)
{
    *cmd = malloc(sizeof(Command));

    (*cmd)->name = NULL;
    (*cmd)->args = NULL;
}

void freeCommand(Command *cmd)
{
    if(cmd->name != NULL)
        free(cmd->name);

    if(cmd->args != NULL)
        freeStringArray(cmd->args, COMMAND_ARGS_COUNT_MAX);

    free(cmd);
}

CommandError printInCaseOfError(CommandError err)
{
    if(err != COMMAND_ERR_SUCCESS)
        fprintf(stderr, "CommandHandler error occurred: code-%d\n", err);

    return err;
}

char *handleCommand(char *commandText, DataHolder dataHolder)
{
    CommandError err = COMMAND_ERR_SUCCESS;

    Command *cmd = NULL;
    mallocCommand(&cmd);

    err = interpretCommand(commandText, cmd);

    if(err != COMMAND_ERR_SUCCESS)
        goto handleError;

    fprintf(stderr, "cmd = [name: %s, code: %d, args: %s, %s, %s]\n",
            cmd->name, cmd->code, cmd->args[0], cmd->args[1], cmd->args[2]);

    freeCommand(cmd);
    return "OK";

    handleError:
        freeCommand(cmd);

        if(err == COMMAND_ERR_IGNORE_LINE)
            return "";

        printInCaseOfError(err);
        return "ERROR";
}