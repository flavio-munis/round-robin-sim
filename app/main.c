#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "process.h"
#include "scheduler.h"
#include "error-handler.h"

void testRoundRobin1() {

	ProcessList* queue;
	IO_types* IoOp;
	unsigned int* timeIoOp;
	Process** processArr; 

	processArr = (Process**) malloc(sizeof(Process*) * 3);
	checkNullPointer((void*) processArr);

	IoOp = (IO_types*) malloc(sizeof(IO_types) * 2);
	checkNullPointer((void*) IoOp);

	timeIoOp = (unsigned int*) malloc(sizeof(unsigned int) * 2);
	checkNullPointer((void*) timeIoOp);
	
	IoOp[0] = DISK;
	IoOp[1] = PRINTER;

	timeIoOp[0] = 1;
	timeIoOp[1] = 3;

	processArr[0] = createProcess(5, 0, IoOp, timeIoOp, 2);
	processArr[1] = createProcess(7, 1, IoOp, timeIoOp, 2);
    processArr[2] = createProcess(9, 2, IoOp, timeIoOp, 2);

	queue = createProcessList(processArr, 3);
    
	roundRobinAlgo(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);

}

void testRoundRobin2() {

	ProcessList* queue;
	IO_types* IoOp;
	unsigned int* timeIoOp;
	Process** processArr; 
	unsigned int cpuTime;

	processArr = (Process**) malloc(sizeof(Process*) * 10);
	checkNullPointer((void*) processArr);

	IoOp = (IO_types*) malloc(sizeof(IO_types) * 3);
	checkNullPointer((void*) IoOp);

	timeIoOp = (unsigned int*) malloc(sizeof(unsigned int) * 2);
	checkNullPointer((void*) timeIoOp);
	
	for(int i = 0; i < 10; i++) {

		if(!(i % 2)) {
			IoOp[0] = DISK;
			IoOp[1] = TAPE;
			IoOp[2] = PRINTER;

			timeIoOp[0] = 2;
			timeIoOp[1] = 6;
			timeIoOp[2] = 8;
			
			cpuTime = 10;
		} else if(!(i % 3)) {
			IoOp[0] = DISK;
			IoOp[1] = DISK;
			IoOp[2] = DISK;

			timeIoOp[0] = 1;
			timeIoOp[1] = 3;
			timeIoOp[2] = 4;
			
			cpuTime = 6;
		} else {
			IoOp[0] = PRINTER;
			IoOp[1] = DISK;
			IoOp[2] = TAPE;

			timeIoOp[0] = 3;
			timeIoOp[1] = 4;
			timeIoOp[2] = 5;
			
			cpuTime = 8;
		}

		processArr[i] = createProcess(cpuTime, i, IoOp, timeIoOp, 3);
	}

	queue = createProcessList(processArr, 10);
    
	roundRobinAlgo(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);

}

int main(int argc, char* argv[]) {

    testRoundRobin2();

	return 0;
}
