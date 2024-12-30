/**
 * @file fire.c
 * @brief Fire department tasks and operations.
 *
 * This file implements the task and initialization logic for the Fire department.
 * It includes semaphore-based synchronization for task signaling and completion.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "fire.h"
#include "dispatcher.h"
#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Binary semaphore for fire task
SemaphoreHandle_t fireSemaphore = NULL;

/**
 * @brief Initializes the Fire department task and semaphore.
 *
 * Creates the binary semaphore for task synchronization and the Fire task.
 */
void initFire(void) {
    fireSemaphore = xSemaphoreCreateBinary();
    if (fireSemaphore != NULL) {
        logMessage("Fire semaphore initialized successfully\r\n");
    } else {
        logMessage("Failed to initialize Fire semaphore\r\n");
        return;
    }

    if (xTaskCreate(fireTask, "FireTask", FIRE_STACK_SIZE, NULL, FIRE_TASK_PRIORITY, NULL) == pdPASS) {
        logMessage("Fire task created successfully\r\n");
    } else {
        logMessage("Failed to create Fire task\r\n");
    }
}

/**
 * @brief Fire department task.
 *
 * Waits for semaphore signals to process events. Upon event handling, signals
 * completion via the `fireCompletionSemaphore`.
 *
 * @param params Unused task parameters.
 */
void fireTask(void *params) {
    while (1) {
        if (xSemaphoreTake(fireSemaphore, portMAX_DELAY) == pdTRUE) {
            // Simulate event handling
            vTaskDelay(Short_DELAY);

            // Signal completion
            xSemaphoreGive(fireCompletionSemaphore);
        }
    }
}
