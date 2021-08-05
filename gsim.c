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
}	point;

typedef struct agent {
	int x;
	int y;
	int z;
	int mode;
	int speed;
	int flow;
}	agent;

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

int	axisReader(char axis, agent *printer, char *line)
{
	int i = 0;
	char	*command = malloc(5);

	while(*line != '.' && *line != ' ')
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'x')
		printer->x = atoi(command);
	else if (axis == 'y')
		printer->y = atoi(command);
	else if (axis == 'z')
		printer->z = atoi(command);
	free(command);
}

int	cleanGcodeLine(char *line, agent *printer)
{
	if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
	{
		//mode reading
		(printer->mode) = line[1] - '0';
		//x reading
		while(*line != 'X')
			line++;
		line ++;
		if(!(*line >= '0' && *line <= '9'))
			return(0);	
		axisReader('x', printer, line);
		//value reset
		value = 0;
		//y reading
		while(*line != 'Y')
			line++;
		line++;
		if(!(*line >= '0' && *line <= '9'))
			return(0);
		axisReader('y', printer, line);
		//z reading
		if(lineHasZ(line))
		{
			while(*line != 'Z')
				line++;
			line++;
			if(!(*line >= '0' && *line <= '9'))
				return(0);
			axisReader('z', printer, line);
			foundZ++;
		}
		return (1);		
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
	//agent contain values useful for printing simulation result
	agent printerHead = {0};
	char ret[100];
	point currentPoint;
	int i = 0;

	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file) )
		{

			i++;
			 
			
				printf(cleanGcodeLine(line, &printerHead));
				//putPointsIntoArray(cleanGcodeLine(line, ret), 0);
				printf("\n");
			
		}
	}
	printf("z %d\n", foundZ);
}