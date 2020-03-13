#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dataholder.h"

static inline DataHolderError check(DataHolderError err)
{
    if(err != DATAHOLDER_ERR_SUCCESS)
        printf("DataHolder error encourtered with code: %d\n", err);

    assert(err == DATAHOLDER_ERR_SUCCESS);
    return err;
}

int main()
{
    DataHolder dataHolder = NULL;
    check( dataHolderCreate(&dataHolder) );

    printf("Created a new DataHolder\n");
    printf("Its length: %d\n", getLength(dataHolder));

    check( dataHolderAddEntry(dataHolder, "Entry1") );

    check( dataHolderDestroy(dataHolder) );
}