#include "dataholder.h"
#include <stdlib.h>

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
DataHolderError dataHolderCreate(DataHolder *dataHolderPtr)
{
    if(*dataHolderPtr != NULL)
        return DATAHOLDER_ERR_PTR_NOT_NULL;

    *dataHolderPtr = malloc(sizeof(struct DataEntry));

    (*dataHolderPtr)->subHoldersArray =
            malloc(DATAHOLDER_INITIAL_LENGTH_MAX * sizeof(struct DataEntry));

    (*dataHolderPtr)->subHoldersArrayLengthMax = DATAHOLDER_INITIAL_LENGTH_MAX;
    (*dataHolderPtr)->subHoldersArrayLength = 0;

    return *dataHolderPtr!= NULL ?
            DATAHOLDER_ERR_SUCCESS : DATAHOLDER_ERR_PTR_IS_NULL;
}

DataHolderError dataHolderDestroy(DataHolder dataHolder)
{
    //Nie trzeba nic robić jeśli obiekt już nie istnieje
    if(dataHolder == NULL)
        return DATAHOLDER_ERR_SUCCESS;

    //Niszczenie podrzędnych DataHolderów
    for(int i = 0; i < dataHolder->subHoldersArrayLength; i++)
    {
        DataHolderError err =
                dataHolderDestroy(dataHolder->subHoldersArray[i]);

        if(err != DATAHOLDER_ERR_SUCCESS)
            return err;
    }

    free(dataHolder->subHoldersArray);
    free(dataHolder);

    return DATAHOLDER_ERR_SUCCESS;
}

DataHolderError dataHolderAddEntry(DataHolder dataHolder, char *entryName)
{
    int subArrayLength = dataHolder->subHoldersArrayLength;

    if(subArrayLength < dataHolder->subHoldersArrayLengthMax)
    {
        dataHolderCreate(&(dataHolder->subHoldersArray[subArrayLength]));

        dataHolder->subHoldersArray[subArrayLength]->name = entryName;
        dataHolder->subHoldersArrayLength++;
    }
    else {
        return DATAHOLDER_ERR_MEMORY_FULL;
    }

    return DATAHOLDER_ERR_SUCCESS;
}

DataHolderError dataHolderFindEntry(DataHolder dataHolder,
                                    char* entryName,
                                    DataHolder out)
{

}

int getLength(DataHolder dataHolder)
{
    return dataHolder->subHoldersArrayLength;
}