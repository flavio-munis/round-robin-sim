/*-----------------------------------------------------------------*/
/**

  @file   process.c
  @author Flávio M.
  @brief  Creates process for using in scheduler's simulatiors
 */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "process.h"
#include "error-handler.h"

/*-----------------------------------------------------------------
                  Internal Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Print all information from a process 
   @param Process* Current process struct
*/
/*-----------------------------------------------------------------*/
void printProcess(Process*);


/*-----------------------------------------------------------------*/
/**
   @brief Print all information from a ProcessNode
   @param ProcessNode* Current ProcessNode struct
*/
/*-----------------------------------------------------------------*/
void printProcessNode(ProcessNode*);


/*-----------------------------------------------------------------*/
/**
   @brief Allocate a new ProcessNode Struct. 
          Defines PPID randonly.
		  Set HIGH priority and NOT_ALIVE status by default.
   @param Process* Current process struct
*/
/*-----------------------------------------------------------------*/
ProcessNode* createProcessNode(Process*, uint16_t);


/*-----------------------------------------------------------------*/
/**
   @brief Reset next and after to NULL in a ProcessNode
   @param Process* Current process struct
*/
/*-----------------------------------------------------------------*/
void resetNode(ProcessNode*);


/*-----------------------------------------------------------------*/
/**
   @brief Remove a Node to the end of the Queue
   @param ProcessList* Current queue.
   @param uint16_t     PID for process to be removed.
*/
/*-----------------------------------------------------------------*/
void removeNodeFromList(ProcessList*, uint16_t);


/*-----------------------------------------------------------------*/
/**
   @brief Find a process in the queue, 
          IF the process can't be found than returns NULL.
   @param  Process*     Current process struct
   @param  uint16_t     PID of wanted process
   @return ProcessNode* Process searched
*/
/*-----------------------------------------------------------------*/
ProcessNode* getProcessNode(ProcessList*, uint16_t);


/*-----------------------------------------------------------------*/
/**
   @brief Free a ProcessNode struct
   @param ProcessNode* Current ProcessNode struct
*/
/*-----------------------------------------------------------------*/
void freeProcessNode(ProcessNode*);


/*-----------------------------------------------------------------
                      Functions Implementation
  -----------------------------------------------------------------*/
ProcessList* createProcessList(Process** processArr, uint16_t totalProcess) {

	ProcessList* queue = NULL;
	ProcessNode* node = NULL;

	queue = createEmptyProcessList();

	srand(time(NULL));  // Initiate random function for ppid generation

	for (uint16_t i = 0; i < totalProcess; i++) {
		node = createProcessNode(processArr[i], i);
	    addNodeToList(queue, node);
	}

	return queue;
}

Process* createProcess(unsigned int cpuTime, 
					   unsigned int creationTime,
					   IO_types* IoAccess,
					   unsigned int* IoTime,
					   unsigned int totalAccess) {

	ProcessIO* ioList;
	Process* newProcess = (Process*) malloc(sizeof(Process));
	checkNullPointer((void*) newProcess);

	newProcess -> cpuTime = cpuTime;
	newProcess -> creationTime = creationTime;
	
	newProcess -> ioList = (ProcessIO*) malloc(sizeof(ProcessIO));
	checkNullPointer((void*) newProcess -> ioList);

	ioList = newProcess -> ioList;

	ioList -> IoAccess = (IO_types*) malloc(sizeof(IO_types) * totalAccess);
	checkNullPointer((void*) ioList -> IoAccess);

	ioList -> IoTime = (unsigned int*) malloc((sizeof(unsigned int) * totalAccess));
	checkNullPointer((void*) ioList -> IoTime);
	
	for(int i = 0; i < totalAccess; i++) {
		ioList -> IoAccess[i] = IoAccess[i];
		ioList -> IoTime[i] = IoTime[i];
	}

	ioList -> totalIO = totalAccess;

	return newProcess;
}

void printProcess(Process* process) {

	ProcessIO* ioList = process -> ioList;

	printf("PID: %d\n", process -> pid);
	printf("PPID: %d\n", process -> ppid);
	printf("CPU TIME: %d\n", process -> cpuTime);
	printf("CREATION TIME: %d\n", process -> creationTime);
	printf("IO: ");

	for(int i = 0; i < ioList -> totalIO; i++) {
		switch(ioList -> IoAccess[i]) {
		    case DISK:
				printf("DISK IN %d\n", ioList -> IoTime[i]);
				break;

		    case TAPE:
				printf("TAPE IN %d\n", ioList -> IoTime[i]);
				break;

		    case PRINTER:
				printf("PRINTER IN %d\n", ioList -> IoTime[i]);
				break;
		}
	}
	
	puts("");
}

