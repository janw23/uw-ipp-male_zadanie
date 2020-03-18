#include "dataholder.h"

#define COMMAND_ERR_SUCCESS 0
#define COMMAND_ERR_FAILURE 1
#define COMMAND_ERR_CMD_NAME_NOT_RECOGNIZED 2
#define COMMAND_ERR_IGNORE_LINE 3
#define COMMAND_ERR_CMD_TOO_MANY_ARGUMENTS 4

#define KNOWN_COMMANDS_COUNT 4
#define KNOWN_COMMAND_SEPARATORS_COUNT 5

#define COMMAND_ARGS_COUNT_MAX 3
#define COMMAND_IGNORE_LINE_CHARACTER '#'
#define COMMAND_END_LINE_CHARACTER '\n'

#define COMMAND_ADD 0
#define COMMAND_DEL 1
#define COMMAND_PRINT 2
#define COMMAND_CHECK 3

typedef int CommandError;

typedef struct
{
    char *name;
    int code;
    char **args;
} Command;

extern const Command knownCommands[KNOWN_COMMANDS_COUNT];

extern const char commandSeparators[KNOWN_COMMAND_SEPARATORS_COUNT];

char *handleCommand(char *commandText, DataHolder dataHolder);