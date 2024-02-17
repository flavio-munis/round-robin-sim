#include <stdio.h>
#include <stdlib.h>
#include "output.h"
#include "scheduler.h"
#include "process.h"
#include "error-handler.h"

void createProcessList_test1();
void createProcessList_test2();
void createProcessList_test3();

void roundRobinAlgo_test1();

void writeToFile_test1();

int main() {
	
	createProcessList_test1();
    createProcessList_test2();
	createProcessList_test3();

	roundRobinAlgo_test1();

    writeToFile_test1();

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
	timeIoOp[1] = 7;

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

	reQueue(queue);
	reQueue(queue);
	printProcessList(queue);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);
}

void createProcessList_test3() {

	ProcessList* queue, *queue2;
	IO_types* IoOp, *IoOp2;
	unsigned int* timeIoOp, *timeIoOp2;
	Process** processArr, **processArr2; 

	processArr = (Process**) malloc(sizeof(Process*) * 3);
	checkNullPointer((void*) processArr);

	processArr2 = (Process**) malloc(sizeof(Process*) * 5);
	checkNullPointer((void*) processArr2);

	IoOp = (IO_types*) malloc(sizeof(IO_types) * 2);
	checkNullPointer((void*) IoOp);

	IoOp2 = (IO_types*) malloc(sizeof(IO_types) * 2);
	checkNullPointer((void*) IoOp2);

	timeIoOp = (unsigned int*) malloc(sizeof(unsigned int) * 2);
	checkNullPointer((void*) timeIoOp);
	
	timeIoOp2 = (unsigned int*) malloc(sizeof(unsigned int) * 2);
	checkNullPointer((void*) timeIoOp2);

	IoOp[0] = DISK;
	IoOp[1] = PRINTER;

	IoOp2[0] = DISK;
	IoOp2[1] = TAPE;

	timeIoOp[0] = 1;
	timeIoOp[1] = 3;

	timeIoOp2[0] = 2;
	timeIoOp2[1] = 3;

	processArr[0] = createProcess(5, 0, IoOp, timeIoOp, 2);
	processArr[1] = createProcess(7, 1, IoOp, timeIoOp, 2);
    processArr[2] = createProcess(9, 2, IoOp, timeIoOp, 2);

	processArr2[0] = createProcess(4, 0, IoOp2, timeIoOp2, 2);
	processArr2[1] = createProcess(8, 1, IoOp2, timeIoOp2, 2);
    processArr2[2] = createProcess(9, 2, IoOp2, timeIoOp2, 2);
	processArr2[3] = createProcess(9, 2, IoOp2, timeIoOp2, 2);
	processArr2[4] = createProcess(9, 2, IoOp2, timeIoOp2, 2);

	queue = createProcessList(processArr, 3);
	queue2 = createProcessList(processArr2, 5);

	transferNodeToList(queue2, queue, 5);
	transferNodeToList(queue2, queue, 3);
	transferNodeToList(queue2, queue, 4);
	transferNodeToList(queue, queue2, 3);
	printProcessList(queue);
	printProcessList(queue2);

	freeProcessList(queue);
	freeProcessList(queue2);
	free(processArr);
	free(processArr2);
	free(IoOp);
	free(IoOp2);
	free(timeIoOp);
	free(timeIoOp2);
}

void roundRobinAlgo_test1() {

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

void writeToFile_test1() {

	ProcessList* queue;
	IO_types* IoOp;
	unsigned int* timeIoOp;
	Process** processArr; 
	cJSON* buffer = cJSON_CreateObject();

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

	writeProcessInfo(buffer, queue -> head, 2);
	writeProcessInfo(buffer, queue -> head -> next, 2);

	writeToFile(buffer);

	freeProcessList(queue);
	free(processArr);
	free(IoOp);
	free(timeIoOp);
	cJSON_Delete(buffer);
}
