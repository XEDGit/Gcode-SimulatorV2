//
// HOW TO USE: ./a.* <print rateo> <file name>
//
// print rateo is expressed like <rateo>:1
//
// file name must be the full name including extension, the full path is not needed
//

#include "gcodesim.h"

int rateo = 1;

char character = 'x';

settings *currentSettings;

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
	return;
}

void putAxisIntoStruct(char axis, point *currentPoint, char *line)
{
	int	i = 0;
	int	halfZ = currentSettings->max / 2;
	float value;
	char *command = malloc(100);

	while (*line != ' ' && axis != 'Z' && *line != '\n')
		command[i++] = *line++;
	command[i] = 0;
	value = atof(command);
	if (axis == 'X')
		currentPoint->x = roundFloat(value / rateo);
	else if (axis == 'Y')
		currentPoint->y = roundFloat(value / rateo);
	else if (axis == 'Z')
	{
		while (*line != ' ' && *line != '\n' && i < 99)
			command[i++] = *line++;
		command[i] = 0;
		value = atof(command);
		//<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
		currentPoint->z = roundFloat(((value / currentSettings->layerHeight) / rateo) + halfZ);
		printf("\n%f	%f	%f", value, currentSettings->layerHeight, value / currentSettings->layerHeight);
	}
	free(command);
	return;
}

void putSettingsIntoStruct(char axis, int sign, char *line)
{
	int i = 0;
	int value;
	char *command = malloc(100);

	//sign for min = 0, sign for max = 1
	while (!isDigit(*line))
		line++;
	while (*line != ' ' && *line != 0)
		command[i++] = *line++;
	command[i] = 0;
	value = atof(command);
	if (axis == 'X')
		currentSettings->xMinMax[sign] = value;
	else if (axis == 'Y')
		currentSettings->yMinMax[sign] = value;
	else if (axis == 'Z')
		currentSettings->zMinMax[sign] = value;
	else if (axis == 't')
		currentSettings->layerHeight = value;
	free(command);
	return;
}

int findAxisValues(char **line, point *currentPoint, char axis)
{
	char *temp_line = advancePtoChar(*line, axis);
	if (!temp_line)
		return (0);
	*line = temp_line;
	if (!isDigit(**line))
		return (0);
	putAxisIntoStruct(axis, currentPoint, *line);
	return (1);
}

int findSettingsValues(char **line, char axis, int sign)
{
	char *temp_line = advancePtoChar(*line, axis);
	if (!temp_line)
		return (0);
	*line = temp_line;
	putSettingsIntoStruct(axis, sign, *line);
	return (1);
}

int readValuesFromLine(char *line, point *currentPoint)
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
	// else if (line[0] == ';' && line[2] == 'A' && line[3] == 'X')
	// {
	// 	findSettingsValues(&line, line[4], 1);
	// 	return (2);
	// }
	// else if (line[0] == ';' && line[2] == 'I' && line[3] == 'N')
	// {
	// 	findSettingsValues(&line, line[4], 0);
	// 	return (2);
	// }
	// else if (line[0] == ';' && line[1] == 'L' && line[3] == 'y')
	// {
	// 	findSettingsValues(&line, 't', 0);
	// 	return (2);
	// }
	return (0);
}

short ***allocateMatrix()
{
	int max = currentSettings->max;

	short ***matrix = malloc(sizeof(short **) * (max + 1));
	for (int j = 0; j <= max; j++)
	{
		short **z = malloc(sizeof(short *) * max);
		matrix[j] = z;

		for (int k = 0; k < max; k++)
		{
			short *y = malloc(sizeof(short) * max);
			for (int i = 0; i < max; i++)
				y[i] = 0;
			matrix[j][k] = y;
		}
	}
	return (matrix);
}

