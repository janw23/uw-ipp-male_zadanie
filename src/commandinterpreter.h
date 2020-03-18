#include "commandhandler.h"

void callocStringArray(char ***arrayPtr, int elemMaxCount, int elemMaxLenght);

void freeStringArray(char **array, int elemMaxCount);

//zmienia tekst komendy na typ Command zdefiniowany w commandhandler.h
CommandError interpretCommand(const char *cmdText, Command *cmdOut);