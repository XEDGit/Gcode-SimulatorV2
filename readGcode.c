#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE	200

char matrix[SIZE][SIZE][SIZE]; 


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

//linear interpolation formula
int	lin_int(int x1, int y1, int x2, int y2, int x)
{
	int y = y1 + (((x - x1)*(y2 - y1)) / (x2 - x1));
	return (y);
}

//why do we need this?
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
	while(*pointer != c && *pointer != 0)
		pointer++;
	//checking not found error
	if (!*pointer)
		return (0);
	pointer++;
	return(pointer);			
}

//love when you focus too much on something and do all the smaller nonsense details <3
int isDigit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	putAxisIntoStruct(char axis, point *currentPoint, char *line)
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
		while(*line != ' ')
			command[i++] = *line++;
		command[i] = 0;
		//i get what ure trying to do, but layer height is not always 0.2, is stored in currentSettings.layerheight
		currentPoint->z = (int) (atof(command) / 0.2f) + 1;
	}
	free(command);
}

void putSettingsIntoStruct(char axis, int sign, settings *currentSettings, char *line)
{
	int i = 0;
	char	*command = malloc(100);

	//sign for min = 0, sign for max = 1
	line++;
	while (*line != ' ' && *line != 0)
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentSettings->xMinMax[sign] = atoi(command);
	else if (axis == 'Y')
		currentSettings->yMinMax[sign] = atoi(command);
	else if (axis == 'Z')
	{
		currentSettings->zMinMax[sign] = atoi(command);
		if (!sign)
			currentSettings->layerHeight = atof(command);
	}
	free(command);
}

int	findAxisValues(char **line, point *currentPoint, char axis)
{
		char *temp_line = advancePtoChar(*line, axis);
		if (temp_line == 0)
			return (0);
		*line = temp_line;
		if(!isDigit(**line))
			return(0);
		putAxisIntoStruct(axis, currentPoint, *line);
		return (1);
}

int	findSettingsValues(char **line, settings *currentSettings, char axis, int sign)
{
		char *temp_line = advancePtoChar(*line, axis);
		if (temp_line == 0)
			return (0);
		*line = temp_line;
		putSettingsIntoStruct(axis, sign, currentSettings, *line);
		return (1);
}

int	readValuesFromLine(char *line, point *currentPoint, settings *currentSettings)
{
	//linear interpolation
	if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
	{
		currentPoint->mode = line[1] - '0';
		findAxisValues(&line, currentPoint, 'X');
		findAxisValues(&line, currentPoint, 'Y');
		if(lineHasZ(line))
			findAxisValues(&line, currentPoint, 'Z');
		//printf("used line: %s %d\n", line, currentPoint->z);
		return (1);
	}
	//settings
	else if (line[0] == ';' && line[2] == 'A' && line[3] == 'X')
	{
		findSettingsValues(&line, currentSettings, line[4], 1);
		return (2);
	}
	else if (line[0] == ';' && line[2] == 'I' && line[3] == 'N')
	{
		findSettingsValues(&line, currentSettings, line[4], 0);
		return (2);
	}
	return(0);
}

void setZero(settings *currentSettings, point *currentPoint)
{
	//when you dont reset currentPoint and i spend 40 minutes to get what's going on :)) porcoddium
	currentPoint->x = 0;
	currentPoint->y = 0;
	currentPoint->z = 0;
	currentPoint->mode = 0;
	currentSettings->layerHeight = 0;
	currentSettings->xMinMax[0] = 0;
	currentSettings->xMinMax[1] = 0;
	currentSettings->yMinMax[0] = 0;
	currentSettings->yMinMax[1] = 0;
	currentSettings->zMinMax[0] = 0;
	currentSettings->zMinMax[1] = 0;
}

int main(int argc, char *argv[]) 
{
	FILE *file;
  	if (argc != 2)
		file = fopen("hello.gcode", "r");
	else
		file = fopen(argv[1], "r");
	char line[256];
	point *currentPoint = malloc(sizeof(point));
	settings *currentSettings = malloc(sizeof(settings));
	int readSettings = 1;
	int i = 0;

	setZero(currentSettings, currentPoint);
	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file) )
		{
			if (!readSettings)
			{
				printf("Settings succesfully parsed.\n");
				//allocateMatrix();
				printf("3D model allocated.\n");
				readSettings = 2;
			}
			i++;
			int index = readValuesFromLine(line, currentPoint, currentSettings);
			if(index == 1)
			{
				if (readSettings == 1)
					readSettings = 0;
				//printf("x: %d  y: %d z: %d char: %c \n ", currentPoint->x,currentPoint->y,currentPoint->z, matrix[currentPoint->z][currentPoint->y][currentPoint->x]);
				matrix[currentPoint->z][currentPoint->y][currentPoint->x] = 'x';
			}
			else if (index == 2)
			{
				//printf("\nlh: %f\txmin: %d\txmax: %d\tymax: %d\tymax: %d\tzmin: %d\tzmax: %d\t", currentSettings->layerHeight, currentSettings->xMinMax[0], currentSettings->xMinMax[1], currentSettings->yMinMax[0], currentSettings->yMinMax[1], currentSettings->zMinMax[0], currentSettings->zMinMax[1]);
			}
		}
		for(int k = 0; k <= SIZE; k++)
		{
			//printf("%d |", k);
			for(int l = 0; l <= SIZE; l++)
				printf("%c", matrix[1][k][l]);
			printf("\n");
		}
	}
	else
		printf("Error reading file.\n");
	getchar();
}