int validateInput(int argc, char *argv[], FILE **file)
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
		if (atoi(argv[1]) <= 100 && atoi(argv[1]) > 0)
			rateo = atoi(argv[1]);
		else
		{
			printf(RATEO_ERR);
			return (1);
		}
		return (0);
	}
	else if (argc == 4)
	{
		*file = fopen(argv[2], "r");
		character = argv[3][0];
		if (atoi(argv[1]) <= 100 && atoi(argv[1]) > 0)
			rateo = atoi(argv[1]);
		else
		{
			printf(RATEO_ERR);
			return (1);
		}
		return (0);
	}
	//VVV debug only line VVV
	*file = fopen(argv[2], "r");
	return (0);
}

int clamp(int *val)
{
	if (*val - 1 < 0)
		*val = 0;
	if (*val >= currentSettings->max)
		*val = currentSettings->max - 1;
}

void clampValues(point *temp)
{
	for (int i = 0; i < 3; i++)
	{
		int *addr;
		if (i == 0)
			clamp(&temp->x);
		else if (i == 1)
			clamp(&temp->y);
		else if (i == 2)
			clamp(&temp->z);
	}
	return;
}

void lin_int_addPointToMatrix(point *current, point *old, short ***matrix)
{
	int shades = 16;
	point *temp = malloc(sizeof(point));
	int maxZ = currentSettings->max;

	pointcpy(temp, old);
	//first point
	clampValues(temp);
	clampValues(current);
	clampValues(old);
	matrix[temp->z][temp->y][temp->x] = 1;
	//linear interpolation
	if (temp->x > current->x)
	{
		while (temp->y != current->y || temp->x != current->x)
		{
			temp->x--;
			temp->y = lin_int(old->x, old->y, current->x, current->y, temp->x);
			clampValues(temp);
			matrix[temp->z][temp->y][temp->x] = 1;
		}
	}
	else if (temp->x < current->x)
	{
		while (temp->y != current->y || temp->x != current->x)
		{
			temp->x++;
			temp->y = lin_int(old->x, old->y, current->x, current->y, temp->x);
			clampValues(temp);
			matrix[temp->z][temp->y][temp->x] = 1;
		}
	}
	else
	{
		if (temp->y > current->y)
		{
			while (temp->y != current->y || temp->x != current->x)
			{
				temp->y--;
				clampValues(temp);
				matrix[temp->z][temp->y][temp->x] = 1;
			}
		}
		else if (temp->y < current->y)
		{
			while (temp->y != current->y || temp->x != current->x)
			{
				temp->y++;
				clampValues(temp);
				matrix[current->z][temp->y][temp->x] = 1;
			}
		}
		else
		{
			clampValues(temp);
			matrix[current->z][temp->y][temp->x] = 1;
		}
	}
	free(temp);
	return;
}

void printMatrix(short ***matrix)
{
	for (int j = 1; j < currentSettings->max; j++)
	{
		//system(CLEAR); //CLEAR defined in gcodesim.h
		printf("====================== LAYER %d =========================================\n", j);
		for (int k = 0; k <= (currentSettings->max) - 1; k++)
		{
			for (int l = 0; l <= (currentSettings->max) - 1; l++)
				if (matrix[j][k][l])
					printf("%c ", character);
				else
					printf("  ");
			printf("\n");
		}
	}
	return;
}

void printLayer(short ***matrix, int layer)
{
	int lastColor = 0;
	system(CLEAR);
	for (int k = 0; k <= (currentSettings->max) - 1; k++)
	{
		for (int l = 0; l <= (currentSettings->max) - 1; l++)
			if (matrix[layer][k][l])
			{
				// if(matrix[layer][k][l] != lastColor)
				// {
				//      getShadeByPoint(matrix[layer][k][l]);
				//      lastColor = matrix[layer][k][l] ;
				// }
				//printf("%s ", getShadeByPoint(matrix[layer][k][l]));
				printf("%c ", 'x');
			}
			else
				printf("  ");
		printf("\n");
	}
	return;
}

