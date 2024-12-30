/**
 * @file logger.c
 * @brief Logger implementation with performance analysis.
 *
 * This file implements logging functionalities and performance analysis for tasks.
 * It tracks task execution counts, vehicle usage, and generates statistical reports
 * for all departments.
 *
 * @date Dec 29, 2024
 * @author Haim
 */

#include "logger.h"
#include "project_defines.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Static variables for performance tracking
static int taskExecutionCounts[4] = {0}; /**< Count of task executions for departments */
static int totalVehiclesUsed[4] = {0};   /**< Total vehicles dispatched for departments */

/**
 * @brief Logs a formatted message.
 *
 * This function logs a formatted message to the console. The message format
 * follows `printf`-style formatting.
 *
 * @param format The format string (similar to `printf`).
 * @param ... The arguments corresponding to the format specifiers.
 */
void logMessage(const char *format, ...) {
    char message[LOGGER_MESSAGE_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(message, LOGGER_MESSAGE_SIZE, format, args);
    va_end(args);

    printf("%s", message);
}

/**
 * @brief Records task execution for a department.
 *
 * This function increments the task execution count for a specified department
 * and logs the updated total.
 *
 * @param department The department index (POLICE, FIRE, etc.).
 */
void recordTaskExecution(int department) {
    taskExecutionCounts[department]++;
    logMessage("Task executed for department %d. Total: %d\n", department, taskExecutionCounts[department]);
}

/**
 * @brief Records vehicle usage for a department.
 *
 * This function increments the total vehicles used count for a specified department
 * by the given count and logs the updated total.
 *
 * @param department The department index (POLICE, FIRE, etc.).
 * @param count The number of vehicles used for the task.
 */
void recordVehicleUsage(int department, int count) {
    totalVehiclesUsed[department] += count;
    logMessage("Vehicles used for department %d: %d. Total: %d\n", department, count, totalVehiclesUsed[department]);
}

/**
 * @brief Generates a statistics report for all departments.
 *
 * This function logs the total task execution counts and vehicle usage for each department.
 * It provides an overview of the system's performance and resource utilization.
 */
void generateStatisticsReport(void) {
    logMessage("Generating statistics report:\n");
    for (int i = 0; i < 4; i++) {
        logMessage("Department %d: Tasks executed: %d, Total vehicles used: %d\n",
                   i, taskExecutionCounts[i], totalVehiclesUsed[i]);
    }
}
