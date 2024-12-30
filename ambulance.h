/*
 * ambulance.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file ambulance.h
/// @brief Ambulance module interface.

#ifndef INC_AMBULANCE_H_
#define INC_AMBULANCE_H_

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t ambulanceSemaphore;

void initAmbulance(void);
void ambulanceTask(void *params);

#endif /* INC_AMBULANCE_H_ */
