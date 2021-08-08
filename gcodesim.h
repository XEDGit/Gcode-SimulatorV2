#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"
#ifdef _WIN32
	#define CLEAR "cls"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 5"
#else
	#define CLEAR "clear"
	#define RATEO_ERR "Error reading rateo, it must be between 1 and 5\n"
#endif

char	**mergeLayers(char **l1, char **l2, settings *currentSettings, int rateo);