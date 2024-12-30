/*
 * vehicle_management.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

/// @file vehicle_management.h
/// @brief Vehicle management interface.

#ifndef INC_VEHICLE_MANAGEMENT_H_
#define INC_VEHICLE_MANAGEMENT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "dispatcher.h"

enum {
    POLICE = 0,
    FIRE,
    AMBULANCE,
    CORONA
};

void initVehicleManagement(void);
int borrowVehicles(int *from, int *to, int needed, const char *fromDept, const char *toDept);
void reallocateVehicles(DispatchRequest request);
int getVehicleCount(uint8_t department);

#endif /* INC_VEHICLE_MANAGEMENT_H_ */
