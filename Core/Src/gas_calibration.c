/**
 ******************************************************************************
 * @file    gas_calibration.c
 * @brief   Implementation of gas sensor calibration using slope and offset.
 ******************************************************************************
 */
#include "gas_calibration.h"

void GasCalibration_Init(void)
{
    /* Try to load calibration data from EEPROM */
    if (!Calib_LoadFromEEPROM(&g_CalibData))
    {
        /* If load fails, set default values and save them to EEPROM */
        Calib_ResetDefaults(&g_CalibData);
        Calib_SaveToEEPROM(&g_CalibData);
    }
}

float O2_CalcPercent(int16_t adc_raw)
{
    float pct = Calib_Apply(&g_CalibData.o2, (float)adc_raw);

    /* Apply boundary checks for O2 sensor */
    if (pct < 0.0f)  pct = 0.0f;
    if (pct > 25.0f) pct = 25.0f;

    return pct;
}

float CO_CalcPPM(int16_t adc_raw)
{
    float ppm = Calib_Apply(&g_CalibData.co, (float)adc_raw);

    /* Apply boundary checks for CO sensor */
    if (ppm < 0.0f)  ppm = 0.0f;
    if (ppm > 2000.0f) ppm = 2000.0f;

    return ppm;
}

float CO2_CalcPercent(uint16_t raw_co2)
{
    /* Convert raw value from K33 sensor (it divides by 100 to get percent) */
    float pct = Calib_Apply(&g_CalibData.co2, (float)raw_co2 / 100.0f);

    /* Apply boundary checks for CO2 sensor (0 to 30.0%) */
    if (pct < 0.0f)  pct = 0.0f;
    if (pct > 30.0f) pct = 30.0f;

    return pct;
}
