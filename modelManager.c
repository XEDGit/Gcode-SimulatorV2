#include "structs.h"
#include <stdlib.h>

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