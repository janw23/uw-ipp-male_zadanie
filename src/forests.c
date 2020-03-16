//Główny plik programu male_zadanie
//Wczytuje dane z wejścia i przetwarza je dysponując odpowiednimi modułami

#include "commandhandler.h"
#include "dataholder.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void handleInput(DataHolder dataHolder)
{
    size_t bufsize = 64;
    char *buffer = malloc(bufsize * sizeof(char));

    //czyta kolejne linie dopóki nie wystąpi 'samotny' znak \n
    while(getline(&buffer, &bufsize, stdin) > 1)
    {
        char *reply = handleCommand(buffer, dataHolder);
        printf("%s\n", reply);
    }

    free(buffer);
}

int main()
{
    DataHolder dataHolder = NULL;
    dataHolderCreate(&dataHolder);

    handleInput(dataHolder);

    dataHolderDestroy(dataHolder);
}