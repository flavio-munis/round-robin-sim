/*-----------------------------------------------------------------*/
/**

  @file   scheduler.h
  @author Flávio M. and Eduardo Barros
 */
/*-----------------------------------------------------------------*/

#pragma once

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include "cJSON.h"
#include "process.h"


/*-----------------------------------------------------------------
                             Definitions
  -----------------------------------------------------------------*/
#define QUANTUM 2


/*-----------------------------------------------------------------
                         Structs and Enums
  -----------------------------------------------------------------*/
typedef struct queues {
	ProcessList* highPri;
	ProcessList* lowPri;
	ProcessList* IO;
	ProcessList* finished;
	cJSON* buffer;
}Queues;


/*-----------------------------------------------------------------
                  External Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Executes round robin scheduling algorithm in a queue. 
   @param  ProcessList* Inital queue.
   @return ProcessList* Finished queue.
*/
/*-----------------------------------------------------------------*/
void roundRobinAlgo(ProcessList*);
