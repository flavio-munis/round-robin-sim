/*-----------------------------------------------------------------*/
/**

  @file   process.h
  @author Flávio M.

 */
/*-----------------------------------------------------------------*/

#pragma once

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include <stdint.h>


/*-----------------------------------------------------------------
                             Definitions
  -----------------------------------------------------------------*/
#define PROCESS_LIMIT 65535     // Limit of process that can be created
#define DISK_ACCESS_TIME 4      // Disk IO Access Time
#define TAPE_ACCESS_TIME 6      // Tapae IO Access Time
#define PRINTER_ACCESS_TIME 10  // Printer IO Access TIme


/*-----------------------------------------------------------------
                         Structs and Enums
  -----------------------------------------------------------------*/
typedef enum {
	DISK,
	TAPE,
	PRINTER
}IO_types;

typedef enum {
	RUNNING,
	BLOCKED,
	FINISHED,
	READY,
	NOT_ALIVE
}Process_status;

typedef enum {
	HIGH,
	LOW
} Priority;

typedef struct process_io {
	IO_types* IoAccess;       // Array with the type of all IO operations
	unsigned int* IoTime;     // Time of activition relative to process time
	unsigned int totalIO;     // Total IO operations in Array
}ProcessIO;

typedef struct process {
	uint16_t pid;                // Process Identifier
	uint16_t ppid;               // PID of father process
	unsigned int cpuTime;        // Total cpu execution time
	unsigned int creationTime;   // When will process be created
	ProcessIO* ioList;            // List of all process
} Process;

typedef struct process_node {
	Process* process;
	int timeUntilExec;           // Time until the process finish IO operation
	Priority priority;           // Process priority (HIGH or LOW)
	Process_status status;       // Process current status
	unsigned int finishedIO;     // Total IO operations already executed
	unsigned int executionTime;  // Time already executed in CPU
	struct process_node* prev;
	struct process_node* next;
}ProcessNode;

typedef struct process_list {
	ProcessNode* head;
	ProcessNode* tail;
	uint16_t totalProcess;       // Total Process in List
}ProcessList;


/*-----------------------------------------------------------------
                  External Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Creates a new Queue for the given processes. 
          PID's are generated sequentially in the process array order.
		  PPID's are generated randonly.
		  All process nodes are created with HIGH priority and 
		  NOT_ALIVE status.
   @param  Process**    List of process to be added to the queue.
   @param  uint16_t     Number of process in the list. (Max: 65535)
   @return ProcessList* Process Queue.
*/
/*-----------------------------------------------------------------*/
ProcessList* createProcessList(Process**, uint16_t);


/*-----------------------------------------------------------------*/
/**
   @brief Allocate a new process struct 
   @param  unsigned int  Time executing in CPU
   @param  unsigned int  When will process be born
   @param  IO_types*     IO Operations te process will realize
   @param  unsigned int* Time when process will execute IO
   @param  unsigned int  Total number of IO Operations
   @return Process*      New alocated process struct
*/
/*-----------------------------------------------------------------*/
Process* createProcess(unsigned int, 
					   unsigned int, 
					   IO_types*, 
					   unsigned int*,
					   unsigned int);


/*-----------------------------------------------------------------*/
/**
   @brief Allocate a queue.
   @return ProcessList* Empty queue struct
*/
/*-----------------------------------------------------------------*/
ProcessList* createEmptyProcessList();


/*-----------------------------------------------------------------*/
/**
   @brief Add a new Node to the end of the Queue
   @param ProcessList* Current queue.
   @param ProcessNode* Node to be added.
*/
/*-----------------------------------------------------------------*/
void addNodeToList(ProcessList*, ProcessNode*);


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
   @brief Print all information about a queue.
   @param ProcessList* Current queue.
*/
/*-----------------------------------------------------------------*/
void printProccesList(ProcessList*);


/*-----------------------------------------------------------------*/
/**
   @brief Free queue.
   @param ProcessList* Current queue
*/
/*-----------------------------------------------------------------*/
void freeProcessList(ProcessList*);


/*-----------------------------------------------------------------*/
/**
   @brief Free a Process struct
   @param Process* Current Process struct
*/
/*-----------------------------------------------------------------*/
void freeProcess(Process*);
