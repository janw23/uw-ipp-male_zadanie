#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dataholder.h"

void printFound(char *name, int found)
{
    printf("Entry of name %s%sfound\n", name, found ? " " : " not ");
}

int main()
{
    DataHolder dataHolder = NULL;
    dataHolderCreate(&dataHolder);

    printf("Created a new DataHolder\n");
    printf("Its length: %d\n", getLength(dataHolder));

    dataHolderAddEntry(dataHolder, "Entry1");
    dataHolderAddEntry(dataHolder, "Entry2");
    dataHolderAddEntry(dataHolder, "Entry3");

    DataHolder found;

    found = dataHolderFindEntry(dataHolder, "Entry1");
    printFound("Entry1", found != NULL);

    found = dataHolderFindEntry(dataHolder, "Entry3");
    printFound("Entry3", found != NULL);

    dataHolderRemoveEntry(dataHolder, "Entry3");

    found = dataHolderFindEntry(dataHolder, "Entry3");
    printFound("Entry3", found != NULL);

    found = dataHolderFindEntry(dataHolder, "Entry69420");
    printFound("Entry69420", found != NULL);


    found = dataHolderFindEntry(dataHolder, "Entry2");
    printFound("Entry2", found != NULL);

    dataHolderAddEntry(found, "SubEntry1");
    dataHolderAddEntry(found, "SubEntry2");

    found = dataHolderFindEntry(dataHolderFindEntry(dataHolder,
            "Entry2"),
                    "SubEntry2");

    printFound("Entry2/SubEntry2", found != NULL);

    dataHolderDestroy(dataHolder);
}