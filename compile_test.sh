gcc -Wall -Wextra -g -std=c11 src/dataholder.c -c
gcc -Wall -Wextra -g -std=c11 -o bin/test src/test.c dataholder.o
rm *.o
