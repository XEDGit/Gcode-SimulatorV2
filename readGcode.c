#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
char matrix[200][200][200]; 


typedef struct point {
	int x;
	int y;
	int z;
	int mode;
}	point;

typedef struct settings {
	float layerHeight;
	int xMinMax[2];
	int yMinMax[2];
	int zMinMax[2];
} settings;

int lineHasZ(char *line)
{
	int i = 0;

	while(line[i] != 0)
		if(line[i++] == 'Z')
			return(1);
	return (0);	
}

char *advancePtoChar(char *pointer, char c)
{
	while(*pointer != c)
		pointer++;
	pointer++;		
	return(pointer);			
}

int isDigit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	putAxisIntoStruct(char axis, point *currentPoint, char *line)
{
	int i = 0;
	char	*command = malloc(100);

	while (*line != '.' && *line != ' ' && axis != 'Z')
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentPoint->x = atoi(command);
	else if (axis == 'Y')
		currentPoint->y = atoi(command);
	else if (axis == 'Z')
	{
		while(*line)
			command[i++] = *line++;
		command[i] = 0;
		currentPoint->z = (int) (atof(command) / 0.2f);
	}
	free(command);
}

void findAxisValues(char **line, point *currentPoint, char axis)
{
		*line = advancePtoChar(*line, axis);	
		if(!isDigit(**line))
			return(0);	
		putAxisIntoStruct(axis, currentPoint, *line);
}

int	readValuesFromLine(char *line, point *currentPoint)
{
	if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
	{
		(currentPoint->mode) = line[1] - '0';
		findAxisValues(&line, currentPoint, 'X');
		findAxisValues(&line, currentPoint, 'Y');
		if(lineHasZ(line))
			findAxisValues(&line, currentPoint, 'Z');
		return (1);		
	}
	return(0);
}

int main() 
{
  	FILE *file = fopen("hello.gcode", "r");
	char line[256];
	point *currentPoint = malloc(sizeof(point));
	int i = 0;

	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file) )
		{
				i++;
				if(readValuesFromLine(line, currentPoint))
				{
					//printf("x: %d  y: %d z: %d mode: %d \n ", currentPoint->x,currentPoint->y,currentPoint->z,currentPoint->mode);
					matrix[currentPoint->z][currentPoint->y][currentPoint->x] = 'x';
				}	
		}
	}
	for(int k = 0; k < 200; k++)
	{
		for(int l = 0; l < 200; l++)
			printf(" %c ", matrix[1][k][l]);
		printf("\n");
	}
}