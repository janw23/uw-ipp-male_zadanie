gcc -Wall -Wextra -g -std=c11 -c src/dataholder.c
gcc -Wall -Wextra -g -std=c11 -c src/commandhandler.c
gcc -Wall -Wextra -g -std=c11 -o bin/test src/test.c dataholder.o commandhandler.o
rm *.o
