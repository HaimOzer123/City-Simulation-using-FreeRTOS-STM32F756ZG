/**
 * @file vehicle_management.c
 * @brief Vehicle management system for all departments.
 *
 * This file implements functions to manage vehicle allocation, borrowing, and reallocation
 * among different departments such as Police, Fire, Ambulance, and Corona. It also includes
 * synchronization using FreeRTOS mutexes.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "vehicle_management.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "dispatcher.h"
#include "logger.h"

// Static variables to hold the current vehicle counts for each department
static int policeCount = POLICE_COUNT_INITIAL;      /**< Initial vehicle count for Police department */
static int fireCount = FIRE_COUNT_INITIAL;          /**< Initial vehicle count for Fire department */
static int ambulanceCount = AMBULANCE_COUNT_INITIAL;/**< Initial vehicle count for Ambulance department */
static int coronaCount = CORONA_COUNT_INITIAL;      /**< Initial vehicle count for Corona department */

SemaphoreHandle_t vehicleMutex; /**< Mutex for synchronizing access to vehicle counts */

/**
 * @brief Initializes the vehicle management system.
 *
 * This function creates a mutex to ensure thread-safe access to vehicle counts.
 */
void initVehicleManagement(void) {
    vehicleMutex = xSemaphoreCreateMutex();
    if (vehicleMutex != NULL) {
        logMessage("Vehicle management system initialized successfully\r\n");
    } else {
        logMessage("Failed to initialize vehicle management system\r\n");
    }
}

/**
 * @brief Borrows vehicles from one department to another.
 *
 * This function deducts the number of vehicles borrowed from the source department
 * and adds them to the target department, based on availability and need.
 *
 * @param from Pointer to the vehicle count of the source department.
 * @param to Pointer to the vehicle count of the target department.
 * @param needed The number of vehicles needed by the target department.
 * @param fromDept Name of the source department (for logging purposes).
 * @param toDept Name of the target department (for logging purposes).
 * @return The number of vehicles still needed after borrowing.
 */
int borrowVehicles(int *from, int *to, int needed, const char *fromDept, const char *toDept) {
    if (*from > 0 && needed > 0) { // Only borrow if needed
        int borrowed = (*from >= needed) ? needed : *from;
        *from -= borrowed;
        *to += borrowed;
        needed -= borrowed;
        logMessage("Borrowed %d vehicles from %s to %s\r\n", borrowed, fromDept, toDept);
    }
    return needed;
}

/**
 * @brief Reallocates vehicles to fulfill a department's request.
 *
 * This function redistributes vehicles from other departments to the requesting department
 * to fulfill its required vehicles. It prioritizes borrowing from departments with available
 * vehicles.
 *
 * @param request The dispatch request containing the department and required vehicles.
 */
void reallocateVehicles(DispatchRequest request) {
    const char *departmentNames[] = DEPARTMENT_NAMES;
    xSemaphoreTake(vehicleMutex, portMAX_DELAY);

    int needed = request.requiredVehicles; // Vehicles still needed
    if (needed <= 0) {
        logMessage("No reallocation needed for department %s\r\n", departmentNames[request.department]);
        xSemaphoreGive(vehicleMutex);
        return;
    }

    logMessage("Reallocating vehicles for department %s, need %d\r\n", departmentNames[request.department], needed);

    switch (request.department) {
        case POLICE:
            needed = borrowVehicles(&fireCount, &policeCount, needed, "Fire", "Police");
            needed = borrowVehicles(&ambulanceCount, &policeCount, needed, "Ambulance", "Police");
            needed = borrowVehicles(&coronaCount, &policeCount, needed, "Corona", "Police");
            break;

        case FIRE:
            needed = borrowVehicles(&policeCount, &fireCount, needed, "Police", "Fire");
            needed = borrowVehicles(&ambulanceCount, &fireCount, needed, "Ambulance", "Fire");
            needed = borrowVehicles(&coronaCount, &fireCount, needed, "Corona", "Fire");
            break;

        case AMBULANCE:
            needed = borrowVehicles(&policeCount, &ambulanceCount, needed, "Police", "Ambulance");
            needed = borrowVehicles(&fireCount, &ambulanceCount, needed, "Fire", "Ambulance");
            needed = borrowVehicles(&coronaCount, &ambulanceCount, needed, "Corona", "Ambulance");
            break;

        case CORONA:
            needed = borrowVehicles(&policeCount, &coronaCount, needed, "Police", "Corona");
            needed = borrowVehicles(&fireCount, &coronaCount, needed, "Fire", "Corona");
            needed = borrowVehicles(&ambulanceCount, &coronaCount, needed, "Ambulance", "Corona");
            break;

        default:
            logMessage("Invalid department for reallocation\r\n");
            break;
    }

    if (needed > 0) {
        logMessage("Warning: Still need %d vehicles for department %s\r\n", needed, departmentNames[request.department]);
    }

    // Log updated vehicle counts after reallocation
    logMessage("After reallocation - Police:%d | Fire:%d | Ambulance:%d | Corona:%d\r\n",
               policeCount, fireCount, ambulanceCount, coronaCount);

    xSemaphoreGive(vehicleMutex);
}

/**
 * @brief Gets the current vehicle count for a department.
 *
 * This function retrieves the current number of vehicles available in the specified department.
 *
 * @param department The department index (POLICE, FIRE, etc.).
 * @return The current number of vehicles available in the department.
 */
int getVehicleCount(uint8_t department) {
    xSemaphoreTake(vehicleMutex, portMAX_DELAY);
    int count = 0;

    switch (department) {
        case POLICE:
            count = policeCount;
            break;
        case FIRE:
            count = fireCount;
            break;
        case AMBULANCE:
            count = ambulanceCount;
            break;
        case CORONA:
            count = coronaCount;
            break;
        default:
            logMessage("Invalid department index for vehicle count\r\n");
            break;
    }

    xSemaphoreGive(vehicleMutex);
    return count;
}
