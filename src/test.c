#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dataholder.h"
#include "commandhandler.h"

void printFound(char *name, int found)
{
    printf("Entry of name %s%sfound\n", name, found ? " " : " not ");
}

int main()
{
    DataHolder dataHolder = NULL;
    dataHolderCreate(&dataHolder);

    printf("Created a new DataHolder\n");

    handleCommand("Hello there", dataHolder);

    dataHolderDestroy(dataHolder);
}