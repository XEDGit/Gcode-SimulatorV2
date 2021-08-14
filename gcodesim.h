//include guard
#ifndef GCODESIM_H
#define GCODESIM_H

//include libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
void	setZero(point *currentPoint);
void	putAxisIntoStruct(char axis, point *currentPoint, char *line);
void	putSettingsIntoStruct(char axis, int sign, char *line);
int		findAxisValues(char **line, point *currentPoint, char axis);
int		findSettingsValues(char **line, char axis, int sign);
int		readValuesFromLine(char *line, point *currentPoint);
short	***allocateMatrix();
int		validateInput(int argc, char *argv[], FILE **file);
int		clampValue(int value, int axis);
void	lin_int_addPointToMatrix(point *current, point *old, short ***matrix);
void	printMatrix(short ***matrix);
short	***readAllLines(short ***matrix, FILE **file);
void	freeMatrix(short ***matrix);
char	*advancePtoChar(char *pointer, char c);
int		isDigit(char c);
void	pointcpy(point *p1, point *p2);
int		roundFloat(float value);
int		lin_int(int x1, int y1, int x2, int y2, int x);
short	**mergeLayers(short **l1, short **l2);
double	degtorad(int deg);
double	*pointToArray(point *pt);
point	*arrayToPoint(double *pt);
double	*distToCenter(int x, int y, int z);
point	*findPointInMatrix(double *pt);
double	*matMul(double **projection, int size, double *pt);
short	***matRotation(short ***matrix, int	axis, float angle);
double	**allocate2DF();
void	free2DF(double **p);
double	**rotationGen(float angle, int axis);
char *getShadeByPoint(int layer);

//extern variables declaration
extern settings *currentSettings;
extern int		rateo;

#endif