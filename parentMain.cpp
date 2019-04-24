/*Demo for sharing data between two processes. Child process is created and called by parent process.*/
#include <stdio.h>
#include <Windows.h>
#include "shared_struct.h"
#include "sharedData.h"

#define UPDATE_INTERVAL_S (0.5)

int main() {
	printf("PARENT\n");

	char* exePath = "../Debug/ChildProcess.exe";
	char* sharedDataName = "mySharedData";
	char exePathAndSharedDataName[255];
	sprintf_s(exePathAndSharedDataName, "%s %s", exePath, sharedDataName);
	CSharedStruct<SHARED_DATA> SharedData;
	SharedData.Acquire(sharedDataName);

	sprintf_s(SharedData->inputs.name, "%s", "samil");
	SharedData->inputs.value = 7;

	SharedData->outputs.isStarted = false;
	SharedData->outputs.isEnded = false;

	//Start child process:
	STARTUPINFO						si;
	PROCESS_INFORMATION				pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(NULL, exePathAndSharedDataName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed for %s! Error code = %d\n", exePathAndSharedDataName, GetLastError());
	}
	else {
		//Get outputs at defined time intervals:
		clock_t start = clock();
		clock_t t0 = start;
		printf("\nPARENT Waiting for child start...\n");
		while (!SharedData->outputs.isEnded) {
			if (SharedData->outputs.isStarted) {
				clock_t now = clock();
				if (calcTime_s(start, now) >= UPDATE_INTERVAL_S) {
					double t_s = calcTime_s(t0, now);
					printf("PARENT t = %1.1f, outputs.time_s = %1.1f, outputs.value = %1.1f\n", t_s, SharedData->outputs.time_s, SharedData->outputs.value);
					start = now;
				}
			}
		}
		printf("PARENT Last value SharedData->outputs.value = %1.1f\n", SharedData->outputs.value);
	}

	printf("PARENT ended.\n");
	printf("Press enter...\n");
	getchar();
}