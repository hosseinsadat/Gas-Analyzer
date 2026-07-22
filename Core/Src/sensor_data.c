/*
 * sensor_data.c
 *
 *  Created on: Dec 19, 2025
 *      Author: Manufact
 */
#include "sensor_data.h"

__attribute__((section(".dtcmram")))
SensorData_t gSensorData;


volatile int16_t g_co_x10  = 0;
volatile int16_t g_o2_x10  = 0;
volatile uint32_t g_co2_x10 = 0;

volatile int32_t g_warmup_progress = 0;
volatile uint8_t g_sensor_ready    = 0;
