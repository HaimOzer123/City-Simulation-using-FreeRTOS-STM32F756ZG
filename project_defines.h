/*
 * project_defines.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */
/// @file project_defines.h
/// @brief Defines for project-wide configurations.

#ifndef INC_PROJECT_DEFINES_H_
#define INC_PROJECT_DEFINES_H_

#include <stdint.h>

// Task priorities
#define POLICE_TASK_PRIORITY 3
#define FIRE_TASK_PRIORITY 3
#define AMBULANCE_TASK_PRIORITY 3
#define CORONA_TASK_PRIORITY 3
#define RANDOM_EVENT_PRIORITY 5

// Task stack sizes
#define POLICE_STACK_SIZE 256
#define FIRE_STACK_SIZE 256
#define AMBULANCE_STACK_SIZE 256
#define CORONA_STACK_SIZE 256
#define LOGGER_STACK_SIZE 256
#define RANDOM_EVENT_STACK_SIZE 256

// Vehicle counts
#define POLICE_COUNT_INITIAL 3
#define FIRE_COUNT_INITIAL 2
#define AMBULANCE_COUNT_INITIAL 4
#define CORONA_COUNT_INITIAL 2

// Logger defines
#define LOGGER_QUEUE_LENGTH 10
#define LOGGER_MESSAGE_SIZE 128

// General defines
#define MAX_CARS 11
#define NULL_PARAM NULL
#define DEFAULT_DELAY pdMS_TO_TICKS(1000)
#define Short_DELAY pdMS_TO_TICKS(500)

// Department names for logging
#define DEPARTMENT_NAMES {"Police", "Fire", "Ambulance", "Corona"}

#endif /* INC_PROJECT_DEFINES_H_ */
