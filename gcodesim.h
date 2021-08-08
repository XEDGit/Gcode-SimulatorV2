#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"
#ifdef _WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

char	**mergeLayers(char **l1, char **l2, settings *currentSettings, int rateo);