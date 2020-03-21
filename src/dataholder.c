#include "dataholder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DATAHOLDER_INITIAL_LENGTH_MAX 100

//IMPLEMENTACJA STRUKTURY W POSTACI DRZEWA BST

struct DataEntry {
    //nazwa wpisu, pełniąca jednocześnie rolę klucza
    char *name;

    //"wartość" wierzchołka. Wskaźnik na drzewo BST "głębszego poziomu" danych
    DataHolder nextDepthChild;

    //lewy i prawy syn. Oba są na tej samej
    //"głębokości poziomu" danych co ich rodzic
    DataHolder leftChild, rightChild;
};

int compareStrings(char *a, char *b) {
    assert(a != NULL && b != NULL);

    int compareResult = strcmp(b, a);

    if (compareResult > 0)
        return 1;

    if (compareResult < 0)
        return -1;

    return 0;
}

void dataHolderCreate(DataHolder *dataHolderPtr, char *name) {
    assert(*dataHolderPtr == NULL);

    *dataHolderPtr = calloc(1, sizeof(struct DataEntry));

    assert((*dataHolderPtr)->name == NULL
           && (*dataHolderPtr)->leftChild == NULL
           && (*dataHolderPtr)->rightChild == NULL
           && (*dataHolderPtr)->nextDepthChild == NULL);

    int nameSize = (int) strlen(name) + 1;

    //obiekt, który utworzył "name" może wcześniej
    //zwolnić go z pamięci, więc żeby nie stracić
    //dostępu do jego zawartości, trzeba ją skopiować

    (*dataHolderPtr)->name = malloc(nameSize);
    memcpy((*dataHolderPtr)->name, name, nameSize);
}

//usuwa cały dataHolder, razem z dziećmi
void dataHolderDestroy(DataHolder dataHolder) {

    //WAŻNE: przed usunięciem dataHoldera trzeba zagwarantować
    //że żaden inny DataHolder na niego nie wskazuje (np. jest jego ojcem)

    assert(dataHolder != NULL);

    if (dataHolder->leftChild != NULL)
        dataHolderDestroy(dataHolder->leftChild);

    if (dataHolder->rightChild != NULL)
        dataHolderDestroy(dataHolder->rightChild);

    if (dataHolder->nextDepthChild != NULL)
        dataHolderDestroy(dataHolder->nextDepthChild);

    free(dataHolder->name);

    //odejdź wolny ku wieczności
    free(dataHolder);
}

//Jeśli w DataHolderze [dataHolder] istnieje wpis o nazwie [entryName]
//to zwraca ten wpis (odpowiedni DataHolder)
//w przeciwnym przypadku tworzy nowy wpis (DataHolder) i go zwraca
//Wpis jest na "głębszym poziomie" danych niż [dataHolder]
DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    if (dataHolder->nextDepthChild == NULL) {
        dataHolderCreate(&(dataHolder->nextDepthChild), entryName);
        return dataHolder->nextDepthChild;
    }

    DataHolder currentHolder = dataHolder->nextDepthChild;

    while (1) {
        int nameCompareResult = compareStrings(currentHolder->name, entryName);

        if (nameCompareResult == 0)
            return currentHolder;

        //Dzięki wskaźnikowi, w przypadku utworzenia nowego holdera
        //ojciec automatycznie będzie na niego wskazywał
        DataHolder *nextHolderPtr = nameCompareResult == 1 ?
                                    &(currentHolder->rightChild) :
                                    &(currentHolder->leftChild);

        if (*nextHolderPtr == NULL) {
            dataHolderCreate(nextHolderPtr, entryName);

            assert((nameCompareResult == 1 ?
                    currentHolder->rightChild :
                    currentHolder->leftChild) != NULL);

            return *nextHolderPtr;
        }

        currentHolder = *nextHolderPtr;
    }
}

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName) {
    assert(0);
}

void dataHolderRemoveAllEntries(DataHolder dataHolder) {
    assert(0);
}

//Zwraca DataHolder o nazwie [entryName] albo NULL jeśli takiego nie ma
DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    DataHolder currentHolder = dataHolder->nextDepthChild;

    while (currentHolder != NULL) {
        int nameCompareResult = compareStrings(currentHolder->name, entryName);

        if (nameCompareResult == 0)
            return currentHolder;

        if (nameCompareResult == 1) {
            currentHolder = currentHolder->rightChild;

        } else if (nameCompareResult == -1) {
            currentHolder = currentHolder->leftChild;
        }
    }

    return currentHolder;
}

void dataHolderPrintEntryName(DataHolder dataHolder) {
    assert(0);
}

void dataHolderPrintAllEntries(DataHolder dataHolder) {
    assert(0);
}