# City Simulation Project
# Haim Ozer - 316063569 - RTED - "R.T Concepts" course
This project implements a city simulation system using **FreeRTOS** on an **STM32F756ZG** development board. The simulation models the dispatching of resources to various departments like Police, Fire, Ambulance, and Corona response.

## Overview
The simulation handles the following:
- Randomly generated dispatch requests for each department.
- Resource allocation and management for vehicles.
- Synchronization between tasks using semaphores.
- Performance tracking and logging of system activity.

The system is modular, ensuring each department operates independently while coordinating through shared resources and communication.

## Features
- **FreeRTOS-based task management.**
- **Resource synchronization using semaphores and mutexes.**
- **Dynamic resource reallocation across departments.**
- **Performance statistics logging.**

## System Components
### Task List
- **Dispatcher Task:** Handles event generation and dispatching to departments.
- **Department Tasks:** Separate tasks for Police, Fire, Ambulance, and Corona, each processing dispatch requests.
- **Logger Task:** Logs messages and statistics for system performance monitoring.

### Synchronization Mechanisms
#### Binary Semaphores
Used for task signaling and completion synchronization.
- Each department has a dedicated binary semaphore to signal task readiness (e.g., `policeSemaphore`, `fireSemaphore`).
- Completion semaphores (e.g., `policeCompletionSemaphore`) signal when a task has finished processing an event.

#### Mutex
Used for protecting shared resources.
- The `vehicleMutex` ensures synchronized access to vehicle counts across departments to avoid race conditions.

#### Queues
- The `dispatchQueue` is used for managing dispatch requests.

## Usage of FreeRTOS Components
### Dispatcher Task
- **Purpose:** Generates random dispatch requests and checks resource availability for each department.
- **Synchronization:**
  - Uses `dispatchSemaphore` to ensure exclusive access to resource allocation.
  - Signals department tasks using their respective semaphores after ensuring resource availability.

### Department Tasks
- **Purpose:** Process dispatch requests for their respective departments.
- **Synchronization:**
  - Wait for binary semaphore signals (e.g., `xSemaphoreTake(policeSemaphore)`).
  - Signal completion using completion semaphores (e.g., `xSemaphoreGive(policeCompletionSemaphore)`).

### Vehicle Management
- **Purpose:** Manages vehicle allocation, borrowing, and reallocation among departments.
- **Synchronization:**
  - `vehicleMutex` ensures thread-safe access to vehicle counts.
  - Implements borrowing logic to fulfill requests when a department lacks sufficient vehicles.

### Logger Task
- **Purpose:** Logs messages for system events and generates performance reports.
- **Key Functions:**
  - `logMessage`: Logs formatted messages.
  - `recordTaskExecution`: Tracks task execution counts per department.
  - `recordVehicleUsage`: Tracks the number of vehicles used per department.
  - `generateStatisticsReport`: Outputs task execution and vehicle usage statistics.

## Setup and Initialization
1. **Initialize Resources:**
   - `initVehicleManagement()` creates the vehicle mutex.
   - `initDispatcher()` creates semaphores and initializes the dispatcher task.
   - Each department's initialization function creates the task and semaphore for the department.

2. **Start Scheduler:**
   - Call `vTaskStartScheduler()` to start the FreeRTOS scheduler.

## Code Highlights
### Dispatcher Initialization
```c
void initDispatcher(void) {
    dispatchQueue = xQueueCreate(10, sizeof(DispatchRequest));
    dispatchSemaphore = xSemaphoreCreateBinary();
    policeCompletionSemaphore = xSemaphoreCreateBinary();
    // ... initialize other semaphores

    xTaskCreate(randomEventAndDispatchTask, "RandomEvent", RANDOM_EVENT_STACK_SIZE, NULL, RANDOM_EVENT_PRIORITY, NULL);
}
```

### Resource Allocation
```c
bool checkAndAllocateVehicles(uint8_t department, int requiredVehicles) {
    xSemaphoreTake(vehicleMutex, portMAX_DELAY);
    int currentVehicles = getVehicleCount(department);
    if (currentVehicles >= requiredVehicles) {
        xSemaphoreGive(vehicleMutex);
        return true;
    }
    reallocateVehicles((DispatchRequest){.department = department, .requiredVehicles = requiredVehicles});
    xSemaphoreGive(vehicleMutex);
    return false;
}
```

### Department Task Logic
```c
void policeTask(void *params) {
    while (1) {
        if (xSemaphoreTake(policeSemaphore, portMAX_DELAY) == pdTRUE) {
            // Process the event
            vTaskDelay(PROCESS_DELAY);
            xSemaphoreGive(policeCompletionSemaphore);
        }
    }
}
```

## Performance Monitoring
- Execution counts and vehicle usage are tracked per department.
- Statistics are logged periodically using the `generateStatisticsReport` function.

## Hardware and Dependencies
- **STM32F756ZG Development Board.**
- **FreeRTOS**: Real-time operating system for task management.
- **GCC ARM Compiler**: For compiling the project.

## How to Run
1. Build and flash the project to the STM32F756ZG board.
2. Observe log outputs through UART for system behavior and performance statistics.
3. Monitor vehicle allocation and task processing via the log messages.

## Future Enhancements
- Dynamic priority adjustment for tasks based on resource availability.
- Implementing fault-tolerant mechanisms for task failures.
- Real-time visualization of system statistics.

---
**Author:** Haim  
**Date:** December 29, 2024

