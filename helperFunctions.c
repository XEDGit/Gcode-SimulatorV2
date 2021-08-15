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


short	**mergeLayers(short **l1, short **l2)
{
	// short	**res = malloc(sizeof(short *) * currentSettings->yMinMax[1]);
	// for (int i = 0; i < currentSettings->yMinMax[1] -1; i++)
	// {
	// 	res[i] = malloc(currentSettings->xMinMax[1]);
	// 	for (int j = 0; j < currentSettings->xMinMax[1] - 1; j++)
	// 		res[i][j] = 0;
	// }
	for (int i = 0; i < (currentSettings->yMinMax[1] / rateo) - 1; i++)
	{
		for (int j = 0; j < (currentSettings->xMinMax[1] / rateo) - 1; j++)
		{
			if (l2[i][j] != 0)
				l1[i][j] = l2[i][j];
		}
	}
	return (l1);
}
// int getdissedbyrafcamora(char *who)
// {
// kill(who);
// }
// int main()
// {
// 	getdissedbyrafcamora("tom");
// 	return (0);
// }

// char * getShadeByPoint(int shade)
// {
// 	shade = (shade - 50) / 8;
// 	switch(shade + 1) 
// 	{
// 	case 8: return("\u2598"); break;
// 	case 7: return("\u2587"); break;
// 	case 6: return("\u2586"); break;
// 	case 5: return("\u2585"); break;
// 	case 4: return("\u2584"); break;
// 	case 3: return("\u2583"); break;
// 	case 2: return("\u2582"); break;
// 	case 1: return("\u2581"); break;
// 	default: 
// 	return(".");
// 	 break;
// 	}
// }

char * getShadeByPoint(int shade)
{
	shade = (shade - 60) / 8;
	switch(shade + 1) 
	{
	case 1: return("X"); break;
	case 2: return("@"); break;
	case 3: return("&"); break;
	case 4: return("#"); break;
	case 5: return("+"); break;
	case 6: return("="); break;
	case 7: return(":"); break;
	case 8: return("'"); break;
	default: 
	return(".");
	 break;
	}
}


// char * getShadeByPoint(int shade)
// {
// 	shade = (shade - 50) / 8;
// 	switch(shade + 1) 
// 	{
// 	case 1: return("\u2588");  break;
// 	case 2: return("\u2597"); break;
// 	case 3: return("\u2589"); break;
// 	case 4: return("\u258A"); break;
// 	case 5: return("\u2586"); break;
// 	case 6: return("\u258B"); break;
// 	case 7: return("\u2585"); break;
// 	case 8:  return("\u258C"); break;
// 	case 9:	 return("\u2584");  break;
// 	case 10:  return("\u258D"); break;
// 	case 11:  return("\u2583"); break;
// 	case 12:  return("\u258E"); break;
// 	case 13:  return("\u2582"); break;
// 	case 14:  return("\u258F"); break;
// 	case 15:  return("\u2581"); break;
// 	case 16:  return("\u258F"); break;
// 	default: 
// 	return("\u2589");
// 	 break;
// 	}
// }