/*Demo for sharing data between two processes. Child process is created and called by parent process.*/
#include <stdio.h>
#include "shared_struct.h"
#include "sharedData.h"
#define UPDATE_INTERVAL_S (0.2)

int main(int argc, char* argv[]) {
	printf("CHILD\n");
	char* sharedDataName = argv[1];
	printf("CHILD sharedDataName = %s\n", sharedDataName);

	CSharedStruct<SHARED_DATA> SharedData;
	SharedData.Acquire(sharedDataName);
	printf("CHILD SharedData->inputs.value: %1.1f\n", SharedData->inputs.value);
	printf("CHILD SharedData->inputs.name: %s\n", SharedData->inputs.name);
	SharedData->outputs.isStarted = true;
	//Get outputs at defined time intervals:
	clock_t start = clock();
	clock_t t0 = start;
	int counter = 0;
	while (!SharedData->outputs.isEnded) {
		clock_t now = clock();
		if (calcTime_s(start, now) >= UPDATE_INTERVAL_S) {
			double t_s = calcTime_s(t0, now);
			SharedData->outputs.time_s = t_s;
			SharedData->outputs.value = counter * 10.0;
			printf("CHILD counter = %d, t = %1.2f, SharedData->outputs.value = %1.1f\n", counter++, t_s, SharedData->outputs.value);
			start = now;
		}
		if (counter >= 10) {
			SharedData->outputs.isEnded = true;
		}
	}
	printf("CHILD ended.\n");
	printf("Press enter...\n");
	getchar();
}