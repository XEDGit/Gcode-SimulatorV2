// 
// HOW TO USE: ./a.* <print rateo> <file name>
//
// print rateo is expressed like 1:<rateo>
//
// file name must be the full name including extension, the full path is not needed
//

#include "gcodesim.h"

int rateo = 1;

char character = 'x';

settings *currentSettings;

void	setZero(point *currentPoint)
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
	int		i = 0;
	char	command[100];

	while (*line != ' ' && axis != 'Z' && *line != '\n')
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentPoint->x = roundFloat(atof(command) / rateo);
	else if (axis == 'Y')
		currentPoint->y = roundFloat(atof(command) / rateo);
	else if (axis == 'Z')
	{
		while(*line != ' ' && *line != '\n')
			command[i++] = *line++;
		command[i] = 0;
		float val = atof(command);
		//<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
		currentPoint->z = (int) ((val / rateo / currentSettings->layerHeight) + 1);
	}
}

void	putSettingsIntoStruct(char axis, int sign, char *line)
{
	int i = 0;
	char	command[100];

	//sign for min = 0, sign for max = 1
	while (!isDigit(*line))
		line++;
	while (*line != ' ' && *line != 0)
		command[i++] = *line++;
	command[i] = 0;
	if (axis == 'X')
		currentSettings->xMinMax[sign] = atof(command) / rateo;
	else if (axis == 'Y')
		currentSettings->yMinMax[sign] = atof(command) / rateo;
	else if (axis == 'Z')
		currentSettings->zMinMax[sign] = atof(command) / rateo;
	else if (axis == 't')
		currentSettings->layerHeight = atof(command);
}

int		findAxisValues(char **line, point *currentPoint, char axis)
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

int		findSettingsValues(char **line, char axis, int sign)
{
	char *temp_line = advancePtoChar(*line, axis); 
	if (temp_line == 0)
		return (0);
	*line = temp_line;
	putSettingsIntoStruct(axis, sign, *line);
	return (1);
}

int		readValuesFromLine(char *line, point *currentPoint)
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
	else if (line[0] != ';')
		return (0);
	else if (line[2] == 'A' && line[3] == 'X')
	{
		findSettingsValues(&line, line[4], 1);
		return (2);
	}
	else if (line[2] == 'I' && line[3] == 'N')
	{
		findSettingsValues(&line, line[4], 0);
		return (2);
	}
	else if (line[1] == 'L' && line[3] == 'y')
	{
		findSettingsValues(&line, 't', 0);
		return (2);
	}
	return(0);
}

char **allocateLayer(int max)
{
	int	yaxis = currentSettings->yMinMax[1];
	int	xaxis = currentSettings->xMinMax[1];

	if (max)
		yaxis = xaxis = max;

	char **layer = malloc(sizeof(char **) * (yaxis + 1));
	for(int k = 0; k <= yaxis; k++)
	{
		char *y = malloc(sizeof(char) * (xaxis + 1));
		for(int i = 0; i <= xaxis; i++)
			y[i] = 0;
		layer[k] = y;
	}
	return (layer);
}

char	***allocateMatrix(int max)
{
	int	zaxis = currentSettings->zMinMax[1];

	if (max)
		zaxis = max;

	char ***matrix = malloc(sizeof(char **) * (zaxis + 1));
	for(int j = 0; j <= zaxis; j++)
	{
		matrix[j] = allocateLayer(max);
	}
	return (matrix);
}

int		validateInput(int argc, char *argv[], FILE **file)
{
	*file = fopen("hello.gcode", "r");
	rateo = 1;

	if (argc >= 2)
	{
		if (atoi(argv[1]) != 0 && atoi(argv[1]) <= 100 && atoi(argv[1]) >= 1)
			rateo = atoi(argv[1]);
		
	}
	else if (argc >= 3)
	{
		if (!(argv[2][0] == '_' && !argv[2][1]))
		{
			fclose(*file);
			*file = fopen(argv[2], "r");
		}
	}
	else if (argc >= 4)
		character = argv[3][0];
	
	if (!rateo)
	{
		fclose(*file);
		printf(RATEO_ERR);
		return (1);
	}
	if (!*file)
	{
		fclose(*file);
		printf(FILE_ERR);
		return (1);
	}
	return (0);
}

//axis 0 = x; 1 = y; 2 = z
int		clampValue(int value, int axis) 
{
	if (value < 0)
		value = 0;
	if (axis == 1)
	{
		if (value >= currentSettings->yMinMax[1])
		{
			value = currentSettings->yMinMax[1];
			return (value - 1);
		}
	}
	else if (axis == 0)
	{
		if (value >= currentSettings->xMinMax[1])
		{
			value = currentSettings->xMinMax[1];
			return (value - 1);
		}
	}
	else if (axis == 2)
	{
		value++;
		if (value >= currentSettings->zMinMax[1])
			value = (currentSettings->zMinMax[1]) - 1;
		return (value);
	}
	return (value);
}

