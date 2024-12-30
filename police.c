/**
 * @file police.c
 * @brief Police department tasks and operations.
 *
 * This file implements the task and initialization logic for the Police department.
 * It includes semaphore-based synchronization for task signaling and completion.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "police.h"
#include "dispatcher.h"
#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Binary semaphore for police task
SemaphoreHandle_t policeSemaphore = NULL;

/**
 * @brief Initializes the Police department task and semaphore.
 *
 * Creates the binary semaphore for task synchronization and the Police task.
 */
void initPolice(void) {
    policeSemaphore = xSemaphoreCreateBinary();
    if (policeSemaphore != NULL) {
        logMessage("Police semaphore initialized successfully\r\n");
    } else {
        logMessage("Failed to initialize Police semaphore\r\n");
        return; // Exit if semaphore creation fails
    }

    if (xTaskCreate(policeTask, "PoliceTask", POLICE_STACK_SIZE, NULL, POLICE_TASK_PRIORITY, NULL) == pdPASS) {
        logMessage("Police task created successfully\r\n");
    } else {
        logMessage("Failed to create Police task\r\n");
    }
}

/**
 * @brief Police department task.
 *
 * Waits for semaphore signals to process events. Upon event handling, signals
 * completion via the `policeCompletionSemaphore`.
 *
 * @param params Unused task parameters.
 */
void policeTask(void *params) {
    while (1) {
        if (xSemaphoreTake(policeSemaphore, portMAX_DELAY) == pdTRUE) {
            // Simulate event handling
            vTaskDelay(Short_DELAY);

            // Signal completion
            xSemaphoreGive(policeCompletionSemaphore);
        }
    }
}
