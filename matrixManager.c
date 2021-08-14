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

double	degtorad(int deg)
{
	double	rad = deg * M_PI / 180.0f;
	return (rad);
}

double *pointToArray(point *pt)
{
	double	*p = malloc(sizeof(double) * 3);
	p[0] = pt->x;
	p[1] = pt->y;
	p[2] = pt->z;
	return (p);
}

point	*arrayToPoint(double *pt)
{
	point	*p = malloc(sizeof(point));
	p->x = pt[0];
	p->z = pt[1];
	p->y = pt[2];
	p->mode = 0;
	return (p);
}

int vals = 0;

double	*distToCenter(int x, int y, int z)
{
	double	zaxis = (currentSettings->zMinMax[1] / currentSettings->layerHeight) / 2;
	double	yaxis = ((currentSettings->yMinMax[1] / rateo)) / 2;
	double	xaxis = ((currentSettings->xMinMax[1] / rateo)) / 2;
	double	*p = malloc(sizeof(double) * 3);
	p[0] = x - xaxis;
	p[1] = y - yaxis;
	p[2] = z - zaxis;
	return (p);
}

point	*findPointInMatrix(double *pt)
{
	double	zaxis = (currentSettings->zMinMax[1] / currentSettings->layerHeight) / 2;
	double	yaxis = ((currentSettings->yMinMax[1] / rateo)) / 2;
	double	xaxis = ((currentSettings->xMinMax[1] / rateo)) / 2;
	point	*p = malloc(sizeof(point));
	p->x = (int) pt[0] + xaxis;
	p->y = (int) pt[1] + yaxis;
	p->z = (int) pt[2] + zaxis;
	p->mode = 0;
	return (p);
}

double	*matMul(double **projection, int size, double *pt)
{
	double *result = malloc(sizeof(double) * 3);

	for (int i = 0; i < size; i++)
	{
		double temp = 0;
		for (int j = 0; j < 3; j++)
			{temp = temp + pt[j] * projection[i][j]; //printf(" y:	%d	->	%f	+=	%f\n", i, projection[i][j], pt[j] * projection[i][j]);
			}
		result[i] = temp;
	}
	return (result);
}

//axis:
//	x:	2	|	y:	1	|	z:	0
short	***matRotation(short ***matrix, int	axis, float angle)
{
	if(angle == 0)
		return (matrix);

	double	*origin;
	double	*multiplied;
	double	**projection;
	point	*pos;
	int		zaxis = currentSettings->zMinMax[1] / currentSettings->layerHeight;
	int		yaxis = currentSettings->yMinMax[1] / rateo;
	int		xaxis = currentSettings->xMinMax[1] / rateo;
	short	***result = allocateMatrix();

	for(int j = 0; j < zaxis; j++)
	{
		for(int k = 0; k < yaxis; k++)
		{
			for(int i = 0; i < xaxis; i++)
			{
				if (!matrix[j][k][i])
					continue;
				origin = distToCenter(i, k, j);
				// if (j == 1)
				// 	printf("\noriginal	|	x: %d, y: %d, z: %d\n", i, k, j);
				projection = rotationGen(angle, axis);
				multiplied = matMul(projection, 3, origin);
				// if (j == 1)
				// 	printf("multiplied	|	x: %f, y: %f, z: %f\n", multiplied[0], multiplied[1], multiplied[2]);
				pos = findPointInMatrix(multiplied);
				// if (j == 1)
				// 	printf("final		|	x: %d, y: %d, z: %d\n", pos->x, pos->y, pos->z);
				result[clampValue(pos->z, 2)][clampValue(pos->y, 1)][clampValue(pos->x, 0)] = 1;
				free(pos);
				free(origin);
				free(multiplied);
			}
		}
	}
	//free2DF(projection);
	return (result);
}

double	**allocate2DF()
{
	double	**res = malloc(sizeof(double *) * 3);
	for (int i = 0; i < 3; i++)
	{
		double *p = malloc(sizeof(double) * 3);
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		res[i] = p;
	}
	return (res);
}

void	free2DF(double **p)
{
	for (int i = 0; i < 3; i++)
		free(p[i]);
	free(p);
}

//axis:
//	x:	2	|	y:	1	|	z:	0
double	**rotationGen(float angle, int axis)
{
	static int		lastAngle = 0;
	static double	**stored;
	double			**result;
	//allocate static float 2D array
	if (firstRotation)
	{
		firstRotation = 0;
		stored = allocate2DF();
	}
	if (lastAngle == angle)
		return (stored);
	else
	{
		result = allocate2DF();
		if (!axis)
		{
			result[0][0] = cos(angle);
			result[0][1] = sin(angle) * -1;
			result[1][0] = sin(angle);
			result[1][1] = cos(angle);
			result[2][2] = 1;
		}
		else if (axis == 1)
		{
			result[0][0] = cos(angle);
			result[0][2] = sin(angle);
			result[1][1] = 1;
			result[2][0] = sin(angle) * -1;
			result[2][2] = cos(angle);
		}
		else
		{
			result[0][0] = 1;
			result[1][1] = cos(angle);
			result[1][2] = sin(angle) * -1;
			result[2][1] = sin(angle);
			result[2][2] = cos(angle);
		}
		lastAngle = angle;
		free2DF(stored);
		stored = result;
		return (result);
	}
	return (0);
}