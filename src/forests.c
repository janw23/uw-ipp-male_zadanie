//Główny plik programu male_zadanie
//Wczytuje dane z wejścia i przetwarza je dysponując odpowiednimi modułami

#include "commandhandler.h"
#include "dataholder.h"

//usuwa warning "implicit declaration of getline()"
//źródło: stackoverflow.com/questions/59014090/warning-implicit-declaration-of-function-getline
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void handleInput(DataHolder dataHolder) {
    size_t bufsize = 64;
    char *buffer = malloc(bufsize * sizeof(char));

    //czyta kolejne linie dopóki nie wystąpi 'samotny' znak \n
    while (getline(&buffer, &bufsize, stdin) > 1) {
        //kopiowanie bufora, ponieważ handleCommand
        //może zmienić wskaźnik argumentu
        char *cmd = buffer;

        char *reply = handleCommand(cmd, dataHolder);

        if (strlen(reply) > 0)
            printf("%s\n", reply);
    }

    free(buffer);
}

int main() {
    DataHolder dataHolder = NULL;
    dataHolderCreate(&dataHolder, "MAIN HOLDER");

    handleInput(dataHolder);

    dataHolderDestroy(dataHolder);
}