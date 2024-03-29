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


char	**mergeLayers(char **l1, char **l2, int depth)
{
	for (int i = 0; i <= currentSettings->max; i++)
	{
		for (int j = 0; j <= currentSettings->max; j++)
		{
			if (l2[i][j] != 0)
				l1[i][j] = depth;
		}
	}
	return (l1);
}

char *getShadeByPoint(int shade)
{
	shade = (shade - 50) / 8;
	switch(shade + 1) 
	{
	case 1: return("\u2588");  break;
	case 2: return("\u2597"); break;
	case 3: return("\u2589"); break;
	case 4: return("\u258A"); break;
	case 5: return("\u2586"); break;
	case 6: return("\u258B"); break;
	case 7: return("\u2585"); break;
	case 8:  return("\u258C"); break;
	case 9:	 return("\u2584");  break;
	case 10:  return("\u258D"); break;
	case 11:  return("\u2583"); break;
	case 12:  return("\u258E"); break;
	case 13:  return("\u2582"); break;
	case 14:  return("\u258F"); break;
	case 15:  return("\u2581"); break;
	case 16:  return("\u258F"); break;
	default: 
	return("\u2589");
	 break;
	}
}

// char * getShadeByPoint(int shade)
// {
// 	shade = (shade - 60) / 8;
// 	//system("tput setaf 214");
// 	switch(shade + 1) 
// 	{
// 	case 1: return("\x1b[38;5;232m\u2586");  break;
// 	case 2: return("\x1b[38;5;233m\u2586"); break;
// 	case 3: return("\x1b[38;5;234m\u2586"); break;
// 	case 4: return("\x1b[38;5;235m\u2586"); break;
// 	case 5: return("\x1b[38;5;236m\u2586"); break;
// 	case 6: return("\x1b[38;5;237m\u2586"); break;
// 	case 7: return("\x1b[38;5;238m\u2586"); break;
// 	case 8: return("\x1b[38;5;239m\u2586"); break;
// 	case 9: return("\x1b[38;5;240m\u2586");  break;
// 	case 10: return("\x1b[38;5;241m\u2586"); break;
// 	case 11: return("\x1b[38;5;242m\u2586"); break;
// 	case 12: return("\x1b[38;5;243m\u2586"); break;
// 	case 13: return("\x1b[38;5;244m\u2586"); break;
// 	case 14: return("\x1b[38;5;245m\u2586"); break;
// 	case 15: return("\x1b[38;5;246m\u2586"); break;
// 	case 16: return("\x1b[38;5;247m\u2586"); break;
// 	default: 
// 		return("\x1b[38;5;248m\u2586");
// 	 break;
// 	 return(0);
// 	}
// }