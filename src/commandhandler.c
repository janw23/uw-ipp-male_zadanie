#include "commandhandler.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define KNOWN_COMMANDS_COUNT 4
#define KNOWN_COMMAND_SEPARATORS_COUNT 5

#define COMMAND_ARGS_COUNT_MAX 3
#define COMMAND_IGNORE_LINE_CHARACTER '#'
#define COMMAND_END_LINE_CHARACTER '\n'

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
        {' ', '\t', '\v', '\f', '\r'};

int isCharSeparator(char ch)
{
    for(int i = 0; i < KNOWN_COMMAND_SEPARATORS_COUNT; i++)
    {
        if(ch == commandSeparators[i])
            return 1;
    }

    return 0;
}

//dzięki [validCharEncountered] znak oznaczający ignorowanie linii jest
//uznawany za poprawny znak jeśli wcześniej wystąpiły inne poprawne znaki
        int isCharValid(char ch, int *validCharEncountered)
{
    return ch >= 33 &&
        (*validCharEncountered || ch != COMMAND_IGNORE_LINE_CHARACTER);
}

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
        (*arrayPtr)[i] = calloc(elemMaxLenght, sizeof(char));
}

void freeStringArray(char **array, int elemMaxCount)
{
    for(int i = 0; i < elemMaxCount; i++)
        free(array[i]);

    free(array);
}

//zwraca prawdę jeśli podany znak [ch] oznacza ignorowanie całej komendy
//dzięki [validCharEncountered] ma efekt tylko jeśli nie wystąpił wcześniej
// żaden inny niebiały znak
int checkForIgnoreLineCharacter(char ch, int *validCharEncountered)
{
    if(isCharValid(ch, validCharEncountered))
        *validCharEncountered = 1;

    return *validCharEncountered == 0 && ch == COMMAND_IGNORE_LINE_CHARACTER;
}

//początkowa wartość wskazywana przez [componentIndex] powinna wynosić -1
CommandHandlerError readNextCharOfCommand(char currentChar, char prevChar,
        int *validCharEncountered, char **componentsArray,
        int *componentIndex, int *componentCharIndex)
{
    //Sprawdzanie, czy wystąpił znak sygnalizujący ignorowanie linii
    if(checkForIgnoreLineCharacter(currentChar, validCharEncountered))
        return COMMANDHANDLER_ERR_IGNORE_LINE;

    //rozpoczęcie kolejnego słowa
    if(isCharSeparator(prevChar) &&
        isCharValid(currentChar, validCharEncountered))
    {
        *componentIndex += 1;
        *componentCharIndex = 0;

        if(*componentIndex >= COMMAND_ARGS_COUNT_MAX + 1)
            return COMMANDHANDLER_ERR_CMD_TOO_MANY_ARGUMENTS;

        fprintf(stderr, "char = %c\n", currentChar);
        fprintf(stderr, "componentIndex = %d\n", *componentIndex);
        fprintf(stderr, "componentCharIndex = %d\n\n", *componentCharIndex);

        componentsArray[*componentIndex][*componentCharIndex] = currentChar;
    }
    //kontynuacja dnagego słowa
    else if(isCharValid(prevChar, validCharEncountered) &&
        isCharValid(currentChar, validCharEncountered))
    {
        *componentCharIndex += 1;

        fprintf(stderr, "char = %c\n", currentChar);
        fprintf(stderr, "componentIndex = %d\n", *componentIndex);
        fprintf(stderr, "componentCharIndex = %d\n\n", *componentCharIndex);
        componentsArray[*componentIndex][*componentCharIndex] = currentChar;
    }

    return COMMANDHANDLER_ERR_SUCCESS;
}


CommandHandlerError interpretCommand(char *commandText, Command *result)
{
    size_t commandTextLength = strlen(commandText);

    //tablica składowych stringów komendy
    char **cmdComponentsArray = NULL;

    //tablica na odczytaną nazwę komendy i jej argumenty
    mallocStringArray(&cmdComponentsArray,
            COMMAND_ARGS_COUNT_MAX + 1,
            commandTextLength);

    //pamiętanie, czy napotkany został znak inny
    //niż ten oznaczający ignorowanie linii
    int validCharEncountered = 0;

    //index aktualnie odczytywanej składowej komendy
    int cmdComponentIndex = -1;

    //index aktualnego znaku w aktualnie odczytywanej składowej komendy
    int cmdComponentCharIndex = 0;

    for(int i = 0; i < commandTextLength; i++)
    {
        CommandHandlerError err = readNextCharOfCommand(commandText[i],
                (i > 0 ? commandText[i - 1] : commandSeparators[0]),
                &validCharEncountered, cmdComponentsArray,
                &cmdComponentIndex, &cmdComponentCharIndex);

        if(err != COMMANDHANDLER_ERR_SUCCESS)
        {
            freeStringArray(cmdComponentsArray,
                    COMMAND_ARGS_COUNT_MAX + 1);

            return err;
        }
    }

    printf("CMD = [name: %s, args: %s, %s, %s]\n", cmdComponentsArray[0],
    cmdComponentsArray[1], cmdComponentsArray[2], cmdComponentsArray[3]);

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