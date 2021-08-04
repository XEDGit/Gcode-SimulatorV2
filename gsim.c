/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gsim.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tom <tom@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/04 17:19:32 by tom           #+#    #+#                 */
/*   Updated: 2021/08/05 01:42:33 by tom           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

char printResult[200][200][200]; 

typedef struct point {
	int x;
	int y;
}point;

int foundZ = 0;


int lineHasZ(char *line)
{
	int i = 0;

	while(line[i] != 0)
	{
		if(line[i] == 'Z')
			return(1);
		i++;	
	}
	return (0);	
}
char *cleanGcodeLine(char *line , char ret[100])
{
	int		i;

	i = 0;
	if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
	{
		while(*line != 'X')
			line++;
		line ++;	
		if(!(*line >= '0' && *line <= '9'))
			return(0);	
		while(*line != '.' && *line != ' ')
			ret[i++] = *line++;
		while(*line != 'Y')
			line++;
		while(*line != '.' && *line != ' ')
			ret[i++] = *line++;
		
		if(lineHasZ(line))
		{
			while(*line != 'Z')
				line++;
			while(*line != ' ' && *line != 0)
			ret[i++] = *line++;
			foundZ++;
		}
		ret[i] = 0;
		return (ret);		
	}
	return(0);
			
}

void putPointsIntoArray(char *line, int z)
{
	int i = 0;
	char x[5] = " ";
	char y[5] = " ";
	
	while(*line != 'Y')
		x[i++] = *line++;
	x[i]= 0;	
	line++;
	while(*line)
		y[i++] = *line++;
	y[i] = 0;	
	printf("x %s y %s ", x, y);
	printResult[z][atoi(x)][atoi(y)] = 'X';
}
int main() 
{
  	FILE *file = fopen("hello.gcode", "r");
	char line[256];
	int index = 0;
	char **modell;
	char ret[100];
	point currentPoint;
	int i = 0;

	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file) )
		{

			i++;
			 
			
				printf(cleanGcodeLine(line, ret));
				//putPointsIntoArray(cleanGcodeLine(line, ret), 0);
				printf("\n");
			
		}
	}
	printf("z %d\n", foundZ);
}