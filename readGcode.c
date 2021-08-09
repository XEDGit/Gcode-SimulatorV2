// 
// HOW TO USE: ./a.* <print rateo> <file name>
//
// print rateo is expressed like 1:<rateo>
//
// file name must be the full name including extension, the full path is not needed 
//

#include "gcodesim.h"

int rateo = 1;

settings *currentSettings;

//linear interpolation formula
int	lin_int(int x1, int y1, int x2, int y2, int x)
{
	int y = y1 + (((x - x1)*(y2 - y1)) / (x2 - x1));
	return (y);
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



//love when you focus too much on something and do all the smaller nonsense details <3


void setZero(point *currentPoint)
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

void	putAxisIntoStruct(char axis, point *currentPoint, char *line)
{
	int i = 0;
	char	*command = malloc(100);

	while (*line != ' ' && axis != 'Z' && *line != '\n')
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentPoint->x = roundFloat(atof(command));
	else if (axis == 'Y')
		currentPoint->y = roundFloat(atof(command));
	else if (axis == 'Z')
	{
		while(*line != ' ' && *line != '\n')
			command[i++] = *line++;
		command[i] = 0;
		//i get what ure trying to do, but layer height is not always 0.2, is stored in currentSettings.layerheight yes thank u captain obvious <3
		currentPoint->z = (int) ((atof(command) / currentSettings->layerHeight) + 1);
	}
	free(command);
}

void putSettingsIntoStruct(char axis, int sign, char *line)
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
		currentSettings->xMinMax[sign] = atof(command);
	else if (axis == 'Y')
		currentSettings->yMinMax[sign] = atof(command);
	else if (axis == 'Z')
		currentSettings->zMinMax[sign] = atof(command);
	else if (axis == 't')
		currentSettings->layerHeight = atof(command);
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

int	findSettingsValues(char **line, char axis, int sign)
{
		char *temp_line = advancePtoChar(*line, axis); 
		if (temp_line == 0)
			return (0);
		*line = temp_line;
		putSettingsIntoStruct(axis, sign, *line);
		return (1);
}

int	readValuesFromLine(char *line, point *currentPoint)
{
	//linear interpolation
	if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
	{
		currentPoint->mode = line[1] - '0';
		findAxisValues(&line, currentPoint, 'X');
		findAxisValues(&line, currentPoint, 'Y');
		findAxisValues(&line, currentPoint, 'Z');
		return (1);
	}
	//settings
	else if (line[0] == ';' && line[2] == 'A' && line[3] == 'X')
	{
		findSettingsValues(&line, line[4], 1);
		return (2);
	}
	else if (line[0] == ';' && line[2] == 'I' && line[3] == 'N')
	{
		findSettingsValues(&line, line[4], 0);
		return (2);
	}
	else if (line[0] == ';' && line[1] == 'L' && line[3] == 'y')
	{
		findSettingsValues(&line, 't', 0);
		return (2);
	}
	return(0);
}


char	***allocateMatrix()
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

int	validateInput(int argc, char *argv[], FILE **file)
{
	if (argc == 2)
	{
		if (atoi(argv[1]) != 0 && atoi(argv[1]) <= 100 && atoi(argv[1]) >= 1)
			rateo = atoi(argv[1]);
		else
		{
			printf(RATEO_ERR);
			return (1);
		}
		*file = fopen("hello.gcode", "r");
	}
	else if (argc == 3)
	{
		*file = fopen(argv[2], "r");
		if (atoi(argv[1]) != 0 && atoi(argv[1]) <= 100 && atoi(argv[1]) >= 1)
			rateo = atoi(argv[1]);
		else
		{
			printf(RATEO_ERR);
			return (1);
		}
			
	}
	*file = fopen("hello.gcode", "r");
	return (0);
}

int	clampValue(int value, int axis) 
{
	//axis 0 = x; 1 = y; 2 = z
	value--;
	if (value < 0)
		value = 0;
	if (axis == 1)
	{
		if (value >= currentSettings->yMinMax[1] - rateo)
		{
			value = currentSettings->yMinMax[1];
			return ((value / rateo) - 1);
		}
	}
	else if (axis == 0)
	{
		if (value >= currentSettings->xMinMax[1] - rateo)
		{
			value = currentSettings->xMinMax[1];
			return ((value / rateo) - 1);
		}
	}
	else if (axis == 2)
	{
		value++;
		if (value >= (currentSettings->zMinMax[1] / currentSettings->layerHeight))
			value = (currentSettings->zMinMax[1] / currentSettings->layerHeight) - 1;
		return (value);
	}
	return (value / rateo);
}

void	lin_int_addPointToMatrix(point *current, point *old, char ***matrix)
{
	point *temp = malloc(sizeof(point));
	pointcpy(temp, old);
	//first point
	matrix[clampValue(temp->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
	//linear interpolation
	if (temp->x > current->x)
	{
		while (temp->y != current->y || temp->x != current->x)
		{
			temp->x--;
			temp->y = lin_int(old->x, old->y, current->x, current->y, temp->x);
			matrix[clampValue(temp->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
		}
	}
	else if (temp->x < current->x)
	{
		while (temp->y != current->y || temp->x != current->x)
		{
			temp->x++;
			temp->y = lin_int(old->x, old->y, current->x, current->y, temp->x);
			matrix[clampValue(temp->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
		}
	}
	else
	{
		if (temp->y > current->y)
		{
			while (temp->y != current->y || temp->x != current->x)
			{
				temp->y--;
				matrix[clampValue(temp->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
			}
		}
		else if (temp->y < current->y)
		{
			while (temp->y != current->y || temp->x != current->x)
			{
				temp->y++;
				matrix[clampValue(current->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
			}
		}
		else
		{
			matrix[clampValue(current->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
		}
	}
	//end point //don't know if it's needed
	matrix[clampValue(temp->z, 2)][clampValue(temp->y, 1)][clampValue(temp->x, 0)] = 'x';
	free(temp);
}

void	printMatrix(char ***matrix)
{
	for(int j = 1; j < currentSettings->zMinMax[1] / currentSettings->layerHeight; j++)
	{
		//system(CLEAR); //CLEAR defined in gcodesim.h
		printf("====================== LAYER %d =========================================\n", j);
		for(int k = 0; k <= (currentSettings->yMinMax[1] / rateo) - 1; k++)
		{
			for(int l = 0; l <= (currentSettings->xMinMax[1] / rateo) - 1; l++)
				printf("%c ", matrix[j][k][l]);
			printf("\n");
		}
	}
}
void readAllLines(char ***matrix, FILE **file)
{
	char line[256];
	int readSettings = 1;
	point *oldPoint = malloc(sizeof(point));
	point *currentPoint = malloc(sizeof(point));

	setZero(currentPoint);
	while (fgets(line, sizeof(line), *file) )
	{
		if (!readSettings)
		{
			printf("Settings succesfully parsed.\n");
			matrix = allocateMatrix();
			if(matrix != 0)
				printf("3D model allocated.\n");
			readSettings = 2;
		}
		pointcpy(oldPoint, currentPoint);
		int index = readValuesFromLine(line, currentPoint);
		if(index == 1)
		{
			if (readSettings == 1)
			{
				readSettings = 0;
				continue;
			}
			if (currentPoint->mode)
				lin_int_addPointToMatrix(currentPoint, oldPoint, matrix);
		}
	}
}
int main(int argc, char *argv[]) 
{
	FILE *file;
	char ***matrix;

	currentSettings = malloc(sizeof(settings));
	if (validateInput(argc, argv, &file))
		return (1);
	if (file != 0 && matrix != NULL)
		readAllLines(matrix, &file);
	else
	{
		printf("Error reading file.");
		return (0);
	}
	printMatrix(matrix);
	free(matrix);
	return (0);
}

//Comment Section

//print result

		//not working
		// for(int l = 1; l <= ((currentSettings->zMinMax[1] / currentSettings->layerHeight) / rateo) - 1; l++)
		// 			matrix[0] = mergeLayers(matrix[0], matrix[l], currentSettings, rateo);
		/////////////

//printf("x: %d  y: %d z: %d char: %c \n ", currentPoint->x,currentPoint->y,currentPoint->z, matrix[currentPoint->z][currentPoint->y][currentPoint->x]);

//else if (index == 2)
			//{
				//printf("\nlh: %f\txmin: %d\txmax: %d\tymax: %d\tymax: %d\tzmin: %d\tzmax: %d\t"->layerHeight, currentSettings->xMinMax[0], currentSettings->xMinMax[1], currentSettings->yMinMax[0], currentSettings->yMinMax[1], currentSettings->zMinMax[0], currentSettings->zMinMax[1]);
			//}

// else
				// 	matrix[currentPoint->z][clampValue(currentPoint->y, 1)][clampValue(currentPoint->x, 0)] = 'x';			

//for debugging purposes only
		// *file = fopen(argv[2], "r");
		// rateo = atoi(argv[1]);