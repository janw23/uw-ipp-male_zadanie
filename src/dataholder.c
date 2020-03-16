#include "dataholder.h"
#include <stdlib.h>
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
    assert(*dataHolderPtr == NULL);

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

void dataHolderAddEntry(DataHolder dataHolder, char *entryName)
{
    int subArrayLength = dataHolder->subHoldersArrayLength;

    assert(subArrayLength < dataHolder->subHoldersArrayLengthMax);

    dataHolderCreate(&(dataHolder->subHoldersArray[subArrayLength]));

    dataHolder->subHoldersArray[subArrayLength]->name = entryName;
    dataHolder->subHoldersArrayLength++;
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
        if(dataHolder->subHoldersArray[i]->name == entryName)
        {
            swapDataHolders(&(dataHolder->subHoldersArray[i]),
                    &(dataHolder->subHoldersArray[subArrayLength - 1]));

            dataHolderDestroy(dataHolder->subHoldersArray[subArrayLength - 1]);
            dataHolder->subHoldersArrayLength--;

            return;
        }
    }
}

DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName)
{
    assert(dataHolder != NULL);

    int subArrayLength = dataHolder->subHoldersArrayLength;

    for(int i = 0; i < subArrayLength; i++)
    {
        if(dataHolder->subHoldersArray[i]->name == entryName)
            return dataHolder->subHoldersArray[i];
    }

    return NULL;
}

int getLength(DataHolder dataHolder)
{
    return dataHolder->subHoldersArrayLength;
}

char *getName(DataHolder dataHolder)
{
    return dataHolder->name;
}