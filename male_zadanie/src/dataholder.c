#include "dataholder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    int compareResult = strcmp(b, a);

    if (compareResult > 0)
        return 1;

    if (compareResult < 0)
        return -1;

    return 0;
}

void dataHolderCreate(DataHolder *dataHolderPtr, char *name) {

    *dataHolderPtr = calloc(1, sizeof(struct DataEntry));

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
            return *nextHolderPtr;
        }

        currentHolder = *nextHolderPtr;
    }
}

void removeCaseTargetHasNullChild(DataHolder *targetPtr) {

    DataHolder target = *targetPtr;

    if (target->rightChild == NULL) {
        *targetPtr = target->leftChild;
    } else {
        *targetPtr = target->rightChild;
    }

    target->rightChild = target->leftChild = NULL;
    dataHolderDestroy(target);
}

void removeCaseTargetHasBothChildren(DataHolder *targetPtr) {

    //najbardziej lewy syn prawego syna targeta
    //albo sam prawy syn targeta jeśli nie ma "bardziej lewego" syna
    DataHolder *leftmostPtr = &((*targetPtr)->rightChild);

    while ((*leftmostPtr)->leftChild != NULL)
        leftmostPtr = &((*leftmostPtr)->leftChild);

    DataHolder leftmost = *leftmostPtr;
    DataHolder target = *targetPtr;

    *leftmostPtr = leftmost->rightChild;

    leftmost->rightChild = target->rightChild;
    leftmost->leftChild = target->leftChild;

    *targetPtr = leftmost;

    target->rightChild = target->leftChild = NULL;
    dataHolderDestroy(target);
}

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName) {

    DataHolder *targetPtr = &(dataHolder->nextDepthChild);

    //znajdowanie wskaźnika na obiekt, który należy usunąć
    //żeby modyfikacje zmieniały się również w jego rodzicu
    while (*targetPtr != NULL) {
        int nameCompareResult = compareStrings((*targetPtr)->name, entryName);

        if (nameCompareResult == 0)
            break;

        if (nameCompareResult == 1) {
            targetPtr = &((*targetPtr)->rightChild);

        } else if (nameCompareResult == -1) {
            targetPtr = &((*targetPtr)->leftChild);
        }
    }

    //nie trzeba usuwac czegoś, co nie istnieje
    if (*targetPtr == NULL)
        return;

    if ((*targetPtr)->leftChild == NULL && (*targetPtr)->rightChild == NULL) {
        dataHolderDestroy(*targetPtr);
        *targetPtr = NULL;  //zmiana powinna zajść też w rodzicu targeta

        return;;
    }

    if ((*targetPtr)->leftChild == NULL || (*targetPtr)->rightChild == NULL) {
        removeCaseTargetHasNullChild(targetPtr);

    } else {
        removeCaseTargetHasBothChildren(targetPtr);
    }
}

void dataHolderRemoveAllEntries(DataHolder dataHolder) {

    if (dataHolder->nextDepthChild != NULL) {
        dataHolderDestroy(dataHolder->nextDepthChild);
        dataHolder->nextDepthChild = NULL;
    }
}

//Zwraca DataHolder o nazwie [entryName] albo NULL jeśli takiego nie ma
DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName) {

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

void addEntryAndChildrenToArray(
        DataHolder dataHolder, DataHolder **arrPtr,
        unsigned int *arrCapacity, unsigned int *arrElementsCount) {

    if (dataHolder->leftChild != NULL) {
        addEntryAndChildrenToArray(
                dataHolder->leftChild, arrPtr, arrCapacity, arrElementsCount);
    }

    if (*arrElementsCount == *arrCapacity) {
        *arrCapacity <<= 1U;
        *arrPtr = realloc(*arrPtr, *arrCapacity * sizeof(DataHolder));
    }

    (*arrPtr)[*arrElementsCount] = dataHolder;
    *arrElementsCount += 1;

    if (dataHolder->rightChild != NULL) {
        addEntryAndChildrenToArray(
                dataHolder->rightChild, arrPtr, arrCapacity, arrElementsCount);
    }
}

unsigned int dataHolderGetAllEntries
        (DataHolder dataHolder, DataHolder **outPtr) {

    if (dataHolder->nextDepthChild == NULL)
        return 0;

    unsigned int outCapacity = 2U << 10U;
    unsigned int outElementsCount = 0;

    *outPtr = malloc(outCapacity * sizeof(DataHolder));

    addEntryAndChildrenToArray
            (dataHolder->nextDepthChild, outPtr,
             &outCapacity, &outElementsCount);

    return outElementsCount;
}

void dataHolderPrintEntryAndChildren(DataHolder dataHolder) {

    if (dataHolder->leftChild != NULL)
        dataHolderPrintEntryAndChildren(dataHolder->leftChild);

    printf("%s\n", dataHolder->name);

    if (dataHolder->rightChild != NULL)
        dataHolderPrintEntryAndChildren(dataHolder->rightChild);
}

void dataHolderPrintAllEntriesOrdered(DataHolder dataHolder) {

    if (dataHolder->nextDepthChild != NULL)
        dataHolderPrintEntryAndChildren(dataHolder->nextDepthChild);
}