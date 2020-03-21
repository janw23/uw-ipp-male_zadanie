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
            dataHolder->parent->left = NULL;
        }
    }

    if (dataHolder->overHolder != NULL)
        dataHolder->overHolder->subHolder = NULL;

    dataHolderDestroy(dataHolder->subHolder);

    free(dataHolder->name);
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

    if (compareResult < 0)
        return -1;

    return 0;
}

void swapDataHolders(DataHolder a, DataHolder b) {
    assert(a != NULL);
    assert(b != NULL);

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

//procedura usuwania targetHoldera, jeśli jedno z jego dzieci jest NULL
void removeCaseTargetsChildIsNull(DataHolder target) {
    assert(target->left != NULL || target->right != NULL);

    //targetsChild to to dziecko, które nie jest NULL
    DataHolder targetsChild =
            (target->left != NULL ? target->left : target->right);

    assert(targetsChild != NULL);

    //Syn ojca targeta zaczyna wskazywać na odpowiedniego syna targeta
    if (target->parent != NULL) {
        if (target->isRightChild) {
            target->parent->right = targetsChild;
        } else {
            target->parent->left = targetsChild;
        }
    }

    if (target->overHolder != NULL)
        target->overHolder->subHolder = targetsChild;

    targetsChild->parent = target->parent;
    targetsChild->overHolder = target->overHolder;

    targetsChild->isRightChild = target->isRightChild;

    target->parent = NULL;
    target->left = target->right = NULL;
    target->overHolder = NULL;

    dataHolderDestroy(target);
}

//procedura usuwania targeta w przypadku, gdy oba jego dzieci istnieją
void removeCaseTargetsChildrenNotNull(DataHolder target) {
    assert(target->left != NULL && target->right != NULL);

    DataHolder leftmost = dataHolderGetLeftmostChild(target->right);

    assert(leftmost->left == NULL);
    assert(leftmost->parent != NULL);
    assert(leftmost->overHolder == NULL);

    //przepinanie między leftmost a synem i ojcem leftmost

    //na wypadek, jeśli leftmost to target->right
    if (leftmost->isRightChild) {
        leftmost->parent->right = leftmost->right;
    } else {
        leftmost->parent->left = leftmost->right;
    }

    if (leftmost->right != NULL) {
        leftmost->right->isRightChild = leftmost->isRightChild;
        leftmost->right->parent = leftmost->parent;
    }

    //przepinanie między leftmost a target

    if (target->parent != NULL) {
        if (target->isRightChild) {
            target->parent->right = leftmost;
        } else {
            target->parent->left = leftmost;
        }
    }

    if (target->overHolder != NULL)
        target->overHolder->subHolder = leftmost;

    leftmost->isRightChild = target->isRightChild;

    leftmost->parent = target->parent;
    leftmost->overHolder = target->overHolder;

    leftmost->right = target->right;
    leftmost->left = target->left;

    //target->right może stać się prawym synem leftmost i być NULL
    if (target->right != NULL)
        target->right->parent = leftmost;

    assert(target->left != NULL);
    target->left->parent = leftmost;

    target->parent = NULL;
    target->overHolder = NULL;
    target->left = target->right = NULL;

    dataHolderDestroy(target);
}

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

    if (target->left == NULL || target->right == NULL) {
        removeCaseTargetsChildIsNull(target);
    } else {
        removeCaseTargetsChildrenNotNull(target);
    }
}

void dataHolderRemoveAllEntries(DataHolder dataHolder) {
    assert(dataHolder != NULL);

    dataHolderDestroy(dataHolder->subHolder);
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