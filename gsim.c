/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gsim.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tom <tom@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/04 17:19:32 by tom           #+#    #+#                 */
/*   Updated: 2021/08/04 19:18:18 by tom           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

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
			return("notneeded");	
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
	return("notneeded");
			
}
int main() 
{
  	FILE *file = fopen("hello.gcode", "r");
	char line[256];
	int index = 0;
	char **modell;
	char ret[100];
	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file))
		{
			if(lineHasZ(cleanGcodeLine(line, ret)))
				printf("%s\n", cleanGcodeLine(line, ret));
			if(strcmp(cleanGcodeLine(line,ret), "X0Y220") == 0)
				break;
		}
	}
	printf("z %d\n", foundZ);
}