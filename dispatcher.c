/**
 * @file dispatcher.c
 * @brief Implements dispatcher logic for managing random events and allocating resources.
 *
 * This file contains the initialization of dispatcher resources, vehicle management,
 * and task communication for handling random events across various departments such
 * as Police, Fire, Ambulance, and Corona.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "dispatcher.h"
#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "vehicle_management.h"
#include "logger.h"
#include "police.h"
#include "fire.h"
#include "ambulance.h"
#include "corona.h"
#include <stdlib.h>
#include <time.h>

// Global variables for dispatcher
QueueHandle_t dispatchQueue;                     /**< Queue for dispatch requests */
SemaphoreHandle_t dispatchSemaphore;            /**< Semaphore for dispatcher resource access */
SemaphoreHandle_t policeCompletionSemaphore;    /**< Semaphore for Police task completion */
SemaphoreHandle_t fireCompletionSemaphore;      /**< Semaphore for Fire task completion */
SemaphoreHandle_t ambulanceCompletionSemaphore; /**< Semaphore for Ambulance task completion */
SemaphoreHandle_t coronaCompletionSemaphore;    /**< Semaphore for Corona task completion */

/**
 * @brief Initializes dispatcher resources, including semaphores and tasks.
 *
 * This function creates the necessary semaphores and initializes the task for handling
 * random events. It also initializes a queue for dispatch requests.
 */
void initDispatcher(void) {
    srand(time(NULL)); // Seed the random number generator

    dispatchQueue = xQueueCreate(10, sizeof(DispatchRequest));
    dispatchSemaphore = xSemaphoreCreateBinary();

    policeCompletionSemaphore = xSemaphoreCreateBinary();
    fireCompletionSemaphore = xSemaphoreCreateBinary();
    ambulanceCompletionSemaphore = xSemaphoreCreateBinary();
    coronaCompletionSemaphore = xSemaphoreCreateBinary();

    if (dispatchQueue != NULL && dispatchSemaphore != NULL &&
        policeCompletionSemaphore != NULL && fireCompletionSemaphore != NULL &&
        ambulanceCompletionSemaphore != NULL && coronaCompletionSemaphore != NULL) {
        logMessage("Dispatcher resources initialized successfully\r\n");
    } else {
        logMessage("Dispatcher resource initialization failed\r\n");
    }

    xTaskCreate(randomEventAndDispatchTask, "RandomEvent", RANDOM_EVENT_STACK_SIZE, NULL_PARAM, RANDOM_EVENT_PRIORITY, NULL_PARAM);
    xSemaphoreGive(dispatchSemaphore);
}

/**
 * @brief Checks if a department has enough vehicles. If not, triggers allocation/reallocation.
 *
 * This function determines if a department has sufficient vehicles to handle a request.
 * If not, it reallocates resources from other departments as needed.
 *
 * @param department The department index (POLICE, FIRE, etc.).
 * @param requiredVehicles The number of vehicles required by the department.
 * @return True if the department has enough vehicles after allocation, False otherwise.
 */
bool checkAndAllocateVehicles(uint8_t department, int requiredVehicles) {
    const char *departmentNames[] = DEPARTMENT_NAMES;

    while (1) {
        xSemaphoreTake(dispatchSemaphore, portMAX_DELAY); // Protect vehicle checks and allocation
        int currentVehicles = getVehicleCount(department);

        if (currentVehicles >= requiredVehicles) {
            logMessage("%s department has enough vehicles to process the task\r\n",
                       departmentNames[department]);
            xSemaphoreGive(dispatchSemaphore);
            return true; // Enough vehicles available, task can proceed
        } else {
            int neededToBorrow = requiredVehicles - currentVehicles;

            logMessage("%s department needs %d more vehicles to fulfill the request\r\n",
                       departmentNames[department], neededToBorrow);

            DispatchRequest adjustedRequest = {.department = department, .requiredVehicles = neededToBorrow};

            if (neededToBorrow > 0) {
                logMessage("Insufficient vehicles for %s, reallocating\r\n",
                           departmentNames[department]);
                reallocateVehicles(adjustedRequest);
            }
        }
        xSemaphoreGive(dispatchSemaphore);

        // Recheck the count after allocation/reallocation
        currentVehicles = getVehicleCount(department);
        if (currentVehicles >= requiredVehicles) {
            logMessage("%s department now has enough vehicles to process the task\r\n",
                       departmentNames[department]);
            return true;
        }
    }
}

/**
 * @brief Handles random events and dispatches tasks to appropriate departments.
 *
 * This task generates random events for departments, checks resource availability,
 * and signals the corresponding department task to handle the event.
 *
 * @param params Task parameters (unused).
 */
void randomEventAndDispatchTask(void *params) {
    const char *departmentNames[] = DEPARTMENT_NAMES;

    while (1) {
        DispatchRequest request;
        request.department = rand() % 4;  // Random department
        request.requiredVehicles = (rand() % MAX_CARS) + 1;  // Random vehicles (1-7)

        logMessage("Random event for department %s requesting %d vehicles\r\n",
                   departmentNames[request.department], request.requiredVehicles);

        logMessage("Current vehicle counts - Police:%d | Fire:%d | Ambulance:%d | Corona:%d\r\n",
                   getVehicleCount(POLICE), getVehicleCount(FIRE),
                   getVehicleCount(AMBULANCE), getVehicleCount(CORONA));

        if (checkAndAllocateVehicles(request.department, request.requiredVehicles)) {
            // Signal the corresponding department
            switch (request.department) {
                case POLICE:
                    if (policeSemaphore != NULL) {
                        xSemaphoreGive(policeSemaphore);
                        logMessage("Police notification given\r\n");
                        xSemaphoreTake(policeCompletionSemaphore, portMAX_DELAY);
                        logMessage("\n*******************Police task completed*******************\r\n");
                    }
                    break;

                case FIRE:
                    if (fireSemaphore != NULL) {
                        xSemaphoreGive(fireSemaphore);
                        logMessage("Fire notification given\r\n");
                        xSemaphoreTake(fireCompletionSemaphore, portMAX_DELAY);
                        logMessage("\n*******************Fire task completed*******************\r\n");
                    }
                    break;

                case AMBULANCE:
                    if (ambulanceSemaphore != NULL) {
                        xSemaphoreGive(ambulanceSemaphore);
                        logMessage("Ambulance notification given\r\n");
                        xSemaphoreTake(ambulanceCompletionSemaphore, portMAX_DELAY);
                        logMessage("\n*******************Ambulance task completed*******************\r\n");
                    }
                    break;

                case CORONA:
                    if (coronaSemaphore != NULL) {
                        xSemaphoreGive(coronaSemaphore);
                        logMessage("Corona notification given\r\n");
                        xSemaphoreTake(coronaCompletionSemaphore, portMAX_DELAY);
                        logMessage("\n*******************Corona task completed*******************\r\n");
                    }
                    break;

                default:
                    logMessage("Invalid department generated\r\n");
                    break;
            }
        }

        // Delay before generating the next random event
        vTaskDelay(Short_DELAY);
    }
}
