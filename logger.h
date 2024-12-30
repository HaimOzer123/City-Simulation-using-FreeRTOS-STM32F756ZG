/*
 * logger.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Haim
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

void logMessage(const char *format, ...);
void recordTaskExecution(int department);
void recordVehicleUsage(int department, int count);
void generateStatisticsReport(void);

#endif /* INC_LOGGER_H_ */
