#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "process.h"
#include "scheduler.h"
#include "error-handler.h"

int main(int argc, char* argv[]) {

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

	return 0;
}

