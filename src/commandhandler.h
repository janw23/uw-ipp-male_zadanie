#include "dataholder.h"

#define COMMAND_ERR_SUCCESS 0
#define COMMAND_ERR_FAILURE 1
#define COMMAND_ERR_CMD_NAME_NOT_RECOGNIZED 2
#define COMMAND_ERR_IGNORE_LINE 3
#define COMMAND_ERR_CMD_TOO_MANY_ARGUMENTS 4

typedef int CommandError;

char *handleCommand(char *commandText, DataHolder dataHolder);