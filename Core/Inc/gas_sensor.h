/**
 ******************************************************************************
 * @file    gas_sensor.h
 * @brief   Header file for gas sensor data handling and processing task.
 ******************************************************************************
 */
#ifndef INC_GAS_SENSOR_H_
#define INC_GAS_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    float co_ppm;
    float o2_percent;
    float temperature;
    float humidity;
} GasSensorData_t;

/**
 * @brief  Initialize the gas sensor module.
 */
void GasSensor_Init(void);

/**
 * @brief  Update the gas sensor values.
 */
void GasSensor_Update(void);

/**
 * @brief  Retrieve the latest gas sensor data.
 * @retval Latest GasSensorData_t structure.
 */
GasSensorData_t GasSensor_GetData(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_GAS_SENSOR_H_ */
