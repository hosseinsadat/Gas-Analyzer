/*
 * ads_safe.c
 *
 *  Created on: Feb 20, 2026
 *      Author: Manufact
 */



#include "ADS1015_ADS1115.h"
#include "cmsis_os.h"


extern osMutexId_t i2cBusMutexHandle;

uint16_t ADS_SafeReadChannel(ADS1xx5_I2C *i2c, uint8_t channel) {
    uint16_t result = 0;

    if (osMutexAcquire(i2cBusMutexHandle, pdMS_TO_TICKS(100)) == osOK) {
        result = ADSreadADC_SingleEnded(i2c, channel);
        osMutexRelease(i2cBusMutexHandle);
    }

    return result;
}
