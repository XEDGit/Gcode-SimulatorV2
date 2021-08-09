#include <stdlib.h>

int floorFloat(float value)
{
	if(((int) (value * 10)) % 10  >= 5)
		return ((int) (value + 0.5f));
	return ((int)value);	
}
