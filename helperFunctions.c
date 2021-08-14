#include "gcodesim.h"

char *advancePtoChar(char *pointer, char c)
{
	while(*pointer != c && *pointer != 0)
		pointer++;
	//checking not found error
	if (!*pointer)
		return (0);
	pointer++;
	return(pointer);	
}

int isDigit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	pointcpy(point *p1, point *p2)
{
	p1->x = p2->x;
	p1->y = p2->y;
	p1->z = p2->z;
	p1->mode = p2->mode;
}

int roundFloat(float value)
{
	if (value < 0)
	{
		if((value - (int) value) * -1 >= 0.5f)
			return ((int) (value + 0.5f));
		return ((int)value);
	}
	else
	{
		if(value - (int) value >= 0.5f)
			return ((int) (value + 0.5f));
		return ((int)value);
	}
}

int	lin_int(int x1, int y1, int x2, int y2, int x)
{
	int y = y1 + (((x - x1) * (y2 - y1)) / (x2 - x1));
	return (y);
}


char	**mergeLayers(char **l1, char **l2, settings *currentSettings, int rateo)
{
	char	**res = malloc(sizeof(char *) * currentSettings->yMinMax[1]);
	for (int i = 0; i < currentSettings->yMinMax[1] - 1; i++)
	{
		char	*temp = malloc(currentSettings->xMinMax[1]);
		for (int j = 0; j < currentSettings->xMinMax[1] - 1; j++)
			temp[j] = ' ';
		res[i] = temp;
	}
	for (int i = 0; i < (currentSettings->yMinMax[1] / rateo) - 10; i++)
	{
		for (int j = 0; j < (currentSettings->xMinMax[1] / rateo) - 20; j++)
		{
			if (l2[i][j] == 'x' || l1[i][j] == 'x')
				res[i][j] = 'x';
		}
	}
	return (res);
}