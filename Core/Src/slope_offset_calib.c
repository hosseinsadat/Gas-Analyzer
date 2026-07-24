#include "slope_offset_calib.h"
#include "at24cxx.h"
#include <string.h>
#include <stddef.h>
#include <math.h>

EEPROM_CalibData_t g_CalibData;

void Calib_ResetDefaults(EEPROM_CalibData_t *data)
{
    memset(data, 0, sizeof(EEPROM_CalibData_t));
    data->magic = CALIB_MAGIC_NUMBER;

    Calib_ResetCO(data);
    Calib_ResetO2(data);
    Calib_ResetCO2(data);

    data->checksum = Calib_CalculateChecksum(data);
}

void Calib_ResetCO(EEPROM_CalibData_t *data)
{
    data->co.ref_zero = 0.0f;
    data->co.ref_span = 1000.0f;
    data->co.raw_zero = 500.0f;
    data->co.raw_span = 32000.0f;
    Calib_CalculateSlopeOffset(&data->co);
}

void Calib_ResetO2(EEPROM_CalibData_t *data)
{
    data->o2.ref_zero = 0.0f;
    data->o2.ref_span = 20.9f;
    data->o2.raw_zero = 11087.0f;
    data->o2.raw_span = 10190.0f;
    Calib_CalculateSlopeOffset(&data->o2);
}

void Calib_ResetCO2(EEPROM_CalibData_t *data)
{
    data->co2.ref_zero = 0.0f;
    data->co2.ref_span = 10.0f;
    data->co2.raw_zero = 0.0f;
    data->co2.raw_span = 10.0f;
    Calib_CalculateSlopeOffset(&data->co2);
}

uint32_t Calib_CalculateChecksum(const EEPROM_CalibData_t *data)
{
    uint32_t sum = 0;
    const uint8_t *ptr = (const uint8_t *)data;
    /* Calculate sum of all bytes except the checksum field itself */
    size_t size = offsetof(EEPROM_CalibData_t, checksum);
    for (size_t i = 0; i < size; i++)
    {
        sum += ptr[i];
    }
    return sum;
}

bool Calib_LoadFromEEPROM(EEPROM_CalibData_t *data)
{
    if (!at24_isConnected())
    {
        return false;
    }

    EEPROM_CalibData_t temp;
    if (!at24_read(EEPROM_CALIB_ADDR, (uint8_t *)&temp, sizeof(EEPROM_CalibData_t), 1000))
    {
        return false;
    }

    if (temp.magic != CALIB_MAGIC_NUMBER)
    {
        return false;
    }

    uint32_t calc = Calib_CalculateChecksum(&temp);
    if (temp.checksum != calc)
    {
        return false;
    }

    memcpy(data, &temp, sizeof(EEPROM_CalibData_t));
    return true;
}

bool Calib_SaveToEEPROM(EEPROM_CalibData_t *data)
{
    if (!at24_isConnected())
    {
        return false;
    }

    data->magic = CALIB_MAGIC_NUMBER;
    data->checksum = Calib_CalculateChecksum(data);

    return at24_write(EEPROM_CALIB_ADDR, (uint8_t *)data, sizeof(EEPROM_CalibData_t), 1000);
}

void Calib_CalculateSlopeOffset(SensorCalibParams_t *params)
{
    float denom = params->raw_span - params->raw_zero;

    /* Avoid division by zero or extremely small spans */
    if (fabsf(denom) < 1e-4f)
    {
        params->slope = 1.0f;
        params->offset = 0.0f;
        return;
    }

    float calculated_slope = (params->ref_span - params->ref_zero) / denom;
    float calculated_offset = params->ref_zero - (calculated_slope * params->raw_zero);

    /* Validate results to prevent storing NaN or Inf in EEPROM */
    if (isnan(calculated_slope) || isinf(calculated_slope) ||
        isnan(calculated_offset) || isinf(calculated_offset))
    {
        params->slope = 1.0f;
        params->offset = 0.0f;
    }
    else
    {
        params->slope = calculated_slope;
        params->offset = calculated_offset;
    }
}

float Calib_Apply(const SensorCalibParams_t *params, float raw)
{
    /* Protect against NaN/Inf values */
    if (isnan(raw) || isinf(raw) ||
        isnan(params->slope) || isinf(params->slope) ||
        isnan(params->offset) || isinf(params->offset))
    {
        return 0.0f;
    }

    float result = (params->slope * raw) + params->offset;
    if (isnan(result) || isinf(result))
    {
        return 0.0f;
    }
    return result;
}
