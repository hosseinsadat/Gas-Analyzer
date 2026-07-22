/*
 * gas_calibration.c
 *
 *  Created on: Dec 20, 2025
 *      Author: Manufact
 */




#include "gas_calibration.h"
#include  "config.h"


/* O2 Sensor — مثال: سنسور الکتروشیمیایی 0-25% */
#define O2_ADC_AT_LOW      10710   // مقدار ADC در 18.0%
#define O2_ADC_AT_HIGH     10190    // مقدار ADC در 20.9%
#define O2_PCT_LOW         15.0f
#define O2_PCT_HIGH        20.9f

/* CO Sensor — مثال: سنسور 0-1000 ppm */
#define CO_ADC_AT_ZERO     500      // مقدار ADC در 0 ppm (هوای تازه)
#define CO_ADC_AT_FULL     32000    // مقدار ADC در 1000 ppm
#define CO_PPM_MIN         0.0f
#define CO_PPM_MAX         1000.0f

/* ===== USER CONSTANTS ===== */
#define ADS_PGA_VOLTAGE     4.096f
#define CO_RTIA_OHMS        35000.0f
#define O2_RTIA_OHMS        120000.0f
#define CO_INTERNAL_ZERO_V  1.65f



static GasCalibration_t coCalib = {0};
static GasCalibration_t o2Calib = {0};

/* ===== INTERNAL ===== */


static inline float ADC_ToVoltage(uint16_t raw)
{
    return ((float)raw * VREF_ADC) / ADC_FULL_SCALE;
}

float CO_ConvertRawToPPM(int16_t raw)
{
    float v = ADC_ToVoltage(raw);
    float current_uA = (v - CO_VREF) / CO_RTIA * 1e6f;
    return current_uA * CO_PPM_SCALE;
}

/* ===== CO ===== */

float CO_Calibration_ComputeZero(int16_t raw)
{
    float v = ADC_ToVoltage(raw);
    float i_uA = ((v - CO_INTERNAL_ZERO_V) / CO_RTIA_OHMS) * 1e6f;
    coCalib.zero_uA = i_uA;
    return i_uA;
}

float CO_Calibration_ComputeSpan(int16_t raw, float known_ppm)
{
    float v = ADC_ToVoltage(raw);
    float i_uA = ((v - CO_INTERNAL_ZERO_V) / CO_RTIA_OHMS) * 1e6f;
    coCalib.sensitivity = (i_uA - coCalib.zero_uA) / known_ppm;
    return coCalib.sensitivity;
}



/* ===== O2 ===== */

float O2_Calibration_ComputeZero(int16_t raw)
{
    float v = ADC_ToVoltage(raw);
    float i_uA = (v / O2_RTIA_OHMS) * 1e6f;
    o2Calib.zero_uA = i_uA;
    return i_uA;
}

float O2_Calibration_ComputeSpan(int16_t raw)
{
    float v = ADC_ToVoltage(raw);
    float i_uA = (v / O2_RTIA_OHMS) * 1e6f;
    o2Calib.sensitivity = (i_uA - o2Calib.zero_uA) / 20.9f;
    return o2Calib.sensitivity;
}

float O2_ConvertRawToPercent(int16_t raw)
{
    float v = ADC_ToVoltage(raw);
    float i_uA = (v / O2_RTIA_OHMS) * 1e6f;
    return (i_uA - o2Calib.zero_uA) / o2Calib.sensitivity;
}

/* ─── O2 Calculation ────────────────────────────────────────────────────── */

float O2_CalcPercent(int16_t adc_raw)
{
    float slope = -0.023304f;
    float offset = 258.37f;

    float pct = (slope * (float)adc_raw) + offset;

    // Boundary limits
    if (pct < 0.0f)  pct = 0.0f;
    if (pct > 25.0f) pct = 25.0f;

    return pct;
}


/* ─── CO Calculation ────────────────────────────────────────────────────── */

float CO_CalcPPM(int16_t adc_raw)
{
    const float adc_zero = (float)CO_ADC_AT_ZERO;
    const float adc_full = (float)CO_ADC_AT_FULL;

    if (adc_full == adc_zero) return -1.0f;

    float ppm = CO_PPM_MIN +
                ((float)adc_raw - adc_zero) *
                (CO_PPM_MAX - CO_PPM_MIN) /
                (adc_full - adc_zero);


    if (ppm < 0.0f)            ppm = 0.0f;
    if (ppm > CO_PPM_MAX)      ppm = CO_PPM_MAX;

    return ppm;
}


long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
