compile:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm

run:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out 5