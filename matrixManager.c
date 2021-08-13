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
	p->z = pt[1];
	p->y = pt[2];
	p->mode = 0;
	return (p);
}

point	*distToCenter(int x, int y, int z)
{
	int	zaxis = (currentSettings->zMinMax[1] / currentSettings->layerHeight) / 2;
	int	yaxis = (currentSettings->yMinMax[1] / rateo) / 2;
	int	xaxis = (currentSettings->xMinMax[1] / rateo) / 2;
	point	*p = malloc(sizeof(point));
	p->x = x - xaxis;
	p->y = y - yaxis;
	p->z = z - zaxis;
	p->mode = 0;
}

point	*findPointInMatrix(float *pt)
{
	int	zaxis = (currentSettings->zMinMax[1] / currentSettings->layerHeight) / 2;
	int	yaxis = (currentSettings->yMinMax[1] / rateo) / 2;
	int	xaxis = (currentSettings->xMinMax[1] / rateo) / 2;
	point	*p = malloc(sizeof(point));
	p->x = (int) pt[0] + xaxis;
	p->y = (int) pt[1] + yaxis;
	p->z = (int) pt[2] + zaxis;
	p->mode = 0;
}

float	*matMul(float **projection, int size, point *pointData)
{
	float *pt = pointToArray(pointData);
	float *result = malloc(sizeof(float) * 3);

	for (int i = 0; i < size; i++)
	{
		float temp = 0;
		for (int j = 0; j < 3; j++)
			temp += pt[j] * projection[i][j];
		result[i] = temp;
	}
	free(pt);
	return (result);
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
				if (!matrix[j][k][i])
					continue;
				point	*pos = distToCenter(i, k, j);
				pos = findPointInMatrix(matMul(rotationGen(angle, axis), 3, pos));
				result[clampValue(pos->z, 2)][clampValue(pos->y, 1)][clampValue(pos->x, 0)] = 1;
			}
		}
	}
	return (result);
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

float	divide(int value)
{
	int		neg = 0;
	float	res = (float) value;
	if (res < 0)
	{
		neg = 1;
		res *= -1;
	}
	res /= 100;
	if (neg)
		res *= -1;
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
			result[0][0] = divide(roundFloat(cos(angle) * 100));
			result[0][1] = divide(roundFloat(sin(angle) * -1 * 100));
			result[1][0] = divide(roundFloat(sin(angle) * 100));
			result[1][1] = divide(roundFloat(cos(angle) * 100));
			result[2][2] = 1;
		}
		else if (axis == 1)
		{
			result[0][0] = divide(roundFloat(cos(angle) * 100));
			result[0][2] = divide(roundFloat(sin(angle) * 100));
			result[1][1] = 1;
			result[2][0] = divide(roundFloat(sin(angle) * -1 * 100));
			result[2][2] = divide(roundFloat(cos(angle) * 100));
		}
		else
		{
			result[0][0] = 1;
			result[1][1] = divide(roundFloat(cos(angle) * 100));
			result[1][2] = divide(roundFloat(sin(angle) * -1 * 100));
			result[2][1] = divide(roundFloat(sin(angle) * 100));
			result[2][2] = divide(roundFloat(cos(angle) * 100));
		}
		lastAngle = angle;
		free(stored);
		stored = result;
		return (result);
	}
	return (0);
}