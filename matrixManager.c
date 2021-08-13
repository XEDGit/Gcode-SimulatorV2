#include "gcodesim.h"
#include "projections.h"

extern settings	*currentSettings;
extern int		rateo;
int firstRotation = 1;

int nullProj[3][3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1},
};

float *pointToArray(point *pt)
{
	float	*p = malloc(sizeof(float) * 3);
	p[0] = pt->x;
	p[1] = pt->y;
	p[2] = pt->z;
	return (p);
}

point	*arrayToPoint(float *pt)
{
	point	*p = malloc(sizeof(point));
	p->x = pt[0];
	p->y = pt[1];
	p->z = pt[2];
	p->mode = 0;
	return (p);
}

point	*newPoint(int x, int y, int z)
{
	point	*p = malloc(sizeof(point));
	p->x = x;
	p->y = y;
	p->z = z;
	p->mode = 0;
}

point	*matMul(float **projection, int size, point *pointData)
{
	float *pt = pointToArray(pointData);
	float *result = malloc(sizeof(float) * 3);
	for (int i = 0; i < size; i++)
	{
		float temp = 0;
		for (int j = 0; j < 3; j++)
			temp += (float) pt[j] * projection[i][j];
		result[i] = temp;
	}
	free(pt);
	return (arrayToPoint(result));
}

//axis:
//	x:	2	|	y:	1	|	z:	0
short	***matRotation(short ***matrix, int	axis, int angle)
{
	if(!angle)
		return (matrix);

	int	zaxis = currentSettings->zMinMax[1] / currentSettings->layerHeight;
	int	yaxis = currentSettings->yMinMax[1] / rateo;
	int	xaxis = currentSettings->xMinMax[1] / rateo;
	short	***result = allocateMatrix();

	for(int j = 0; j < zaxis; j++)
	{
		for(int k = 0; k < yaxis; k++)
		{
			for(int i = 0; i < xaxis; i++)
			{
				point	*pos = matMul(rotationGen(angle, axis), 3, newPoint(j, k, i));
				result[pos->z][pos->y][pos->x] = 1;
			}
		}
	}
}

float	**allocate2DF()
{
	float	**res = malloc(sizeof(float *) * 3);
	for (int i = 0; i < 3; i++)
	{
		float *p = malloc(sizeof(float) * 3);
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		res[i] = p;
	}
	return (res);
}

//axis:
//	x:	2	|	y:	1	|	z:	0
float	**rotationGen(int angle, int axis)
{
	static int		lastAngle = 0;
	static float	**stored;
	float			**result;
	//allocate static float 2D array
	if (firstRotation)
	{
		firstRotation = 0;
		stored = allocate2DF();
	}
	
	//checking 0 exception but value will never be 0
	//if (!angle)
	//	return (nullProj);
	if (lastAngle == angle)
		return (stored);
	else
	{
		result = allocate2DF();
		if (!axis)
		{
			result[0][0] = cos(angle);
			result[0][1] = sin(angle) * -1;4;
			result[1][0] = sin(angle);
			result[1][1] = cos(angle);
			result[2][2] = 1;
		}
		else if (axis == 1)
		{
			result[0][0] = cos(angle);
			result[0][2] = sin(angle);
			result[1][1] = 1;
			result[2][0] = sin(angle) * -1;4;
			result[2][2] = cos(angle);
		}
		else
		{
			result[0][0] = 1;
			result[1][1] = cos(angle);
			result[1][2] = sin(angle) * -1;4;
			result[2][1] = sin(angle);
			result[2][2] = cos(angle);
		}
		lastAngle = angle;
		free(stored);
		stored = result;
		return (result);
	}
	return (0);
}