int parseSettings(FILE **file)
{
	char *command = malloc(100);
	char line[300];
	char *cursor;
	int readSettings = 1;
	int minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
	int c = 0;

	rewind(*file);
	while (fgets(&line[0], 300, *file))
	{
		cursor = &line[0];
		if (line[0] == 'G' && (line[1] == '0' || line[1] == '1'))
		{
			int i = 0;
			if (advancePtoChar(cursor, 'X') != 0)
			{
				cursor = advancePtoChar(cursor, 'X');
				while (*cursor != ' ' && *cursor != '\n' && *cursor != 0)
				{
					command[i] = cursor[0];
					cursor++;
					i++;
				}
				command[i] = 0;
				if (!minX)
					minX = roundFloat(atof(command));
				if (roundFloat(atof(command)) > maxX)
					maxX = roundFloat(atof(command));
				else if (roundFloat(atof(command)) < minX && atof(command) > 0)
					minX = roundFloat(atof(command));
				i = 0;
			}
			if (advancePtoChar(cursor, 'Y') != 0)
			{
				cursor = advancePtoChar(cursor, 'Y');
				while (*cursor != ' ' && *cursor != '\n' && *cursor != 0)
				{
					command[i] = cursor[0];
					cursor++;
					i++;
				}
				command[i] = 0;
				if (!minY)
					minY = roundFloat(atof(command));
				if (roundFloat(atof(command)) > maxY)
					maxY = roundFloat(atof(command));
				else if (roundFloat(atof(command)) < minY && atof(command) > 0)
					minY = roundFloat(atof(command));
				i = 0;
			}
			if (advancePtoChar(cursor, 'Z') != 0)
			{
				cursor = advancePtoChar(cursor, 'Z');
				while (*cursor != ' ' && *cursor != '\n' && *cursor != 0)
					command[i++] = *cursor++;
				command[i] = 0;
				if (roundFloat(atof(command)) > maxZ)
					maxZ = roundFloat(atof(command));
				else if (roundFloat(atof(command)) < minZ && atof(command) > 0)
					minZ = roundFloat(atof(command));
				i = 0;
			}
		}
		c++;
	}
	free(command);
	rewind(*file);
	if (maxX != 0 && maxY != 0 && maxZ != 0)
	{
		currentSettings->xMinMax[0] = minX;
		currentSettings->xMinMax[1] = maxX;
		currentSettings->yMinMax[0] = minY;
		currentSettings->yMinMax[1] = maxY;
		currentSettings->zMinMax[0] = minZ;
		currentSettings->zMinMax[1] = maxZ;
		currentSettings->layerHeight = 0.3f;
	}
	else
		return (5);

	return (0);
}

void setSettingsMax()
{
	int max = currentSettings->xMinMax[1];
	if (currentSettings->yMinMax[1] > max)
		max = currentSettings->yMinMax[1];
	if (currentSettings->zMinMax[1] / currentSettings->layerHeight > max)
		max = currentSettings->zMinMax[1] / currentSettings->layerHeight;
	currentSettings->max = max / rateo;
	return;
}

short ***readAllLines(short ***matrix, FILE **file, int argc, char *argv[])
{
	char line[256];
	int readSettings = 1;
	point *oldPoint = malloc(sizeof(point));
	point *currentPoint = malloc(sizeof(point));

	setZero(currentPoint);
	while (fgets(line, 256, *file))
	{
		if (!readSettings)
		{
			printf("Settings succesfully parsed.\n");
			matrix = allocateMatrix();
			if (matrix != 0)
				printf("3D model allocated.\n");
			else
			{
				printf("Error allocating 3D model");
				exit(6);
			}
			readSettings = 2;
			//currentSettings debugging
			printf("settings:\n     xmin: %d        xmax: %d\n      ymin: %d        ymax: %d\n      zmin: %d        zmax: %d\n", currentSettings->xMinMax[0], currentSettings->xMinMax[1], currentSettings->yMinMax[0], currentSettings->yMinMax[1], currentSettings->zMinMax[0], currentSettings->zMinMax[1]);
			printf("layer heigth:	%f", currentSettings->layerHeight);
		}
		pointcpy(oldPoint, currentPoint);
		int index = readValuesFromLine(line, currentPoint);
		if (index == 1)
		{
			if (readSettings == 1)
			{
				if (currentSettings->xMinMax[1] != 0 && currentSettings->yMinMax[1] != 0 && currentSettings->zMinMax[1] != 0)
					readSettings = 0;
				else
				{
					printf("Error reading settings, starting recovery...\n");
					readSettings = parseSettings(file);
					if (!readSettings)
						printf("Recovery successful\n");
				}
				setSettingsMax();
				continue;
			}
			else if (readSettings == 5)
			{
				printf("Recovery failed. Terminating\n");
				exit(-1);
			}
			if (currentPoint->mode)
				lin_int_addPointToMatrix(currentPoint, oldPoint, matrix);
		}
	}
	free(oldPoint);
	free(currentPoint);
	return (matrix);
}

