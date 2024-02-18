/*-----------------------------------------------------------------*/
/**

  @file   output.h
  @author Flávio M.
 */
/*-----------------------------------------------------------------*/

#pragma once

/*-----------------------------------------------------------------
                              Includes
  -----------------------------------------------------------------*/
#include "process.h"
#include "cJSON.h"

/*-----------------------------------------------------------------
                  External Functions Declarations
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/**
   @brief Writes current process information to a JSON file
   @param  ProcessList* Inital queue.
   @return ProcessList* Finished queue.
*/
/*-----------------------------------------------------------------*/
void writeToFile(cJSON*);


/*-----------------------------------------------------------------*/
/**
   @brief Writes current process information to a JSON file
   @param  ProcessList* Inital queue.
   @return ProcessList* Finished queue.
*/
/*-----------------------------------------------------------------*/
void writeProcessInfo(cJSON**, ProcessNode*, unsigned int);