void	lin_int_addPointToMatrix(point *current, point *old, char ***matrix)
{
	point temp;
	pointcpy(&temp, old);
	if (old->x >= currentSettings->xMinMax[1] ||
	old->y >= currentSettings->yMinMax[1] ||
	old->z >= currentSettings->zMinMax[1] ||
	current->x >= currentSettings->xMinMax[1] ||
	current->y >= currentSettings->yMinMax[1] ||
	current->z >= currentSettings->zMinMax[1])
		return ;
	//first point
	matrix[temp.z][temp.y][temp.x] = 1;
	//linear interpolation
	if (temp.x > current->x)
	{
		while (temp.y != current->y || temp.x != current->x)
		{
			temp.x--;
			temp.y = lin_int(old->x, old->y, current->x, current->y, temp.x);
			matrix[temp.z][temp.y][temp.x] = 1;
		}
	}
	else if (temp.x < current->x)
	{
		while (temp.y != current->y || temp.x != current->x)
		{
			temp.x++;
			temp.y = lin_int(old->x, old->y, current->x, current->y, temp.x);
			matrix[temp.z][temp.y][temp.x] = 1;
		}
	}
	else
	{
		if (temp.y > current->y)
		{
			while (temp.y != current->y || temp.x != current->x)
			{
				temp.y--;
				matrix[temp.z][temp.y][temp.x] = 1;
			}
		}
		else if (temp.y < current->y)
		{
			while (temp.y != current->y || temp.x != current->x)
			{
				temp.y++;
				matrix[current->z][temp.y][temp.x] = 1;
			}
		}
		else
		{
			matrix[current->z][temp.y][temp.x] = 1;
		}
	}
	//end point //don't know if it's needed //leave it there for now
	matrix[temp.z][temp.y][temp.x] = 1;
}

void	printMatrix(char ***matrix)
{
	for(int j = 1; j < currentSettings->zMinMax[1]; j++)
	{
		//system(CLEAR); //CLEAR defined in gcodesim.h
		char ch = character;
		printf("====================== LAYER %d =========================================\n", j);
		for(int k = 0; k <= currentSettings->yMinMax[1] - 1; k++)
		{
			for(int l = 0; l <= currentSettings->xMinMax[1] - 1; l++)
				if (matrix[j][k][l])
					printf("%c ", ch);
				else
					printf("  ");
			printf("\n");
		}
	}
}

#include <sys/time.h>
#include <unistd.h>
void	printLayer(char ***matrix , int layer)
{
	// int lastColor = 0;
	static char chars[] = " @%#*+=-:.";
	static int chlen = 0;
	static struct timeval last = {0};
	struct timeval now;
	gettimeofday(&now, 0);
	while ((((now.tv_sec - last.tv_sec) * 1000000) + (now.tv_usec -last.tv_usec)) < 15000) {
		gettimeofday(&now, 0);
		usleep(1500);
	}
	if (chlen == 0)
		while (chars[chlen++]);
	if (!system(CLEAR))
		(void)layer;
	int matrix_size = currentSettings->yMinMax[1] * currentSettings->xMinMax[1] * (currentSettings->zMinMax[1]) * 2 + (currentSettings->zMinMax[1]);
	char buffer[matrix_size + 1];
	buffer[matrix_size] = 0;
	int buff_c = 0;
	for (int k = 0; k <= (currentSettings->max); k++)
	{
		for(int l = 0; l <= (currentSettings->max); l++) {
				int c = ((float)matrix[layer][k][l] / currentSettings->max) * chlen;
				buffer[buff_c++] = chars[c];
				buffer[buff_c++] = chars[c];
		}
		buffer[buff_c++] = '\n';
	}
	write(1, buffer, buff_c);
	last = now;
}

char ***readAllLines(FILE **file)
{
	char line[256];
	int readSettings = 1;
	char ***matrix = NULL;
	point oldPoint;
	point currentPoint;

	setZero(&currentPoint);
	while (fgets(line, sizeof(line), *file))
	{
		if (!readSettings)
		{
			printf("Settings succesfully parsed.\n");
			matrix = allocateMatrix(0);
			if(matrix != 0)
				printf("3D model allocated.\n");
			readSettings = 2;
		}
		pointcpy(&oldPoint, &currentPoint);
		int index = readValuesFromLine(&line[0], &currentPoint);
		if(index == 1)
		{
			if (readSettings == 1)
			{
				currentSettings->zMinMax[1] /= currentSettings->layerHeight;
				currentSettings->max = currentSettings->xMinMax[1];
				if (currentSettings->max < currentSettings->yMinMax[1])
					currentSettings->max = currentSettings->yMinMax[1];
				if (currentSettings->max < currentSettings->zMinMax[1])
					currentSettings->max = currentSettings->zMinMax[1];
				readSettings = 0;
				continue;
			}
			if (currentPoint.mode)
				lin_int_addPointToMatrix(&currentPoint, &oldPoint, matrix);
		}
	}
	return (matrix);
}

void clearLayer(char **layer)
{
	int	max = currentSettings->max;

	for(int j = 0; j <= max; j++)
		for(int k = 0; k <= max; k++)
			layer[j][k] = 0;
}

