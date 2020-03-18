#include "commandinterpreter.h"

#include <string.h>
#include <malloc.h>

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
int isCharValid(char ch, const int *validCharEncountered)
{
    return ch >= 33 &&
           (*validCharEncountered || ch != COMMAND_IGNORE_LINE_CHARACTER);
}

void callocStringArray(char ***arrayPtr, int elemMaxCount, int elemMaxLenght)
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
CommandError readNextCharOfCommand
        (char currentChar, char prevChar,
         int *validCharEncountered, char **componentsArray,
         int *componentIndex, int *componentCharIndex)
{
    //Sprawdzanie, czy wystąpił znak sygnalizujący ignorowanie linii
    if(checkForIgnoreLineCharacter(currentChar, validCharEncountered))
        return COMMAND_ERR_IGNORE_LINE;

    //rozpoczęcie kolejnego słowa
    if(isCharSeparator(prevChar) &&
       isCharValid(currentChar, validCharEncountered))
    {
        *componentIndex += 1;
        *componentCharIndex = 0;

        if(*componentIndex >= COMMAND_ARGS_COUNT_MAX + 1)
            return COMMAND_ERR_CMD_TOO_MANY_ARGS;

        componentsArray[*componentIndex][*componentCharIndex] = currentChar;
    }
        //kontynuacja dnagego słowa
    else if(isCharValid(prevChar, validCharEncountered) &&
            isCharValid(currentChar, validCharEncountered))
    {
        *componentCharIndex += 1;

        componentsArray[*componentIndex][*componentCharIndex] = currentChar;
    }

    return COMMAND_ERR_SUCCESS;
}

CommandError getCommandCode(char *cmdName, int *result)
{
    for(int i = 0; i < KNOWN_COMMANDS_COUNT; i++)
    {
        if(strcmp(cmdName, knownCommands[i].name) == 0)
        {
            *result = knownCommands[i].code;
            return COMMAND_ERR_SUCCESS;
        }
    }

    return COMMAND_ERR_CMD_NAME_NOT_RECOGNIZED;
}

CommandError commandFromStringArray(char **componentsArray, Command *cmdOut)
{
    CommandError err;

    cmdOut->name = malloc(strlen(componentsArray[0]) + 1);
    memcpy(cmdOut->name, componentsArray[0], strlen(componentsArray[0]) + 1);

    cmdOut->args = malloc(COMMAND_ARGS_COUNT_MAX * sizeof(char*));

    for(int i = 0; i < COMMAND_ARGS_COUNT_MAX; i++)
    {
        cmdOut->args[i] = malloc(strlen(componentsArray[i + 1]) + 1);

        memcpy(cmdOut->args[i], componentsArray[i + 1],
               strlen(componentsArray[i + 1]) + 1);
    }

    int cmdCode;

    err = getCommandCode(cmdOut->name, &cmdCode);
    if(err != COMMAND_ERR_SUCCESS)
        return err;

    cmdOut->code = cmdCode;

    return COMMAND_ERR_SUCCESS;
}

CommandError interpretCommand(const char *cmdText, Command *cmdOut)
{
    CommandError err = COMMAND_ERR_SUCCESS;

    size_t commandTextLength = strlen(cmdText);

    //tablica na odczytaną nazwę komendy i jej argumenty
    char **cmdComponentsArray = NULL;
    callocStringArray(&cmdComponentsArray,
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
        err = readNextCharOfCommand(cmdText[i],
                (i > 0 ? cmdText[i - 1] : commandSeparators[0]),
                &validCharEncountered, cmdComponentsArray,
                &cmdComponentIndex, &cmdComponentCharIndex);

        if(err != COMMAND_ERR_SUCCESS)
            goto handleError;
    }

    if(!validCharEncountered)
    {
        err = COMMAND_ERR_IGNORE_LINE;
        goto handleError;
    }

    err = commandFromStringArray(cmdComponentsArray, cmdOut);

    goto handleError;

    handleError:
    freeStringArray(cmdComponentsArray,COMMAND_ARGS_COUNT_MAX + 1);
    return err;
}