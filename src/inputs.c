#include <stdlib.h>
#include <time.h>
#include "inputs.h"

Inputs *inputs_create()
{
	Inputs *inputs = malloc(sizeof(Inputs));
	return inputs;
}

void inputs_destroy(Inputs *inputs)
{
	free(inputs);
}
