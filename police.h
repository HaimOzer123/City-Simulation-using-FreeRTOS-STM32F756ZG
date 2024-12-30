/*
 * police.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file police.h
/// @brief Police module interface.

#ifndef INC_POLICE_H_
#define INC_POLICE_H_

#include "FreeRTOS.h"
#include "semphr.h"

// Declare the semaphore for the police task
extern SemaphoreHandle_t policeSemaphore;

// Function declarations
void initPolice(void);
void policeTask(void *params);

#endif /* INC_POLICE_H_ */
