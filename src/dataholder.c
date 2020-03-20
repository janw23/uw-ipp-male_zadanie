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

    //odnosi się do wyższego 'poziomu' danych
    DataHolder overHolder;
};

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
    (*dataHolderPtr)->isRightChild = 0;
    (*dataHolderPtr)->subHolder = NULL;
    (*dataHolderPtr)->overHolder = NULL;

}

//usuwa [dataHolder] z pominięciem [dataHolder->left] i [dataHolder->right]
void dataHolderDestroyKeepChildren(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    if (dataHolder->parent != NULL) {
        if (dataHolder->isRightChild) {
            dataHolder->parent->right = NULL;
        } else {
            dataHolder->left = NULL;
        }
    }

    if (dataHolder->overHolder != NULL)
        dataHolder->overHolder->subHolder = NULL;

    free(dataHolder->name);

    dataHolderDestroy(dataHolder->subHolder);

    free(dataHolder);
}

void dataHolderDestroy(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    if (dataHolder->parent != NULL) {
        if (dataHolder->isRightChild) {
            dataHolder->parent->right = NULL;
        } else {
            dataHolder->parent->left = NULL;
        }
    }

    if (dataHolder->overHolder != NULL)
        dataHolder->overHolder->subHolder = NULL;

    dataHolderDestroy(dataHolder->left);
    dataHolderDestroy(dataHolder->right);
    dataHolderDestroy(dataHolder->subHolder);

    free(dataHolder->name);
    free(dataHolder);
}

int compareStrings(char *a, char *b) {
    if (a == NULL && b == NULL)
        return 0;

    if (a == NULL)
        return -1;

    if (b == NULL)
        return 1;

    int compareResult = strcmp(b, a);

    if (compareResult > 0)
        return 1;

    if(compareResult < 0)
        return -1;

    return 0;
}

void swapDataHolders(DataHolder a, DataHolder b) {
    struct DataEntry aCopy = *a;

    a->parent = b->parent;
    b->parent = aCopy.parent;

    a->isRightChild = b->isRightChild;
    b->isRightChild = aCopy.isRightChild;

    a->overHolder = b->overHolder;
    b->overHolder = aCopy.overHolder;

    *a = *b;
    memcpy(b, &aCopy, sizeof(aCopy));
}

DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName) {
    assert(dataHolder != NULL);

    if (dataHolder->subHolder == NULL) {
        dataHolderCreate(&(dataHolder->subHolder), entryName);
        dataHolder->subHolder->overHolder = dataHolder;

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
                currentHolder->right->isRightChild = 1;

                return currentHolder->right;
            }

            currentHolder = currentHolder->right;

        } else if (compareNameResult == -1) {
            if (currentHolder->left == NULL) {
                dataHolderCreate(&(currentHolder->left), entryName);

                currentHolder->left->parent = currentHolder;
                currentHolder->left->isRightChild = 0;

                return currentHolder->left;
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

    fprintf(stderr, "Removing entry: %s\n", entryName);

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
    DataHolder holderCopy;

    //jeśli nie ma lewego syna, to prawy syn przenoszony jest
    //na miejsce ojca, a ojciec jest usuwany
    if (target->left == NULL) {
        holderCopy = target->right;
        swapDataHolders(target, target->right);

        //ojciec, który teraz jest prawym synem, wciąż wskazuje
        //na swoje poprzednie dzieci, których błędem byłoby usunięcie
        dataHolderDestroyKeepChildren(holderCopy);

    } else if (target->right == NULL) {
        holderCopy = target->left;
        swapDataHolders(target, target->left);

        dataHolderDestroyKeepChildren(holderCopy);   //tak jak wyżej

    } else {
        //jeśli obaj synowie istnieją to podmienia ojca
        //z najbardziej lewym dzieckiem prawego syna dbając o
        //wskaźniki na dzieci tego dziecka i usuwa oryginalnego ojca

        DataHolder leftMost = dataHolderGetLeftmostChild(target->right);

        leftMost->right->parent = leftMost->parent;

        if (leftMost->isRightChild) {
            leftMost->parent->right = leftMost->right;
        } else {
            leftMost->parent->left = leftMost->right;
        }

        swapDataHolders(target, leftMost);

        //target to leftMost po zamianie
        dataHolderDestroyKeepChildren(leftMost);
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

void printAll(DataHolder dataHolder) {
    if (dataHolder == NULL)
        return;

    printAll(dataHolder->left);
    dataHolderPrintEntryName(dataHolder);
    printAll(dataHolder->right);
}

void dataHolderPrintAllEntries(DataHolder dataHolder) {
    printAll(dataHolder->subHolder);
}