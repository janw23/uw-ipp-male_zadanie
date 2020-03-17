#include "commandhandler.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define KNOWN_COMMANDS_COUNT 4
#define KNOWN_COMMAND_SEPARATORS_COUNT 5

#define COMMAND_ARGS_COUNT_MAX 3
#define COMMAND_LINE_IGNORE_CHARACTER '#'
#define COMMAND_LINE_END_CHARACTER '\n'

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

const char commandSeparators[KNOWN_COMMAND_SEPARATORS_COUNT] =
        {',', '\t', '\v', '\f', '\r'};

CommandHandlerError printInCaseOfError(CommandHandlerError err)
{
    if(err != COMMANDHANDLER_ERR_SUCCESS)
        fprintf(stderr, "CommandHandler error occurred: code-%d\n", err);

    return err;
}

void mallocStringArray(char ***arrayPtr, int elemMaxCount, int elemMaxLenght)
{
    *arrayPtr = malloc((elemMaxCount) * sizeof(char*));

    for(int i = 0; i <= COMMAND_ARGS_COUNT_MAX; i++)
        (*arrayPtr)[i] = malloc(elemMaxLenght * sizeof(char));
}

void freeStringArray(char **array, int elemMaxCount)
{
    for(int i = 0; i < elemMaxCount; i++)
        free(array[i]);

    free(array);
}

CommandHandlerError interpretCommand(char *commandText, Command *result)
{
    size_t commandTextLength = strlen(commandText);

    char **cmdComponentsArray = NULL;

    mallocStringArray(&cmdComponentsArray,
            COMMAND_ARGS_COUNT_MAX + 1,
            commandTextLength);

    //pamiętanie, czy napotkany został znak inny
    //niż ten oznaczający ignorowanie linii
    int validCharEncountered = 0;

    for(int i = 0; i < commandTextLength; i++)
    {
        validCharEncountered =  validCharEncountered ||
                (commandText[i] != COMMAND_LINE_IGNORE_CHARACTER &&
                 commandText[i] >= 33);

        //Sprawdzanie, czy wystąpił znak sygnalizujący ignorowanie linii
        if(!validCharEncountered)
        {
            if(commandText[i] == COMMAND_LINE_IGNORE_CHARACTER)
            {
                freeStringArray(cmdComponentsArray,
                        COMMAND_ARGS_COUNT_MAX + 1);

                return COMMANDHANDLER_ERR_IGNORE_LINE;
            }
        }

    }

    freeStringArray(cmdComponentsArray,COMMAND_ARGS_COUNT_MAX + 1);

    if(!validCharEncountered)
        return COMMANDHANDLER_ERR_IGNORE_LINE;

    return COMMANDHANDLER_ERR_SUCCESS;
}

char *handleCommand(char *commandText, DataHolder dataHolder)
{
    CommandHandlerError err;

    Command cmd;

    printInCaseOfError(interpretCommand(commandText, &cmd));

    return "OK";
}