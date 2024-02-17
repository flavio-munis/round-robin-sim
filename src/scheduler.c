/*-----------------------------------------------------------------*/
/**

  @file   scheduler.c
  @author Flávio M. & Eduardo Barros
  @brief  Simulates scheduler running an round robin algorithm
 */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cJSON.h"
#include "scheduler.h"
#include "process.h"
#include "output.h"
#include "error-handler.h"


/*-----------------------------------------------------------------
                  Internal Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Initiates all 4 queues used in algorithm 
   @return Queues* Struct for queues
*/
/*-----------------------------------------------------------------*/
Queues* initQueues();


/*-----------------------------------------------------------------*/
/**
   @brief Counts all process wainting in queue that are not finished
   @param  Queues*      Algorithm queues
   @param  ProcessList* Not Alive Process queue 
   @return int          Number of process still waiting to be executed
*/
/*-----------------------------------------------------------------*/
int getTotalProcessLeft(Queues*, ProcessList*);


/*-----------------------------------------------------------------*/
/**
   @brief Tranfer new born process to high priority queue
   @param ProcessList* Not Alive Process queue
   @param Queues*      Scheduler queues 
   @param unsined int  Total execution time 
*/
/*-----------------------------------------------------------------*/
void checkIfAlive(ProcessList*, Queues*, unsigned int);


/*-----------------------------------------------------------------*/
/**
   @brief "Execute" a process for the defined QUANTUM of time and
          updates IO queue in real time.
          IF process realizes a IO operation during the QUANTUM of
		  execution, THAN, it will be transfered to the IO queue.
		  ELSE IF, process has finished all it's operations, THAN,
		  it will be transfered to Finished queue.
		  ELSE IF, process is in high priority queue, THAN, it will
		  be transfered to Low Priority queue.
		  ELSE, process will be requeud in the Low Priority queue.
   @param Queues*      Algorithm queues
   @param unsigned int Total time executed in CPU
*/
/*-----------------------------------------------------------------*/
int executeProcess(Queues*, unsigned int);


/*-----------------------------------------------------------------*/
/**
   @brief Transfer process to finished queue or IO queue if it's the
          right time
   @param  Queues*      Algorithm queues
   @param  ProcessNode* Process to be checked
   @return bool         If process was transfered or not
*/
/*-----------------------------------------------------------------*/
bool transferFinishOrIO(Queues*, ProcessNode*);


/*-----------------------------------------------------------------*/
/**
   @brief Check if a process must realize an IO Operation
   @param  ProcessNode* Process
   @return bool         If must realize a IO operation
*/
/*-----------------------------------------------------------------*/
bool checkIfIO(ProcessNode*);


/*-----------------------------------------------------------------*/
/**
   @brief Transfer the process to IO Queue and update it's properties
   @param ProcessNode* Source queue
   @param ProcessNode* IO queue
*/
/*-----------------------------------------------------------------*/
void transferToIO(ProcessList*, ProcessList*);


/*-----------------------------------------------------------------*/
/**
   @brief Update time remaining time of all process executing IO.
          IF the process is done executing the IO, THAN, it will
		  be transfered to the right queue.
   @param Queues*      Algorithm queues
   @param unsigned int Time last executed in CPU
*/
/*-----------------------------------------------------------------*/
void updateIO(Queues*, unsigned int);


/*-----------------------------------------------------------------*/
/**
   @brief Return a process done execution IO to a priority queue
   @param Queues*      Algorithm queues
   @param unsigned int Time last executed in CPU
*/
/*-----------------------------------------------------------------*/
void fromIOToExec(Queues*, uint16_t);


/*-----------------------------------------------------------------*/
/**
   @brief Checks if a process has nothing else to execute
   @param  ProcessNode* Process to be checked     
   @return bool         If process has nothing else to execute
*/
/*-----------------------------------------------------------------*/
bool checkIfFinish(ProcessNode*);


/*-----------------------------------------------------------------*/
/**
   @brief Free all memory allocated to Queues struct
   @param Queues* Algorithm queues
*/
/*-----------------------------------------------------------------*/
void freeQueues(Queues*);


