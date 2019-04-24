#ifndef _SHARED_DATA_
#define _SHARED_DATA_
#include <ctime>

double calcTime_s(clock_t clock1, clock_t clock2) {
	return (clock2 - clock1) / (double)CLOCKS_PER_SEC;
}

typedef struct {
	char name[255];
	double value;
}INPUTS;

typedef struct {
	bool isStarted;
	bool isEnded;
	double time_s;
	double value;
}OUTPUTS;


typedef struct {
	INPUTS inputs;
	OUTPUTS outputs;
}SHARED_DATA;

#endif