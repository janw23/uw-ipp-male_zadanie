#include "commandhandler.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define KNOWN_COMMANDS_COUNT 4

#define COMMANDHANDLER_CMD_ADD 0
#define COMMANDHANDLER_CMD_DEL 1
#define COMMANDHANDLER_CMD_PRINT 2
#define COMMANDHANDLER_CMD_CHECK 3

typedef struct
{
    const char *name;
    int code;
    char **args;
} Command;

const Command knownCommands[KNOWN_COMMANDS_COUNT] =
        {{"ADD",   COMMANDHANDLER_CMD_ADD,   NULL},
         {"DEL",   COMMANDHANDLER_CMD_DEL,   NULL},
         {"PRINT", COMMANDHANDLER_CMD_PRINT, NULL},
         {"CHECK", COMMANDHANDLER_CMD_CHECK, NULL}};

CommandHandlerError printInCaseOfError(CommandHandlerError err)
{
    if(err != COMMANDHANDLER_ERR_SUCCESS)
        fprintf(stderr, "CommandHandler error occurred: code-%d\n", err);

    return err;
}

CommandHandlerError getCommandCode(char **commandTextPtr, int *result)
{
    char *cmdName = strsep(commandTextPtr, " ");

    fprintf(stderr, "Here\n");

    //BŁĄÐ JEST TUTAJ!!!!
    fprintf(stderr,"Detected name: <%s>\n", cmdName);



    for(int i = 0; i < KNOWN_COMMANDS_COUNT; i++)
    {

        if(strcmp(cmdName, knownCommands[i].name) == 0)
        {
            *result = knownCommands[i].code;
            return COMMANDHANDLER_ERR_SUCCESS;
        }
    }

    return COMMANDHANDLER_ERR_CMD_NAME_NOT_RECOGNIZED;
}

CommandHandlerError interpretCommand(char *commandText, Command *result)
{
    CommandHandlerError err;

    int cmdCode;

    err = getCommandCode(&commandText, &cmdCode);
    if(err != COMMANDHANDLER_ERR_SUCCESS)
        return err;

    printf("Recognized command code: %d\n", cmdCode);

    //Command *cmd = malloc(sizeof(Command));
    return COMMANDHANDLER_ERR_SUCCESS;
}

char *handleCommand(char *commandText, DataHolder dataHolder)
{
    CommandHandlerError err;

    Command cmd;

    printInCaseOfError(interpretCommand(commandText, &cmd));

    return "OK";
}