/*-----------------------------------------------------------------
                      Functions Implementation
  -----------------------------------------------------------------*/
void roundRobinAlgo(ProcessList* notAlive) {
	
	unsigned int execTime = 0;
	Queues* queues = initQueues();

	while(getTotalProcessLeft(queues, notAlive) > 0) {

		if(notAlive -> totalProcess > 0)
			checkIfAlive(notAlive, queues, execTime);
		
	    execTime += executeProcess(queues, execTime);
	}

	writeToFile(queues -> buffer);

	freeQueues(queues);
}

Queues* initQueues() {
	
    Queues* newQueues = (Queues*) malloc(sizeof(Queues));
	checkNullPointer((void*) newQueues);

	newQueues -> highPri = createEmptyProcessList();
	newQueues -> lowPri = createEmptyProcessList();
	newQueues -> IO = createEmptyProcessList();
	newQueues -> finished = createEmptyProcessList();
	newQueues -> buffer = cJSON_CreateObject();

	return newQueues;
}

int getTotalProcessLeft(Queues* queues, ProcessList* notAlive) {
	return queues -> highPri -> totalProcess +
		queues -> lowPri -> totalProcess +
		queues -> IO -> totalProcess +
		notAlive -> totalProcess;
}

void checkIfAlive(ProcessList* notAlive, 
				  Queues* queues, 
				  unsigned int execTime) {

	ProcessNode* aux = notAlive -> head;
	ProcessNode* temp;

	while(aux) {
		if(aux -> process -> creationTime <= execTime) {
			temp = aux -> next;
			aux -> status = READY;
			transferNodeToList(notAlive,
							   queues -> highPri,
							   aux -> process -> pid);

			writeProcessInfo(queues -> buffer, aux, execTime);
			
			aux = temp;
			continue;
		}
		
		break;
	}	
}

int executeProcess(Queues* queues, unsigned int executionTime) {

	ProcessList* highPri = queues -> highPri;
	ProcessList* lowPri = queues -> lowPri;
	ProcessNode* processToExec;
	bool isHighPri = false, transfered = false;
	int currCpuTime = 0;
	uint16_t pid;

	// Check if there's process to be executed
	if(highPri -> totalProcess) {
		processToExec = highPri -> head;
		isHighPri = true;
	} else if (lowPri -> totalProcess) {
		processToExec = lowPri -> head;
	} else {
		updateIO(queues, executionTime);
		return 1;
	}

	processToExec -> status = RUNNING;
	pid = processToExec -> process -> pid;

	// Write process info after its have been selected to be executed
	writeProcessInfo(queues -> buffer, 
					 processToExec,
					 executionTime);

	// Execution loop for CPU
	for(int i = 0; i < QUANTUM; i++) {
		transfered = transferFinishOrIO(queues, processToExec);

		if(transfered) {
			// Write process info if it reached the end of it's life
			writeProcessInfo(queues -> buffer, 
							 processToExec, 
							 executionTime + currCpuTime);
			
			break;
		}
	    
		// Updates IO queue in real time
		processToExec -> executionTime++;
		currCpuTime++;
		updateIO(queues, executionTime + currCpuTime);	
	}

	// No reason to execute code below
	if(transfered)
		return currCpuTime;

    transfered = transferFinishOrIO(queues, processToExec);

	if(transfered) {
		// Write process info if it reached the end of it's life
		writeProcessInfo(queues -> buffer, 
						 processToExec, 
						 executionTime + currCpuTime);
		
		return currCpuTime;
	}
		
	// If process is high priority, than it's transfered to low priority
	// Else, process is requeued in the lower priority
	if(isHighPri) {
		processToExec -> priority = LOW;
		transferNodeToList(highPri, lowPri, pid);
	} else
		reQueue(lowPri);

	processToExec -> status = READY;

	// Write process info after it's been executed in cpu
	writeProcessInfo(queues -> buffer, 
					 processToExec, 
					 executionTime + currCpuTime);

	return currCpuTime;
}

