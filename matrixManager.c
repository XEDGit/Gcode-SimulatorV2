#include "gcodesim.h"
#include "projections.h"

int firstRotation = 1;
point	*lightSource;

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

point	distToCenter(int x, int y, int z)
{
	double	zaxis = (currentSettings->zMinMax[1]) / 2;
	double	yaxis = currentSettings->yMinMax[1] / 2;
	double	xaxis = currentSettings->xMinMax[1] / 2;
	point	p = {
		x - xaxis,
		y - yaxis,
		z - zaxis,
		0
	};
	return (p);
}

point	findPointInMatrix(point *pt)
{
	double	zaxis = (currentSettings->zMinMax[1]) / 2;
	double	yaxis = currentSettings->yMinMax[1] / 2;
	double	xaxis = currentSettings->xMinMax[1] / 2;
	
	return ((point){
		roundFloat(pt->x + xaxis),
		roundFloat(pt->y + yaxis),
		roundFloat(pt->z + zaxis),
		0
	});
}

point matMul(double (*projection)[3], int size, point *pt)
{
	double result[3];
	double ptarr[3] = {pt->x, pt->y, pt->z};

	for (int i = 0; i < size; i++)
	{
		double temp = 0;
		for (int j = 0; j < 3; j++)
			{
				temp = temp + ptarr[j] * projection[i][j]; //printf(" y:	%d	->	%f	+=	%f\n", i, projection[i][j], pt[j] * projection[i][j]);
			}
		result[i] = temp;
	}
	return ((point){result[0], result[1], result[2], 0});
}

int magnitute(int *vector )
{
	return(sqrt(pow(vector[0],2) + pow(vector[1],2) + pow(vector[2],2)));
}

int *getVectorBetweenAB(point *a, point *b)
{
	int *vector = malloc(sizeof(int) * 3);
	vector[0] = a->x - b->x;
	vector[1] = a->y - b->y;
	vector[2] = a->z - b->z;
	return(vector);
}
int vectorProduct(int *a, int *b)
{
	return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

int *crossProduct(point *a, point *b)
{
	int *result = malloc(3);
	result[0] = a->y * b->z - a->z * b->y;
	result[1] = a->z * b->x - a->x * b->z;
	result[2] = a->x * b->y - a->y * b->x;
	return(result);
}

// point *pointsAround(point *b)
// {
// 	point *result = malloc(sizeof(point) * 2);
// }

int dotProduct(point *a, point *b)
{
	int result;
	int *vectorFromLight = getVectorBetweenAB(a, b);
	int *normalizedVectorFromPoint;
	point *normalized = malloc(sizeof(point));
	normalized->x = b->x;
	normalized->y = b->y;
	normalized->z = b->z - 1;


	normalizedVectorFromPoint = crossProduct(b, normalized);
	//printf("vectorproduct: %d magproduct: %d div: %f \n", vectorProduct(vectorFromLight , normalizedVectorFromPoint), (magnitute(vectorFromLight) * magnitute(normalizedVectorFromPoint)), acos((double) (vectorProduct(vectorFromLight , normalizedVectorFromPoint) / (double) (magnitute(vectorFromLight) * magnitute(normalizedVectorFromPoint)))) * 180/M_PI);
	result = acos((double) (vectorProduct(vectorFromLight , normalizedVectorFromPoint) / (double) (magnitute(vectorFromLight) * magnitute(normalizedVectorFromPoint))))* 180/M_PI;

	free(normalized);
	free(vectorFromLight);
	return(result);
}

//axis:
//	x:	2	|	z:	1	|	y:	0
char ***	matRotation(char ***matrix, char ***res, double (*projection)[3][3], int	axis, float angle, int user_max)
{

	point		origin;
	point		multiplied;
	point		pos;

	int			max = currentSettings->max;

	int offx = (max - currentSettings->xMinMax[1]) / 2,
		offy = (max - currentSettings->yMinMax[1]) / 2,
		offz = (max - currentSettings->zMinMax[1]) / 2,
		maxx = currentSettings->xMinMax[1],
		maxy = currentSettings->yMinMax[1],
		maxz = currentSettings->zMinMax[1];

	if (user_max) {
		offx = offy = offz = 0;
		maxx = maxy = maxz = max;
	}

	static char	***result = 0;
	if (!result)
		result = allocateMatrix(max);
	if (!res)
		res = result;
	if (matrix == 0)
	{
		freeMatrix(result, max);
		return 0;
	}

	for(int j = 0; j < currentSettings->max; j++)
		clearLayer(res[j]);
	for(int j = 0; j < maxz; j++)
	{
		for(int k = 0; k < maxy; k++)
		{
			for(int i = 0; i < maxx; i++)
			{
				if (!matrix[j][k][i])
					continue;
				origin = distToCenter(i, k, j);
				// if (j == 1)
				// 	printf("\noriginal	|	x: %d, y: %d, z: %d\n", i, k, j);
				rotationGen(projection, angle, axis);
				multiplied = matMul(*projection, 3, &origin);
				// if (j == 1)
				// 	printf("multiplied	|	x: %f, y: %f, z: %f\n", multiplied[0], multiplied[1], multiplied[2]);
				pos = findPointInMatrix(&multiplied);
				// if (j == 1)
				// 	printf("final		|	x: %d, y: %d, z: %d\n", pos->x, pos->y, pos->z);
				//atan2(p1.y - p2.y, p1.x - p2.x)
				//result[clampValue(pos->z, 2)][clampValue(pos->y, 1)][clampValue(pos->x, 0)] = (int) sqrt(pow(pos->x - lightSource->x ,2) + pow(pos->y - lightSource->y ,2) + pow(pos->z - lightSource->z ,2));
				pos.x += offx;
				pos.y += offy;
				pos.z += offz;
				if (pos.x >= max)
					pos.x = max - 1;
				if (pos.y >= max)
					pos.y = max - 1;
				if (pos.z >= max)
					pos.z = max - 1;
				if (pos.x < 0)
					pos.x = 0;
				if (pos.y < 0)
					pos.y = 0;
				if (pos.z < 0)
					pos.z = 0;
				res[pos.z][pos.y][pos.x] = 1;
			}
		}
	}
	return (res);
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
//	x:	2	|	z:	1	|	y:	0
void rotationGen(double (*result)[3][3], float angle, int axis)
{
	// if (lastAngle == angle && lastAxis == axis && !firstRotation)
	// 	return (stored);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			(*result)[i][j] = 0;
	if (!axis)
	{
		(*result)[0][0] = cos(angle);
		(*result)[0][1] = sin(angle) * -1;
		(*result)[1][0] = sin(angle);
		(*result)[1][1] = cos(angle);
		(*result)[2][2] = 1;
	}
	else if (axis == 1)
	{
		(*result)[0][0] = cos(angle);
		(*result)[0][2] = sin(angle);
		(*result)[1][1] = 1;
		(*result)[2][0] = sin(angle) * -1;
		(*result)[2][2] = cos(angle);
	}
	else
	{
		(*result)[0][0] = 1;
		(*result)[1][1] = cos(angle);
		(*result)[1][2] = sin(angle) * -1;
		(*result)[2][1] = sin(angle);
		(*result)[2][2] = cos(angle);
	}
}