void freeLayer(char **layer, int size)
{
	int	yaxis = currentSettings->yMinMax[1];

	if (size)
		yaxis = size;

	for(int k = 0; k <= yaxis; k++)
		free(layer[k]);
	free(layer);
}

void freeMatrix(char ***matrix, int size)
{
	int	zaxis = currentSettings->zMinMax[1];

	if (size)
		zaxis = size;

	for(int j = 0; j <= zaxis; j++)
		freeLayer(matrix[j], size? size : currentSettings->yMinMax[1]);
	free(matrix);
}

int	output(char ***matrix, double (*projection)[3][3], float angle)
{
	static char **layer = 0;
	static char	***inter = 0;
	int layers = currentSettings->max;
	if (!layer) {
		layer = allocateLayer(layers);
		inter = allocateMatrix(layers);
	}
	if (!matrix) {
		freeMatrix(inter, layers);
		freeLayer(layer, layers);
		return 0;
	}
	matRotation(matrix, inter, projection, 1, degtorad(angle), 0);
	char ***result = matRotation(inter, 0, projection, 0, degtorad(angle), layers);
	clearLayer(layer);
	for(int l = 0; l < layers; l++) {
		mergeLayers(layer, result[l], l);
	}
	printLayer(&layer, 0);
	return (0);
}

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	char ***matrix = NULL;

	currentSettings = &(settings){0};
	if (validateInput(argc, argv, &file))
		return (1);
	if (file == 0)
	{
		printf("Error reading file.");
		return (1);
	}
	matrix = readAllLines(&file);
	fclose(file);
	int iters = 2;
	double projection_buff[3][3] = {{0}};
	while (iters--)
		for (int i = 0; i < 360; i++)
			output(matrix, &projection_buff, i);
	matRotation(0, 0, 0, 0, 0, 0);
	output(0, 0, 0);
	freeMatrix(matrix, 0);
	// printf("%s", getShadeByPoint(20));
	//printf("\u258A\n");
	return (0);
}


//Comment Section



//axis:
//	x:	2	|	y:	1	|	z:	0
// int	outputcl(char ***matrix, int argc, char *argv[], FILE *file , int axis, float angle)
// {
// 	//freeMatrix(matrix);
// 	if (validateInput(argc, argv, &file))
// 		return (1);
// 	if (file != 0)
// 		matrix = readAllLines(matrix, &file);
// 	else
// 	{
// 		printf("Error reading file.");
// 		return (1);
// 	}
// 	//matrix = matRotation(matrix, 0, degtorad(angle));
// 	matrix = matRotation(matrix, 1, degtorad(angle));
// 	for(int l = 1; l < currentSettings->zMinMax[1]; l++)
// 		matrix[0] = mergeLayers(matrix[0], matrix[l]);
// 	system(CLEAR);
// 	printLayer(matrix , 0);
// 	freeMatrix(matrix);
// 	return (0);
// }



//        |  \ \ | |/ /
//           |  |\ `' ' /
//           |  ;'aorta \      / , pulmonary
//           | ;    _,   |    / / ,  arteries
//  superior | |   (  `-.;_,-' '-' ,
// vena cava | `,   `-._       _,-'_
//           |,-`.    `.)    ,<_,-'_, pulmonary
//          ,'    `.   /   ,'  `;-' _,  veins
//         ;        `./   /`,    \-'
//         | right   /   |  ;\   |\`
//         | atrium ;_,._|_,  `, ' \`
//         |        \    \ `       `,
//         `      __ `    \   left  ;,
//          \   ,'  `      \,  ventricle
//           \_(            ;,      ;;
//           |  \           `;,     ;;
//  inferior |  |`.          `;;,   ;'
// vena cava |  |  `-.        ;;;;,;'
//           |  |    |`-.._  ,;;;;;'
//           |  |    |   | ``';;;'  FL
//                   aorta


//print result

		//not working
		// for(int l = 1; l <= ((currentSettings->zMinMax[1]) / rateo) - 1; l++)
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



// int getdissedbyrafcamora(char *who)
// {
// kill(who);
// }
// int main()
// {
// 	getdissedbyrafcamora("tom");
// 	return (0);
// }



// shade backups
//
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



//  char * getShadeByPoint(int shade)
// {
// 	shade = (shade - 60) / 8;
// 	//system("tput setaf 214");
// 	switch(shade + 1) 
// 	{
// 	case 8: return("\x1b[38;5;232m\u2586");  break;
// 	case 7: return("\x1b[38;5;234m\u2586"); break;
// 	case 6: return("\x1b[38;5;236m\u2586"); break;
// 	case 5: return("\x1b[38;5;238m\u2586"); break;
// 	case 4: return("\x1b[38;5;240m\u2586");  break;
// 	case 3: return("\x1b[38;5;242m\u2586"); break;
// 	case 2: return("\x1b[38;5;244m\u2586"); break;
// 	case 1: return("\x1b[38;5;246m\u2586"); break;
// 	default: 
// 		return("\x1b[38;5;248m\u2586");
// 	 break;
// 	 return(0);
// 	}
//  }


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