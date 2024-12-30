/**
 * @file corona.c
 * @brief Corona department tasks and operations.
 *
 * This file implements the task and initialization logic for the Corona department.
 * It includes semaphore-based synchronization for task signaling and completion.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "corona.h"
#include "dispatcher.h"
#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Binary semaphore for corona task
SemaphoreHandle_t coronaSemaphore = NULL;

/**
 * @brief Initializes the Corona department task and semaphore.
 *
 * Creates the binary semaphore for task synchronization and the Corona task.
 */
void initCorona(void) {
    coronaSemaphore = xSemaphoreCreateBinary();
    if (coronaSemaphore != NULL) {
        logMessage("Corona semaphore initialized successfully\r\n");
    } else {
        logMessage("Failed to initialize Corona semaphore\r\n");
        return;
    }

    if (xTaskCreate(coronaTask, "CoronaTask", CORONA_STACK_SIZE, NULL, CORONA_TASK_PRIORITY, NULL) == pdPASS) {
        logMessage("Corona task created successfully\r\n");
    } else {
        logMessage("Failed to create Corona task\r\n");
    }
}

/**
 * @brief Corona department task.
 *
 * Waits for semaphore signals to process events. Upon event handling, signals
 * completion via the `coronaCompletionSemaphore`.
 *
 * @param params Unused task parameters.
 */
void coronaTask(void *params) {
    while (1) {
        if (xSemaphoreTake(coronaSemaphore, portMAX_DELAY) == pdTRUE) {
            // Simulate event handling
            vTaskDelay(Short_DELAY);

            // Signal completion
            xSemaphoreGive(coronaCompletionSemaphore);
        }
    }
}
