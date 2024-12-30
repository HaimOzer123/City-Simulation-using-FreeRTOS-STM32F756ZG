/*
 * CitySim_main.c
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <CitySim_main.h>

#include "dispatcher.h"
#include "vehicle_management.h"
#include "police.h"
#include "fire.h"
#include "ambulance.h"
#include "corona.h"

#include "logger.h"

#include "project_defines.h"

/// @brief Main function initializes.
int CitySim_main(void)
{
	initVehicleManagement();
	initPolice();
    initFire();
    initAmbulance();
    initCorona();
    initDispatcher();

    vTaskStartScheduler();
    while (1) {
        // Should never reach here
    }
    return 0;
  }