bool transferFinishOrIO(Queues* queues, 
						ProcessNode* processToExec) {

	ProcessList* highPri = queues -> highPri;
	ProcessList* lowPri = queues -> lowPri;
	ProcessList* finished = queues -> finished;
	ProcessList* IO = queues -> IO;
	bool isHighPri = processToExec -> priority == HIGH;
	uint16_t pid = processToExec -> process -> pid;

	// Checks if process is done executing
	if(checkIfFinish(processToExec)) {

		processToExec -> status = FINISHED;
		   
		if(isHighPri) 
			transferNodeToList(highPri, finished, pid);
		else
			transferNodeToList(lowPri, finished, pid);
		
		return true;
	}
		
	// Checks if process is in time for IO
	if (checkIfIO(processToExec)) {
		if(isHighPri)
			transferToIO(highPri, IO);
		else
			transferToIO(lowPri, IO);
		
		return true;
	}
	
	return false;
}


bool checkIfIO(ProcessNode* processToExec) {

	unsigned int finishedIO = processToExec -> finishedIO;
	unsigned int totalIO = processToExec -> process -> ioList -> totalIO;
	unsigned int* IoTime = processToExec -> process -> ioList -> IoTime;
	unsigned int executionTime = processToExec -> executionTime;

	if(!(finishedIO == totalIO)) {
		if(IoTime[finishedIO] == executionTime)
			return true;
	}
	
	return false;
}

void transferToIO(ProcessList* source, ProcessList* IO) {

	ProcessNode* process = source -> head;
	unsigned int finishedIO = process -> finishedIO;
	IO_types* IoAccess = process -> process -> ioList -> IoAccess;

	process -> status = BLOCKED;
	
	switch(IoAccess[finishedIO]) {
		case DISK:
			process -> timeUntilExec += DISK_ACCESS_TIME;
			break;
			
		case TAPE:
			process -> timeUntilExec += TAPE_ACCESS_TIME;
			break;

		case PRINTER:
			process -> timeUntilExec += PRINTER_ACCESS_TIME;
			break;
	}

	transferNodeToList(source, IO, process -> process -> pid);
}

void updateIO(Queues* queues, unsigned int execTime) {

	ProcessList* IO = queues -> IO;
	ProcessNode* aux = IO -> head;

	while(aux) {

		aux -> timeUntilExec--;

		// Process is done with IO
		if(aux -> timeUntilExec == 0) {
			fromIOToExec(queues, aux -> process -> pid);
			
			// Write process info after it has been transfered from IO queue
			writeProcessInfo(queues -> buffer, 
							 aux, 
							 execTime);
		}	

		aux = aux -> next;
	}
}

void fromIOToExec(Queues* queues, uint16_t pid) {

	ProcessList* highPri = queues -> highPri;
	ProcessList* lowPri = queues -> lowPri;
	ProcessList* IO = queues -> IO;
	ProcessNode* aux = IO -> head;
	IO_types* IoAccess;
	unsigned int finishedIO;

	while(aux) {

		if(aux -> process -> pid == pid) {
			IoAccess= aux -> process -> ioList -> IoAccess;
			finishedIO = aux -> finishedIO;

			aux -> timeUntilExec = 0;
			aux -> status = READY;

			switch(IoAccess[finishedIO]) {
				case PRINTER:
			    case DISK:
					aux -> priority = LOW;
					transferNodeToList(IO, lowPri, aux -> process -> pid);
					break;
				
			    case TAPE:
					aux -> priority = HIGH;
					transferNodeToList(IO, highPri, aux -> process -> pid);
					break;
			}

			aux -> finishedIO++;

			break;
		}

		aux = aux -> next;
	}
}

bool checkIfFinish(ProcessNode* process) {
	return (process -> executionTime == process -> process -> cpuTime) &&
		(process -> finishedIO == process -> process -> ioList -> totalIO);
}

void freeQueues(Queues* queues) {

	freeProcessList(queues -> highPri);
	freeProcessList(queues -> lowPri);
	freeProcessList(queues -> IO);
	freeProcessList(queues -> finished);
	cJSON_Delete(queues -> buffer);
	free(queues);
}
