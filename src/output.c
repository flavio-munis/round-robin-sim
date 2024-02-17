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
#include "process.h"
#include "error-handler.h"

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


/*-----------------------------------------------------------------
                      Functions Implementation
  -----------------------------------------------------------------*/
void writeToFile(cJSON *info){

	char *jsonString;
	FILE *fp = fopen("./output/output.json", "w");
	checkNullPointer((void*) fp);

	jsonString = cJSON_Print(info);
	fprintf(fp, "%s", jsonString);

	printf("Output File Written!\n");
	
	fclose(fp);
	free(jsonString);
}

void writeProcessInfo(cJSON* buffer, 
				 ProcessNode* process, 
				 unsigned int execTime) {
	
	cJSON* newBuffer = cJSON_CreateObject();
	cJSON* IoAcessBuffer, *IoTimeBuffer, *temp;
    ProcessIO* ioList = process -> process -> ioList;

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
	cJSON_AddNumberToObject(newBuffer, "total_time_executed", execTime);

	// Write to main buffer
	cJSON_AddItemToObject(buffer, "process:", newBuffer);
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
