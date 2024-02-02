compile:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -O3 -flto -Wall -Wextra -Wpedantic

scale:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out 5

run:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out

debug:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -fsanitize=address -g

sym:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -g