void freeMatrix(short ***matrix)
{
	int max = currentSettings->max;

	for (int j = 0; j <= max; j++)
	{
		for (int k = 0; k < max; k++)
			free(matrix[j][k]);
		free(matrix[j]);
	}
	free(matrix);
	return;
}

//x: 2 | z: 1 | y: 0
int output_two_rot(short ***matrix, int argc, char *argv[], FILE *file, int axis, int axistwo, float angle)
{
	matrix = matRotation(matrix, axis, degtorad(angle));
	matrix = matRotation(matrix, axistwo, degtorad(angle));
	for (int l = 1; l < currentSettings->max; l++)
		matrix[0] = mergeLayers(matrix[0], matrix[l]);
	printLayer(matrix, 0);
	freeMatrix(matrix);
	return (0);
}

//x: 2 | z: 1 | y: 0
int output(short ***matrix, int argc, char *argv[], FILE *file, int axis, float angle)
{
	matrix = matRotation(matrix, axis, degtorad(angle));
	//matrix = matRotation(matrix, 2, degtorad(angle));
	for (int l = 1; l < currentSettings->max; l++)
		matrix[0] = mergeLayers(matrix[0], matrix[l]);
	printLayer(matrix, 0);
	freeMatrix(matrix);
	return (0);
}

int main(int argc, char *argv[])
{
	FILE *file;
	short ***matrix;

	currentSettings = malloc(sizeof(settings));
	if (validateInput(argc, argv, &file))
		return (1);
	if (file != 0)
		matrix = readAllLines(matrix, &file, argc, argv);
	else
	{
		printf("Error reading file.");
		return (1);
	}

	//      SINGLE ROTATION
	//output(matrix, argc, argv, file, 1, 45);

	//      CONTINOUS ROTATION
	// while (1)
	// 	for (int i = 0; i < 360; i+=1)
	// 		{output(matrix, argc, argv, file, 2, i);}

	// printf("%s", getShadeByPoint(20));
	//printf("\u258A\n");
	return (0);
}

//Comment Section

//axis:
//      x:      2       |       y:      1       |       z:      0
// int  outputcl(short ***matrix, int argc, char *argv[], FILE *file , int axis, float angle)
// {
//      //freeMatrix(matrix);
//      if (validateInput(argc, argv, &file))
//              return (1);
//      if (file != 0)
//              matrix = readAllLines(matrix, &file);
//      else
//      {
//              printf("Error reading file.");
//              return (1);
//      }
//      //matrix = matRotation(matrix, 0, degtorad(angle));
//      matrix = matRotation(matrix, 1, degtorad(angle));
//      for(int l = 1; l < currentSettings->zMinMax[1] / currentSettings->layerHeight; l++)
//              matrix[0] = mergeLayers(matrix[0], matrix[l]);
//      system(CLEAR);
//      printLayer(matrix , 0);
//      freeMatrix(matrix);
//      return (0);
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
//         | right   /   |  ;\   |\
//         | atrium ;_,._|_,  `, ' \
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
// for(int l = 1; l <= ((currentSettings->zMinMax[1] / currentSettings->layerHeight) / rateo) - 1; l++)
//                      matrix[0] = mergeLayers(matrix[0], matrix[l], currentSettings, rateo);
/////////////

