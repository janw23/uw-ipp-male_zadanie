#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "dataholder.h"

#define COMMAND_ERR_SUCCESS 0
#define COMMAND_ERR_FAILURE 1
#define COMMAND_ERR_CMD_NAME_NOT_RECOGNIZED 2
#define COMMAND_ERR_IGNORE_LINE 3
#define COMMAND_ERR_CMD_TOO_MANY_ARGS 4
#define COMMAND_ERR_CMD_CODE_NOT_RECOGNIZED 5
#define COMMAND_ERR_CMD_TOO_FEW_ARGS 6
#define COMMAND_ERR_CMD_MATCH_ANY_AS_LAST_ARG 7

#define KNOWN_COMMANDS_COUNT 4
#define KNOWN_COMMAND_SEPARATORS_COUNT 5

#define COMMAND_ARGS_COUNT_MAX 3
#define COMMAND_CHAR_IGNORE_LINE '#'
#define COMMAND_CHAR_MATCH_ANY "*" //Life, the Universe and Everything

typedef int CommandError;

typedef struct {
    char *name;
    int code;
    char **args;
} Command;

extern const Command knownCommands[KNOWN_COMMANDS_COUNT];

extern const char commandSeparators[KNOWN_COMMAND_SEPARATORS_COUNT];

char *handleCommand(char *cmdText, DataHolder dataHolder);

#endif /* COMMAND_HANDLER_H */