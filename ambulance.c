/**
 * @file ambulance.c
 * @brief Ambulance department tasks and operations.
 *
 * This file implements the task and initialization logic for the Ambulance department.
 * It includes semaphore-based synchronization for task signaling and completion.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "ambulance.h"
#include "dispatcher.h"
#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Binary semaphore for ambulance task
SemaphoreHandle_t ambulanceSemaphore = NULL;

/**
 * @brief Initializes the Ambulance department task and semaphore.
 *
 * Creates the binary semaphore for task synchronization and the Ambulance task.
 */
void initAmbulance(void) {
    ambulanceSemaphore = xSemaphoreCreateBinary();
    if (ambulanceSemaphore != NULL) {
        logMessage("Ambulance semaphore initialized successfully\r\n");
    } else {
        logMessage("Failed to initialize Ambulance semaphore\r\n");
        return;
    }

    if (xTaskCreate(ambulanceTask, "AmbulanceTask", AMBULANCE_STACK_SIZE, NULL, AMBULANCE_TASK_PRIORITY, NULL) == pdPASS) {
        logMessage("Ambulance task created successfully\r\n");
    } else {
        logMessage("Failed to create Ambulance task\r\n");
    }
}

/**
 * @brief Ambulance department task.
 *
 * Waits for semaphore signals to process events. Upon event handling, signals
 * completion via the `ambulanceCompletionSemaphore`.
 *
 * @param params Unused task parameters.
 */
void ambulanceTask(void *params) {
    while (1) {
        if (xSemaphoreTake(ambulanceSemaphore, portMAX_DELAY) == pdTRUE) {
            // Simulate event handling
            vTaskDelay(Short_DELAY);

            // Signal completion
            xSemaphoreGive(ambulanceCompletionSemaphore);
        }
    }
}
