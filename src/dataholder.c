#include "dataholder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DATAHOLDER_INITIAL_LENGTH_MAX 100

struct DataEntry
{
    char *name;

    DataHolder *subHoldersArray;
    int subHoldersArrayLength;
    int subHoldersArrayLengthMax;
};

//Tworzy pointer na nowy DataHolder w miejscu
//wskazywanym przez podany pusty pointer
void dataHolderCreate(DataHolder *dataHolderPtr)
{
    //assert(*dataHolderPtr == NULL);

    *dataHolderPtr = malloc(sizeof(struct DataEntry));

    (*dataHolderPtr)->subHoldersArray =
            malloc(DATAHOLDER_INITIAL_LENGTH_MAX * sizeof(struct DataEntry));

    (*dataHolderPtr)->subHoldersArrayLengthMax = DATAHOLDER_INITIAL_LENGTH_MAX;
    (*dataHolderPtr)->subHoldersArrayLength = 0;
}

void dataHolderDestroy(DataHolder dataHolder)
{
    //Nie trzeba nic robić jeśli obiekt już nie istnieje
    if(dataHolder == NULL)
        return;

    //Niszczenie podrzędnych DataHolderów
    for(int i = 0; i < dataHolder->subHoldersArrayLength; i++)
         dataHolderDestroy(dataHolder->subHoldersArray[i]);

    free(dataHolder->subHoldersArray);
    free(dataHolder);
}

DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName)
{
    int subArrayLength = dataHolder->subHoldersArrayLength;

    assert(subArrayLength < dataHolder->subHoldersArrayLengthMax);

    dataHolderCreate(&(dataHolder->subHoldersArray[subArrayLength]));

    dataHolder->subHoldersArray[subArrayLength]->name = entryName;
    dataHolder->subHoldersArrayLength++;

    return dataHolder->subHoldersArray[subArrayLength];
}

void swapDataHolders(DataHolder *holderA, DataHolder *holderB)
{
    DataHolder helpHolder = *holderA;

    *holderA = *holderB;
    *holderB = helpHolder;
}

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName)
{
    assert(dataHolder != NULL);

    int subArrayLength = dataHolder->subHoldersArrayLength;

    for(int i = 0; i < subArrayLength; i++)
    {
        if(strcmp(dataHolder->subHoldersArray[i]->name, entryName) == 0)
        {
            swapDataHolders(&(dataHolder->subHoldersArray[i]),
                    &(dataHolder->subHoldersArray[subArrayLength - 1]));

            dataHolderDestroy(dataHolder->subHoldersArray[subArrayLength - 1]);
            dataHolder->subHoldersArrayLength--;

            return;
        }
    }
}

void dataHolderRemoveAllEntries(DataHolder dataHolder)
{
    assert(dataHolder != NULL);

    for(int i = 0; i < dataHolder->subHoldersArrayLength; i++)
        dataHolderDestroy(dataHolder->subHoldersArray[i]);

    dataHolder->subHoldersArrayLength = 0;
}

DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName)
{
    assert(dataHolder != NULL);

    int subArrayLength = dataHolder->subHoldersArrayLength;

    for(int i = 0; i < subArrayLength; i++)
    {
        if(strcmp(dataHolder->subHoldersArray[i]->name, entryName) == 0)
            return dataHolder->subHoldersArray[i];
    }

    return NULL;
}

DataHolder dataHolderFindOrAddEntry(DataHolder dataHolder, char *entryName)
{
    DataHolder result = dataHolderFindEntry(dataHolder, entryName);

    if(result == NULL)
        result = dataHolderAddEntry(dataHolder, entryName);

    return result;
}

void dataHolderPrintEntryName(DataHolder dataHolder)
{
    printf("%s\n", dataHolder->name);
}

void dataHolderPrintAllEntries(DataHolder dataHolder)
{
    for(int i = 0; i < dataHolder->subHoldersArrayLength; i++)
    {
        fprintf(stderr, "Printing dataHolder: %s\n", dataHolder->subHoldersArray[i]->name);
        dataHolderPrintEntryName(dataHolder->subHoldersArray[i]);
        dataHolderPrintAllEntries(dataHolder->subHoldersArray[i]);
    }
}

int getLength(DataHolder dataHolder)
{
    return dataHolder->subHoldersArrayLength;
}

char *getName(DataHolder dataHolder)
{
    return dataHolder->name;
}