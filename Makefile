compile:
	gcc -Wextra -Wall -Werror matrixManager.c readGcode.c helperFunctions.c -lm

scale:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out 5

run:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out

debug:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -fsanitize=address -g