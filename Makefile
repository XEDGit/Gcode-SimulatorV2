compile:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm

scale:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out 5

r:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm && ./a.out

d:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -fsanitize=address -g

dr:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -fsanitize=address -g && ./a.out

drs:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -fsanitize=address -g && ./a.out 2

a:
	gcc matrixManager.c readGcode.c helperFunctions.c -lm -o android.out