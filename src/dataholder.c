#include "dataholder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DATAHOLDER_INITIAL_LENGTH_MAX 100

struct DataEntry {
    char *name;

    //odnoszą się do tego samego 'poziomu'
    DataHolder left, right, parent;
    int isRightChild; //0, jeśli jest lewym synem parent, 1 jeśli prawym

    //odnosi się do niższego 'poziomu' danych
    DataHolder subHolder;
};

int compareStrings(char *a, char *b) {
    if (a == NULL && b == NULL)
        return 0;

    if (a == NULL)
        return -1;

    if (b == NULL)
        return 1;

    return strcmp(a, b);
}

void swapDataHolders(DataHolder *aPtr, DataHolder *bPtr) {
    DataHolder helpHolder = *aPtr;

    if ((*bPtr)->parent != NULL) {
        if ((*bPtr)->isRightChild) {
            (*bPtr)->parent->right = (*aPtr);
        } else {
            (*bPtr)->parent->left = (*aPtr);
        }
    }

    if ((*aPtr)->parent != NULL) {
        if ((*aPtr)->isRightChild) {
            (*aPtr)->parent->right = (*bPtr);
        } else {
            (*aPtr)->parent->left = (*bPtr);
        }
    }

    *aPtr = *bPtr; //SPRAWDZIĆ PODWÓJNĄ DEREFERENCJĘ
    *bPtr = helpHolder;
}

//Tworzy pointer na nowy DataHolder w miejscu
//wskazywanym przez podany pusty pointer
void dataHolderCreate(DataHolder *dataHolderPtr, char *name) {
    *dataHolderPtr = malloc(sizeof(struct DataEntry));

    unsigned long nameLenght = strlen(name) + 1;

    (*dataHolderPtr)->name = malloc(nameLenght);
    memcpy((*dataHolderPtr)->name, name, nameLenght);

    (*dataHolderPtr)->left = NULL;
    (*dataHolderPtr)->right = NULL;
    (*dataHolderPtr)->parent = NULL;
    (*dataHolderPtr)->subHolder = NULL;
}

void dataHolderDestroy(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    free(dataHolder->name);

    dataHolderDestroy(dataHolder->left);
    dataHolderDestroy(dataHolder->right);
    dataHolderDestroy(dataHolder->subHolder);

    free(dataHolder);
}

//usuwa [dataHolder] z pominięciem [dataHolder->left] i [dataHolder->right]
void dataHolderDestroyKeepChildren(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    free(dataHolder->name);

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
                compareStrings(currentHolder->name, entryName);

        if (compareNameResult == 0)
            return currentHolder;

        if (compareNameResult == 1) {
            if (currentHolder->right == NULL) {
                dataHolderCreate(&(currentHolder->right), entryName);

                currentHolder->right->parent = currentHolder;
                currentHolder->isRightChild = 1;

                return dataHolder->right;
            }

            currentHolder = currentHolder->right;

        } else if (compareNameResult == -1) {
            if (currentHolder->left == NULL) {
                dataHolderCreate(&(currentHolder->left), entryName);

                currentHolder->left->parent = currentHolder;
                currentHolder->isRightChild = 0;

                return dataHolder->left;
            }

            currentHolder = currentHolder->left;
        }
    }
}

DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    if (dataHolder->subHolder == NULL)
        return NULL;

    DataHolder currentHolder = dataHolder->subHolder;

    while (1) {
        int compareNameResult =
                compareStrings(currentHolder->name, entryName);

        if (compareNameResult == 0)
            return currentHolder;

        if (compareNameResult == 1) {
            if (currentHolder->right == NULL)
                return NULL;

            currentHolder = currentHolder->right;

        } else if (compareNameResult == -1) {
            if (currentHolder->left == NULL)
                return NULL;

            currentHolder = currentHolder->left;
        }
    }
}

DataHolder dataHolderGetLeftmostChild(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return NULL;

    DataHolder currentHolder = dataHolder;

    while (1) {
        if (currentHolder->left == NULL)
            return currentHolder;

        currentHolder = currentHolder->left;
    }
}

//TUTAJ MOGA BYĆ BŁĘDY Z POINTERAMI
void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    DataHolder target = dataHolderFindEntry(dataHolder, entryName);

    if (target == NULL)
        return;

    //Jeśli nie ma synów to można bezpiecznie usunąć wpis
    if (target->left == NULL && target->right == NULL) {
        dataHolderDestroy(target);
        return;
    }

    //kopia potrzebna, bo poniższe wywołania swapDataHolders
    //powodują utratę tego, na co wskazuje target albo leftMost
    DataHolder holderCopy = target;

    //jeśli nie ma lewego syna, to prawy syn przenoszony jest
    //na miejsce ojca, a ojciec jest usuwany
    if (target->left == NULL) {
        swapDataHolders(&target, &(target->right));

        //ojciec, który teraz jest prawym synem, wciąż wskazuje
        //na swoje poprzednie dzieci, których błędem byłoby usunięcie
        dataHolderDestroyKeepChildren(holderCopy);

    } else if (target->right == NULL) {
        swapDataHolders(&target, &(target->left));

        dataHolderDestroyKeepChildren(holderCopy);    //tak jak wyżej

    } else {
        //jeśli obaj synowie istnieją to podmienia ojca
        //z najbardziej lewym dzieckiem prawego syna dbając o
        //wskaźniki na dzieci tego dziecka i usuwa oryginalnego ojca

        DataHolder leftMost = dataHolderGetLeftmostChild(target->right);
        holderCopy = leftMost;

        swapDataHolders(&leftMost, &(leftMost->right));

        if (holderCopy->isRightChild) {
            holderCopy->parent->right = holderCopy->right;
        } else {
            holderCopy->parent->left = holderCopy->right;
        }


    }
}

void dataHolderRemoveAllEntries(DataHolder dataHolder) {
    assert(dataHolder != NULL);

    dataHolderDestroy(dataHolder->subHolder);
    dataHolder->subHolder = NULL;
}

void dataHolderPrintEntryName(DataHolder dataHolder) {
    printf("%s\n", dataHolder->name);
}

void dataHolderPrintAllEntries(DataHolder dataHolder) {

}