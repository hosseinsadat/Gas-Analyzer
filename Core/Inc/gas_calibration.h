/*
 * gas_calibration.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Manufact
 */

#ifndef INC_GAS_CALIBRATION_H_
#define INC_GAS_CALIBRATION_H_

#include <stdint.h>



#define VREF_ADC        3.3f
#define ADC_FULL_SCALE  65535.0f

/* LMP91000 configuration */
#define CO_VREF         1.65f     // mid-supply
#define CO_RTIA         100000.0f // 100kΩ example
#define CO_PPM_SCALE    1.0f      // adjust experimentally

#define O2_VREF         1.65f
#define O2_RTIA         100000.0f
#define O2_PCT_SCALE    1.0f


typedef struct
{
    float zero_uA;
    float sensitivity;   // uA per ppm or uA per %O2
} GasCalibration_t;

/* === CO === */
void CO_Calibration_StartZero(void);
float CO_Calibration_ComputeZero(int16_t raw);
float CO_Calibration_ComputeSpan(int16_t raw, float known_ppm);
float CO_ConvertRawToPPM(int16_t raw);

/* === O2 === */
void O2_Calibration_StartZero(void);
float O2_Calibration_ComputeZero(int16_t raw);
float O2_Calibration_ComputeSpan(int16_t raw);
float O2_ConvertRawToPercent(int16_t raw);

/* Load / Save */
void GasCalibration_Load(void);
void GasCalibration_Save(void);

float O2_CalcPercent(int16_t adc_raw);
float CO_CalcPPM(int16_t adc_raw);


long map(long x, long in_min, long in_max, long out_min, long out_max) ;


#endif /* INC_GAS_CALIBRATION_H_ */
