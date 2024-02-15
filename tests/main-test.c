#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "error-handler.h"

void createProcessList_test1();
void createProcessList_test2();
void createProcessList_test3();

int main() {
	
	createProcessList_test1();
	createProcessList_test2();
    createProcessList_test3();

	return 0;
}


void createProcessList_test1() {

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
	printProcessList(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);
}


void createProcessList_test2() {

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
	printProcessList(queue);

	removeNodeFromList(queue, 2);
	printProcessList(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);
}

void createProcessList_test3() {

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
	printProcessList(queue);

	reQueue(queue);
	reQueue(queue);
	printProcessList(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);
}
