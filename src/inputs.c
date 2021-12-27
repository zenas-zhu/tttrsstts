#include <stdlib.h>
#include <time.h>
#include "inputs.h"

struct inputs_ {
	Action action;
	struct timespec base;
	struct timespec mark;
};

Inputs *inputs_create()
{
	Inputs *inputs = malloc(sizeof(Inputs));
	return inputs;
}

void inputs_destroy(Inputs *inputs)
{
	free(inputs);
}

void inputs_reset_millis(Inputs *inputs)
{
	clock_gettime(CLOCK_MONOTONIC, &inputs->base);
}

void inputs_measure_millis(Inputs *inputs)
{
	clock_gettime(CLOCK_MONOTONIC, &inputs->mark);
}

long inputs_get_millis(Inputs *inputs)
{
	return (inputs->mark.tv_sec - inputs->base.tv_sec) * 1000
		+ (inputs->mark.tv_nsec - inputs->base.tv_nsec) / 1000000;
}

void inputs_set_action(Inputs *inputs, Action action)
{
	inputs->action = action;
}

Action inputs_get_action(Inputs *inputs)
{
	return inputs->action;
}

