/*-----------------------------------------------------------------*/
/**

  @file   output.c
  @author Flávio M.
  @brief  Handle file output for all scheduler operations
 */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cJSON.h"
#include "process.h"
#include "error-handler.h"


/*-----------------------------------------------------------------
                             Definitions
  -----------------------------------------------------------------*/
#define PATH_TO_OUTPUT "output/output.json"
#define CALL_FOR_UTIL "python3 utils/visualizer.py "
#define SYSTEM_CALL_FOR_GRAPH CALL_FOR_UTIL PATH_TO_OUTPUT


/*-----------------------------------------------------------------
                  Internal Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Convert IO_types enums elements to a string
   @param  IO_types Enum element to be converted
   @return char*    String
*/
/*-----------------------------------------------------------------*/
char* ioToString(IO_types);


/*-----------------------------------------------------------------*/
/**
   @brief Convert Priority enums elements to a string
   @param  Priority Enum element to be converted
   @return char*    String
*/
/*-----------------------------------------------------------------*/
char* priorityToString(Priority);


/*-----------------------------------------------------------------*/
/**
   @brief Convert Process_status enums elements to a string
   @param  Process_status Enum element to be converted
   @return char*          String
*/
/*-----------------------------------------------------------------*/
char* statusToString(Process_status);


/*-----------------------------------------------------------------*/
/**
   @brief After output file is written calls graph visualization tool
*/
/*-----------------------------------------------------------------*/
void showGraph();


/*-----------------------------------------------------------------
                      Functions Implementation
  -----------------------------------------------------------------*/
void writeToFile(cJSON *info){

	char *jsonString;
	FILE *fp = fopen(PATH_TO_OUTPUT, "w");
	checkNullFilePointer((void*) fp);

	jsonString = cJSON_Print(info);
	fprintf(fp, "%s", jsonString);

	printf("Output File Written!\n");
	
	fclose(fp);
	free(jsonString);

	showGraph();
}

void showGraph() {

	int pid = fork();
	
	if(pid == 0) {

		puts("Loading Graph Visualization...");

		if(setsid() < 0) 
			unexpectedError("Couldn't Create Session for Child Process!");

		if(system(SYSTEM_CALL_FOR_GRAPH) != 0)
			unexpectedError("Graph Visualization Could Not Be Called!");
    
		exit(0);
	}
}

void writeProcessInfo(cJSON** buffer, 
				 ProcessNode* process, 
				 unsigned int execTime) {
	
	cJSON* newBuffer = cJSON_CreateObject();
	cJSON* timeBuffer, *arrayBuffer;
	cJSON* element= NULL, *execTimeItem = NULL;
	cJSON* IoAcessBuffer, *IoTimeBuffer, *temp;
    ProcessIO* ioList = process -> process -> ioList;
    bool alreadyHasTime = false;

	// If buffer is null create main object
	if(!(*buffer)) {
		*buffer = cJSON_CreateObject();
		arrayBuffer = cJSON_CreateArray();
		cJSON_AddItemToObject(*buffer, "events", arrayBuffer);
	}

	// Process Main Info
	cJSON_AddNumberToObject(newBuffer, "pid", process -> process -> pid);
	cJSON_AddNumberToObject(newBuffer, "ppid", process -> process -> ppid);
	cJSON_AddNumberToObject(newBuffer, "cpu_time", process -> process -> cpuTime);
	cJSON_AddNumberToObject(newBuffer, "creation_time", process -> process -> pid);

	// Io Info
	IoAcessBuffer = cJSON_CreateIntArray((int*) ioList -> IoTime, (int) ioList -> totalIO);
	cJSON_AddItemToObject(newBuffer, "io_acess_time", IoAcessBuffer);
	
	IoTimeBuffer = cJSON_CreateArray();
	
	for(int i = 0; i < ioList -> totalIO; i++) {
		temp = cJSON_CreateString(ioToString(ioList -> IoAccess[i]));
		cJSON_AddItemToArray(IoTimeBuffer, temp);
	}
	
	cJSON_AddItemToObject(newBuffer, "io_type", IoTimeBuffer);
	cJSON_AddNumberToObject(newBuffer, "total_io_op", ioList -> totalIO);

	
	// Dynamic Info
	cJSON_AddNumberToObject(newBuffer, "time_until_exec", process -> timeUntilExec);
	cJSON_AddStringToObject(newBuffer, "priority", 
							priorityToString(process -> priority));
	cJSON_AddStringToObject(newBuffer, "status", 
							statusToString(process -> status));
	cJSON_AddNumberToObject(newBuffer, "total_io_finished", 
							process -> finishedIO);
	cJSON_AddNumberToObject(newBuffer, "process_time_executed", 
							process -> executionTime);

	// Check if time already exists in JSON
	if((*buffer) -> child) {
		cJSON_ArrayForEach(element, (*buffer) -> child) {
			execTimeItem = cJSON_GetObjectItem(element, "time_of_execution");

		   if(execTimeItem -> valueint == execTime) {
			   arrayBuffer = cJSON_GetObjectItem(element, "processes");
			   cJSON_AddItemToArray(arrayBuffer, newBuffer);

			   alreadyHasTime = true;
		   }
		}
	}

	// Write to main buffer if it's has not been written
	if(!alreadyHasTime) {
		timeBuffer = cJSON_CreateObject();
		arrayBuffer = cJSON_CreateArray();
		execTimeItem = cJSON_GetObjectItem(*buffer, "events");

		cJSON_AddNumberToObject(timeBuffer, "time_of_execution", execTime);
		cJSON_AddItemToArray(arrayBuffer, newBuffer);
		cJSON_AddItemToObject(timeBuffer, "processes", arrayBuffer);
		cJSON_AddItemToArray(execTimeItem, timeBuffer);
	}
}

char* ioToString(IO_types io) {

	switch(io) {
		case DISK:
			return "disk";
		   
		case TAPE:
			return "tape";

	    case PRINTER:
			return "printer";

	}

	return "null";
}

char* priorityToString(Priority priority) {

	switch(priority) {
		case HIGH:
			return "high";

		case LOW:
			return "low";
	}

	return "null";
}

char* statusToString(Process_status status) {

	switch(status) {

		case RUNNING:
			return "running";

	    case RUNNING_IO:
			return "running_io";

		case BLOCKED:
			return "blocked";

		case FINISHED:
			return "finished";

	    case READY:
			return "ready";

		case NOT_ALIVE:
			return "not_alive";
	}

	return "null";
}

