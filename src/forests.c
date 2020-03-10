//Główny plik programu male_zadanie
//Wczytuje dane z wejścia i przetwarza je dysponując odpowiednimi modułami

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void handleInput(char *buffer, size_t *bufsize)
{
    assert(buffer != NULL);

    //czyta kolejne linie dopóki nie wystąpi 'samotny' znak \n
    while(getline(&buffer, bufsize, stdin) > 1)
    {

    }
}

int main()
{
    size_t bufsize = 64;
    char *buffer = malloc(bufsize * sizeof(char));

    handleInput(buffer, &bufsize);

    free(buffer);
}