void printProcessNode(ProcessNode* processNode) {
	
	printProcess(processNode -> process);
	printf("TIME UNTIL EXEC: %d\n", processNode -> timeUntilExec);
	
	printf("PRIORITY: ");
	switch(processNode -> priority) {
		case HIGH:
			printf("HIGH\n");
			break;

		case LOW:
			printf("LOW\n");
			break;
	}

	printf("STATUS: ");
	switch(processNode -> status) {
		case RUNNING:
			printf("RUNNING\n");
			break;
	
	   case RUNNING_IO:
			printf("RUNNING_IO\n");
			break;

	    case BLOCKED:
			printf("BLOCKED\n");
			break;

	    case FINISHED:
			printf("FINISHED\n");
			break;
	
		case READY:
			printf("READY\n");
			break;

		case NOT_ALIVE:
			printf("NOT ALIVE\n");
			break;
	}

	printf("IO'S FINISHED: %d\n", processNode -> finishedIO);
	printf("TIME EXECUTED IN CPU: %d\n", processNode -> executionTime);

	puts("\n");
}

ProcessList* createEmptyProcessList() {
	
	ProcessList* newQueue = (ProcessList*) malloc(sizeof(ProcessList*));
	checkNullPointer((void*) newQueue);

	newQueue -> head = NULL;
	newQueue -> tail = NULL;
	newQueue -> totalProcess = 0;

	return newQueue;
}


ProcessNode* createProcessNode(Process* process, uint16_t pid) {

	ProcessNode* newNode = (ProcessNode*) malloc(sizeof(ProcessNode));
	checkNullPointer((void*) newNode);

	process -> pid = pid;
	process -> ppid = (pid == 0) ? 0 : rand() % pid;

	newNode -> process = process;
	newNode -> timeUntilExec = 0;
	newNode -> priority = HIGH;
	newNode -> status = NOT_ALIVE;
	newNode -> finishedIO = 0;
	newNode -> executionTime = 0;
	newNode -> prev = NULL;
	newNode -> next = NULL;

	return newNode;
}

void addNodeToList(ProcessList* queue, ProcessNode* node) {

	ProcessNode* aux = queue -> head;

	if(!aux) {
		queue -> head = node;
		queue -> tail = node;
	} else {
	    node -> prev = queue -> tail;
		node -> next = NULL;
		queue -> tail -> next = node;
		queue -> tail = node;
	}
	
	queue -> totalProcess++;
}

void resetNode(ProcessNode* process) {
	
	process -> next = NULL;
	process -> prev = NULL;
}

void removeNodeFromList(ProcessList* queue, uint16_t pid) {
	
	ProcessNode* aux = queue -> head;

	if(aux) {
		
		if(queue -> totalProcess == 1 && aux -> process -> pid == pid) {
		    resetNode(aux);

			queue -> head = NULL;
			queue -> tail = NULL;

			queue -> totalProcess--;
			return;
		}

		if(aux -> process -> pid == pid) {
			queue -> head = queue -> head -> next;
			queue -> head -> prev = NULL;
			
			resetNode(aux);
			
			queue -> totalProcess--;
			return;
		}

		if(queue -> tail -> process -> pid == pid) {
			aux = queue -> tail;
			queue -> tail = queue -> tail -> prev;
			queue -> tail -> next = NULL;
			
			resetNode(aux);

			queue -> totalProcess--;
			return;
		}

		aux = aux -> next;

		while(aux -> next) {
			if(aux -> process -> pid == pid) {
				aux -> prev -> next = aux -> next;
				aux -> next -> prev = aux -> prev;
			
				resetNode(aux);

				queue -> totalProcess--;
				return;
			}

			aux = aux -> next;
		}
	}
}


void reQueue(ProcessList* queue) {

	if(queue -> head) {
		queue -> tail -> next = queue -> head;
		queue -> tail = queue -> head;
		queue -> head = queue -> tail -> next;
		queue -> tail -> next = NULL;
		queue -> head -> prev = NULL;
	}
}


void transferNodeToList(ProcessList* source, ProcessList* dest, uint16_t pid) {

	ProcessNode* process = getProcessNode(source, pid);

	if(process){
		removeNodeFromList(source, pid);
		addNodeToList(dest, process);
	}
}


ProcessNode* getProcessNode(ProcessList* queue, uint16_t pid) {
	
	ProcessNode* aux = queue -> head;

	if(aux) {
		while(aux) {
			if(aux -> process -> pid == pid)
				return aux;

			aux = aux -> next;
		}
	}

	return NULL;
}


void printProcessList(ProcessList* queue) {
	
	ProcessNode* aux = queue -> head;

	printf("=== LIST HAS %d ELEMENTS ===\n\n", queue -> totalProcess);

	while(aux) {
		printProcessNode(aux);
		aux = aux -> next;
	}
}


void freeProcessList(ProcessList* queue) {

	ProcessNode* aux = queue -> head;
	ProcessNode* temp;

	while(aux) {
		temp = aux -> next;
		freeProcessNode(aux);
		aux = temp;
	}

	free(queue);
}

void freeProcessNode(ProcessNode* node) {

	freeProcess(node -> process);
	free(node);
}

void freeProcess(Process* process) {

	ProcessIO* ioList = process -> ioList;

	free(ioList -> IoAccess);
	free(ioList -> IoTime);
	free(ioList);
	free(process);
}
