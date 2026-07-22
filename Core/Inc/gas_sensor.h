/*
 * gas_sensor.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Manufact
 */

#ifndef INC_GAS_SENSOR_H_
#define INC_GAS_SENSOR_H_




#include <stdint.h>

typedef struct
{
    float co_ppm;
    float o2_percent;
    float temperature;
    float humidity;
} GasSensorData_t;

void GasSensor_Init(void);
void GasSensor_Update(void);
GasSensorData_t GasSensor_GetData(void);

/* Calibration triggers */
void GasSensor_RequestCOZero(void);
void GasSensor_RequestCOSpan(float known_ppm);
void GasSensor_RequestO2Zero(void);
uint8_t O2_WarmupProgress(float o2_pct);


#endif /* INC_GAS_SENSOR_H_ */
