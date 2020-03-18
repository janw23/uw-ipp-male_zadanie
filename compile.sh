gcc -Wall -Wextra -g -std=c11 -c src/dataholder.c
gcc -Wall -Wextra -g -std=c11 -c src/commandinterpreter.c
gcc -Wall -Wextra -g -std=c11 -c src/commandhandler.c
gcc -Wall -Wextra -g -std=c11 -o bin/forests src/forests.c dataholder.o commandhandler.o commandinterpreter.o
rm *.o
