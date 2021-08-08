// 
// HOW TO USE: ./a.* <print rateo> <file name>
//
// print rateo is expressed like 1:<rateo>
//
// file name must be the full name including extension, the full path is not needed 
//

#include "gcodesim.h"

int rateo = 1;

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

// int	strlenght(char *str)
// {
// 	int i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

// char	*stringjoin(char *s1, char *s2)
// {
// 	int lenght = strlenght(s1) + strlenght(s2);
// 	char	*p = malloc(lenght);
// 	int i = 0;
// 	for (; s1[i] != 0 && i < 100; i++)
// 	{
// 		p[i] = s1[i];
// 	}
// 	for (int j = 0; s2[j] != 0 && i < 100; i++, j++)
// 	{
// 		p[i] = s2[j];
// 	}
// 	p[i] = 0;
// 	return (p);
// }

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

void	putAxisIntoStruct(char axis, point *currentPoint, char *line, settings *currentSettings)
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
		while(*line != ' ' && *line != '\n')
			command[i++] = *line++;
		command[i] = 0;
		//i get what ure trying to do, but layer height is not always 0.2, is stored in currentSettings.layerheight
		currentPoint->z = (int) ((atof(command) / currentSettings->layerHeight) + 1);
	}
	free(command);
}

void putSettingsIntoStruct(char axis, int sign, settings *currentSettings, char *line)
{
	int i = 0;
	char	*command = malloc(100);

	//sign for min = 0, sign for max = 1
	while (!isDigit(*line))
		line++;
	while (*line != ' ' && *line != 0)
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentSettings->xMinMax[sign] = atoi(command);
	else if (axis == 'Y')
		currentSettings->yMinMax[sign] = atoi(command);
	else if (axis == 'Z')
		currentSettings->zMinMax[sign] = atoi(command);
	else if (axis == 't')
		currentSettings->layerHeight = atof(command);
	free(command);
}

int	findAxisValues(char **line, point *currentPoint, char axis, settings *currentSettings)
{
		char *temp_line = advancePtoChar(*line, axis);
		if (temp_line == 0)
			return (0);
		*line = temp_line;
		if(!isDigit(**line))
			return(0);
		putAxisIntoStruct(axis, currentPoint, *line, currentSettings);
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
		findAxisValues(&line, currentPoint, 'X', currentSettings);
		findAxisValues(&line, currentPoint, 'Y', currentSettings);
		if(lineHasZ(line))
			findAxisValues(&line, currentPoint, 'Z', currentSettings);
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
	else if (line[0] == ';' && line[1] == 'L' && line[3] == 'y')
	{
		findSettingsValues(&line, currentSettings, 't', 0);
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

char	***allocateMatrix(settings *currentSettings)
{
	char ***matrix = malloc(sizeof(char **) * currentSettings->zMinMax[1] / currentSettings->layerHeight + 1);
	for(int j = 0; j <= currentSettings->zMinMax[1] / currentSettings->layerHeight; j++)
	{
		char **z = malloc(sizeof(char *) * (currentSettings->yMinMax[1] / rateo));
		matrix[j] = z;

		for(int k = 0; k <= (currentSettings->yMinMax[1] / rateo)  - 1; k++)
		{
			char *y = malloc(sizeof(char) * (currentSettings->xMinMax[1] / rateo));
			for(int j = 0; j <= (currentSettings->xMinMax[1] / rateo) - 1; j++)
				y[j] = ' ';
			matrix[j][k] = y;
		}
	}
	return (matrix);
}

int	validateCoords(settings *currentSettings, point *currentPoint)
{
	if (currentPoint->x > currentSettings->xMinMax[1] || currentPoint->y > currentSettings->yMinMax[1] || currentPoint->z > currentSettings->zMinMax[1]/ currentSettings->layerHeight)
		return(0);
	return (1);
}

int	validateInput(int argc, char *argv[], FILE **file)
{
	if (argc == 2)
	{
		if (atoi(argv[1]) != 0 && atoi(argv[1]) < 6 && atoi(argv[1]) >= 1)
			rateo = atoi(argv[1]);
		else
		{
			printf("Error reading rateo, it must be between 1 and 5");
			return (1);
		}
		*file = fopen("hello.gcode", "r");
	}
	else if (argc == 3)
	{
		*file = fopen(argv[2], "r");
		if (atoi(argv[1]) != 0 && atoi(argv[1]) < 6 && atoi(argv[1]) >= 1)
			rateo = atoi(argv[1]);
		else
		{
			printf("Error reading rateo, it must be between 1 and 5");
			return (1);
		}
			
	}
	else
		*file = fopen("hello.gcode", "r");
		return (0);
}

int main(int argc, char *argv[]) 
{
	FILE *file;
  	if (validateInput(argc, argv, &file))
		return (1);
	char line[256];
	point *currentPoint = malloc(sizeof(point));
	settings *currentSettings = malloc(sizeof(settings));
	char ***matrix;
	int readSettings = 1;
	int i = 0;

	setZero(currentSettings, currentPoint);
	if (file != 0)
	{
		while (fgets(line, sizeof(line), file) )
		{
			if (!readSettings)
			{
				printf("Settings succesfully parsed.\n");
				matrix = allocateMatrix(currentSettings);
				printf("3D model allocated.\n");
				readSettings = 2;
			}
			i++;
			int index = readValuesFromLine(line, currentPoint, currentSettings);
			if(index == 1)
			{
				if (readSettings == 1)
				{
					readSettings = 0;
					continue;
				}
				//printf("x: %d  y: %d z: %d char: %c \n ", currentPoint->x,currentPoint->y,currentPoint->z, matrix[currentPoint->z][currentPoint->y][currentPoint->x]);
				if (validateCoords(currentSettings, currentPoint))
					matrix[currentPoint->z][(currentPoint->y / rateo) - 1][(currentPoint->x / rateo) - 1] = 'x';
			}
			else if (index == 2)
			{
				//printf("\nlh: %f\txmin: %d\txmax: %d\tymax: %d\tymax: %d\tzmin: %d\tzmax: %d\t", currentSettings->layerHeight, currentSettings->xMinMax[0], currentSettings->xMinMax[1], currentSettings->yMinMax[0], currentSettings->yMinMax[1], currentSettings->zMinMax[0], currentSettings->zMinMax[1]);
			}
		}
		//print result

		//not working
		// for(int l = 1; l <= ((currentSettings->zMinMax[1] / currentSettings->layerHeight) / rateo) - 1; l++)
		// 			matrix[0] = mergeLayers(matrix[0], matrix[l], currentSettings, rateo);
		/////////////

		for(int j = 1; j <= currentSettings->zMinMax[1] / currentSettings->layerHeight; j++)
		{
			system(CLEAR); //windows, substitute with 'clean' for Linux
			printf("====================== LAYER %d =========================================", j);
			for(int k = 0; k <= (currentSettings->yMinMax[1] / rateo) - 1; k++)
			{
				//printf("%d |", k);
				for(int l = 0; l <= (currentSettings->xMinMax[1] / rateo) - 1; l++)
					printf("%c ", matrix[j][k][l]);
				printf("\n");
			}
			//printf("\n=========================================================================\n\n");
			//break;
		}
	}
	else
		printf("Error reading file.");
	free(matrix);
	//getchar();
	return (0);
}