#include "gcodesim.h"

int *pointToArray(point *pt)
{
	int	*p = malloc(sizeof(int) * 3);
	p[0] = pt->x;
	p[1] = pt->y;
	p[2] = pt->z;
	return (p);
}

point	*arrayToPoint(int *pt)
{
	point	*p = malloc(sizeof(point));
	p->x = pt[0];
	p->y = pt[1];
	p->z = pt[2];
	p->mode = 0;
	return (p);
}

point	*matmul(int projection[][3], int size, point *pointData)
{
	int *pt = pointToArray(pointData);
	int *result = malloc(sizeof(int) * 3);
	for (int i = 0; i < size; i++)
	{
		int temp = 0;
		for (int j = 0; j < 3; j++)
			temp += pt[j] * projection[i][j];
		result[i] = temp;
	}
	free(pt);
	return (arrayToPoint(result));
}
