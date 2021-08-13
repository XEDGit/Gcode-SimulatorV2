//include guard
#ifndef GCODESIM_H
#define GCODESIM_H

//include libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//operating system setting
#ifdef _WIN32
	#define CLEAR "cls"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 100"
	#define FILE_ERR "Error reading file."
#else
	#define CLEAR "clear"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 100\n"
	#define FILE_ERR "Error reading file.\n"
#endif

//structures define
typedef struct point {
	int x;
	int y;
	int z;
	int mode;
}	point;

typedef struct settings {
	float layerHeight;
	int xMinMax[2];
	int yMinMax[2];
	int zMinMax[2];
} settings;

//function prototyping
char	**mergeLayers(char **l1, char **l2, settings *currentSettings, int rateo);
void	pointcpy(point *p1, point *p2);
int		isDigit(char c);
char	*advancePtoChar(char *pointer, char c);
int		roundFloat(float value);
int		lin_int(int x1, int y1, int x2, int y2, int x);
float		*pointToArray(point *pt);
point	*arrayToPoint(float *pt);
short	***allocateMatrix();
short	***matRotation(short ***matrix, int	axis, int angle);
int		clampValue(int value, int axis);
float	*matMul (float **projection, int size, point *pointData);
float	**rotationGen(int angle, int axis);

#endif