#include "dataholder.h"

#define COMMANDHANDLER_ERR_SUCCESS 0
#define COMMANDHANDLER_ERR_FAILURE 1
#define COMMANDHANDLER_ERR_CMD_NAME_NOT_RECOGNIZED 2

typedef int CommandHandlerError;

char *handleCommand(char *commandText, DataHolder dataHolder);