#include "dataholder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DATAHOLDER_INITIAL_LENGTH_MAX 100

struct DataEntry {
    char *name;

    DataHolder prev, subHolder, next;
};

int dataHolderStringCompare(char *a, char *b) {
    if (a == NULL && b == NULL)
        return 0;

    if (a == NULL)
        return -1;

    if (b == NULL)
        return 1;

    return strcmp(a, b);
}

//Tworzy pointer na nowy DataHolder w miejscu
//wskazywanym przez podany pusty pointer
void dataHolderCreate(DataHolder *dataHolderPtr, char *name) {
    //assert(*dataHolderPtr == NULL);

    *dataHolderPtr = malloc(sizeof(struct DataEntry));

    (*dataHolderPtr)->name = malloc(strlen(name) + 1);
    memcpy((*dataHolderPtr)->name, name, strlen(name) + 1);

    (*dataHolderPtr)->prev = NULL;
    (*dataHolderPtr)->next = NULL;
    (*dataHolderPtr)->subHolder = NULL;
}

void dataHolderDestroy(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    free(dataHolder->name);

    dataHolderDestroy(dataHolder->prev);
    dataHolderDestroy(dataHolder->next);
    dataHolderDestroy(dataHolder->subHolder);

    free(dataHolder);
}

DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName) {
    if (dataHolder->subHolder == NULL) {
        dataHolderCreate(&(dataHolder->subHolder), entryName);
        return dataHolder->subHolder;
    }

    DataHolder currentHolder = dataHolder->subHolder;

    while (1) {
        int compareNameResult =
                dataHolderStringCompare(currentHolder->name, entryName);

        if (compareNameResult == 0)
            return currentHolder;

        if (compareNameResult == 1) {
            if (currentHolder->next == NULL) {
                dataHolderCreate(&(currentHolder->next), entryName);
                return dataHolder->next;
            }

            currentHolder = currentHolder->next;
        } else if (compareNameResult == -1) {
            if (currentHolder->prev == NULL) {
                dataHolderCreate(&(currentHolder->prev), entryName);
                return dataHolder->prev;
            }

            currentHolder = currentHolder->prev;
        }
    }
}

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

}

void dataHolderRemoveAllEntries(DataHolder dataHolder) {
    assert(dataHolder != NULL);

    dataHolderDestroy(dataHolder->subHolder);
    dataHolder->subHolder = NULL;
}

DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    if (dataHolder->subHolder == NULL)
        return NULL;

    DataHolder currentHolder = dataHolder->subHolder;

    while (1) {
        int compareNameResult =
                dataHolderStringCompare(currentHolder->name, entryName);

        if (compareNameResult == 0)
            return currentHolder;

        if (compareNameResult == 1) {
            if (currentHolder->next == NULL)
                return NULL;

            currentHolder = currentHolder->next;
        } else if (compareNameResult == -1) {
            if (currentHolder->prev == NULL)
                return NULL;

            currentHolder = currentHolder->prev;
        }
    }
}

void dataHolderPrintEntryName(DataHolder dataHolder) {
    printf("%s\n", dataHolder->name);
}

void dataHolderPrintAllEntries(DataHolder dataHolder) {

}