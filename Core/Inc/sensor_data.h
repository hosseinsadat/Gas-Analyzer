/*
 * sensor_data.h
 *
 *  Created on: Dec 19, 2025
 *      Author: Manufact
 */

#ifndef INC_SENSOR_DATA_H_
#define INC_SENSOR_DATA_H_

#include "cmsis_os.h"




typedef struct {

	float temperature;
	float humidity;

	int16_t rawCO;
	int16_t rawO2;

	float co_ppm;
	float o2_percent;

	float co2_p;
	uint16_t co2_ppm;

	float probe_temp;


} SensorData_t ;

typedef struct {
    float baselineVoltage;
    float rtia;
    float sensitivity; // nA/ppm
} GasSensorCalib_t;

extern SensorData_t gSensorData;

extern osMutexId_t i2c2MutexHandle;
extern osMutexId_t sensorDataMutexHandle;


extern volatile int16_t g_co_x10;
extern volatile int16_t g_o2_x10;
extern volatile uint32_t g_co2_x10;


extern volatile int32_t g_warmup_progress;  // 0 تا 100
extern volatile uint8_t g_sensor_ready;     // 1 = آماده


#endif /* INC_SENSOR_DATA_H_ */
