#include "dataholder.h"

#define COMMANDHANDLER_ERR_SUCCESS 0
#define COMMANDHANDLER_ERR_FAILURE 1

#define COMMANDHANDLER_CMD_ADD 0
#define COMMANDHANDLER_CMD_DEL 1
#define COMMANDHANDLER_CMD_PRINT 2
#define COMMANDHANDLER_CMD_CHECK 3

typedef int CommandHandlerError;

//Tablica stałych pointerów na stałe znakowe
static const char *const knownCommands[] = {"ADD", "DEL", "PRINT", "CHECK"};

void handleCommand(char *command, DataHolder dataHolder);