/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   gsim.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tom <tom@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/04 17:19:32 by tom           #+#    #+#                 */
/*   Updated: 2021/08/04 19:03:16 by tom           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

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
			printf("%s\n", cleanGcodeLine(line, ret));
			if(strcmp(cleanGcodeLine(line,ret), "X0Y220") == 0)
				break;
		}
	}
	printf("reach");
}