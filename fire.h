/*
 * fire.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file fire.h
/// @brief Fire module interface.

#ifndef INC_FIRE_H_
#define INC_FIRE_H_

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t fireSemaphore;

void initFire(void);
void fireTask(void *params);

#endif /* INC_FIRE_H_ */
