/*
 * dispatcher.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file dispatcher.h
/// @brief Dispatcher module interface.

#ifndef INC_DISPATCHER_H_
#define INC_DISPATCHER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern SemaphoreHandle_t policeCompletionSemaphore;
extern SemaphoreHandle_t fireCompletionSemaphore;
extern SemaphoreHandle_t ambulanceCompletionSemaphore;
extern SemaphoreHandle_t coronaCompletionSemaphore;

typedef struct {
    uint8_t department;
    uint8_t requiredVehicles;
} DispatchRequest;

void initDispatcher(void);
bool checkAndAllocateVehicles(uint8_t department, int requiredVehicles);
void randomEventAndDispatchTask(void *params);

#endif /* INC_DISPATCHER_H_ */
