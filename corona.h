/*
 * corona.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file corona.h
/// @brief Corona module interface.

#ifndef INC_CORONA_H_
#define INC_CORONA_H_

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t coronaSemaphore;

void initCorona(void);
void coronaTask(void *params);

#endif /* INC_CORONA_H_ */