//printf("x: %d  y: %d z: %d char: %c \n ", currentPoint->x,currentPoint->y,currentPoint->z, matrix[currentPoint->z][currentPoint->y][currentPoint->x]);

//else if (index == 2)
//{
//printf("\nlh: %f\txmin: %d\txmax: %d\tymax: %d\tymax: %d\tzmin: %d\tzmax: %d\t"->layerHeight, currentSettings->xMinMax[0], currentSettings->xMinMax[1], currentSettings->yMinMax[0], currentSettings->yMinMax[1], currentSettings->zMinMax[0], currentSettings->zMinMax[1]);
//}

// else
//      matrix[currentPoint->z][clampValue(currentPoint->y, 1)][clampValue(currentPoint->x, 0)] = 'x';

//for debugging purposes only
// *file = fopen(argv[2], "r");
// rateo = atoi(argv[1]);

// int  strlenght(char *str)
// {
//      int i = 0;
//      while (str[i])
//              i++;
//      return (i);
// }

// char *stringjoin(char *s1, char *s2)
// {
//      int lenght = strlenght(s1) + strlenght(s2);
//      char    *p = malloc(lenght);
//      int i = 0;
//      for (; s1[i] != 0 && i < 100; i++)
//      {
//              p[i] = s1[i];
//      }
//      for (int j = 0; s2[j] != 0 && i < 100; i++, j++)
//      {
//              p[i] = s2[j];
//      }
//      p[i] = 0;
//      return (p);
// }

//love when you focus too much on something and do all the smaller nonsense details <3

// int getdissedbyrafcamora(char *who)
// {
// kill(who);
// }
// int main()
// {
//      getdissedbyrafcamora("tom");
//      return (0);
// }

// shade backups
//
// char * getShadeByPoint(int shade)
// {
//      shade = (shade - 50) / 8;
//      switch(shade + 1)
//      {
//      case 8: return("\u2598"); break;
//      case 7: return("\u2587"); break;
//      case 6: return("\u2586"); break;
//      case 5: return("\u2585"); break;
//      case 4: return("\u2584"); break;
//      case 3: return("\u2583"); break;
//      case 2: return("\u2582"); break;
//      case 1: return("\u2581"); break;
//      default:
//      return(".");
//       break;
//      }
// }

//  char * getShadeByPoint(int shade)
// {
//      shade = (shade - 60) / 8;
//      //system("tput setaf 214");
//      switch(shade + 1)
//      {
//      case 8: return("\x1b[38;5;232m\u2586");  break;
//      case 7: return("\x1b[38;5;234m\u2586"); break;
//      case 6: return("\x1b[38;5;236m\u2586"); break;
//      case 5: return("\x1b[38;5;238m\u2586"); break;
//      case 4: return("\x1b[38;5;240m\u2586");  break;
//      case 3: return("\x1b[38;5;242m\u2586"); break;
//      case 2: return("\x1b[38;5;244m\u2586"); break;
//      case 1: return("\x1b[38;5;246m\u2586"); break;
//      default:
//              return("\x1b[38;5;248m\u2586");
//       break;
//       return(0);
//      }
//  }

// char * getShadeByPoint(int shade)
// {
//      shade = (shade - 50) / 8;
//      switch(shade + 1)
//      {
//      case 1: return("\u2588");  break;
//      case 2: return("\u2597"); break;
//      case 3: return("\u2589"); break;
//      case 4: return("\u258A"); break;
//      case 5: return("\u2586"); break;
//      case 6: return("\u258B"); break;
//      case 7: return("\u2585"); break;
//      case 8:  return("\u258C"); break;
//      case 9:  return("\u2584");  break;
//      case 10:  return("\u258D"); break;
//      case 11:  return("\u2583"); break;
//      case 12:  return("\u258E"); break;
//      case 13:  return("\u2582"); break;
//      case 14:  return("\u258F"); break;
//      case 15:  return("\u2581"); break;
//      case 16:  return("\u258F"); break;
//      default:
//      return("\u2589");
//       break;
//      }
// }