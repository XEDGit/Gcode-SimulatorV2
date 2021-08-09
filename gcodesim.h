#ifndef GCODESIM_H
# define GCODESIM_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
	#define CLEAR "cls"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 5"
#else
	#define CLEAR "clear"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 5\n"
#endif

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


char	**mergeLayers(char **l1, char **l2, settings *currentSettings, int rateo);
void	pointcpy(point *p1, point *p2);
int		isDigit(char c);
char	*advancePtoChar(char *pointer, char c);
int		roundFloat(float value);

#endif