/**
 ******************************************************************************
 * @file    gas_calibration.h
 * @brief   Header file for gas sensor calibration using slope and offset.
 ******************************************************************************
 */
#ifndef INC_GAS_CALIBRATION_H_
#define INC_GAS_CALIBRATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "slope_offset_calib.h"

/**
 * @brief  Initialize the calibration parameters by trying to load from EEPROM.
 *         If EEPROM is empty or checksum is invalid, reset to default parameters.
 */
void GasCalibration_Init(void);

/**
 * @brief  Calculate O2 percentage from raw ADC value using O2 calibration data.
 * @param  adc_raw Raw ADC reading.
 * @retval Calibrated O2 percentage (0.0% to 25.0%).
 */
float O2_CalcPercent(int16_t adc_raw);

/**
 * @brief  Calculate CO PPM from raw ADC value using CO calibration data.
 * @param  adc_raw Raw ADC reading.
 * @retval Calibrated CO PPM (0.0 to 2000.0).
 */
float CO_CalcPPM(int16_t adc_raw);

/**
 * @brief  Calculate CO2 percentage from raw CO2 register reading using CO2 calibration data.
 * @param  raw_co2 Raw K33 CO2 reading.
 * @retval Calibrated CO2 percentage.
 */
float CO2_CalcPercent(uint16_t raw_co2);

#ifdef __cplusplus
}
#endif

#endif /* INC_GAS_CALIBRATION_H_ */
