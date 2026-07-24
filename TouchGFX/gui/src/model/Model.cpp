#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "sensor_data.h"
#include "stm32h7xx_hal.h"
#include "ee_test.h"

extern "C" {
#include "gas_sensor.h"
#include "slope_offset_calib.h"
}

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef RTC_Time;
extern RTC_DateTypeDef RTC_Date;
#endif

Model::Model()
    : modelListener(0)
#ifndef SIMULATOR
    , lastCoPpm(0.0f)
    , lastO2Percent(0.0f)
    , lastCO2ppm(0.0f)
    , lastProbTemp(0.0f)
#endif
{
}

void Model::calibrateCOZero()
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    int16_t raw = gSensorData.rawCO;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.co.raw_zero = (float)raw;
    g_CalibData.co.ref_zero = 0.0f;
    Calib_CalculateSlopeOffset(&g_CalibData.co);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::calibrateCOSpan(float target_ppm)
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    int16_t raw = gSensorData.rawCO;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.co.raw_span = (float)raw;
    g_CalibData.co.ref_span = target_ppm;
    Calib_CalculateSlopeOffset(&g_CalibData.co);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::calibrateO2Zero()
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    int16_t raw = gSensorData.rawO2;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.o2.raw_zero = (float)raw;
    g_CalibData.o2.ref_zero = 0.0f;
    Calib_CalculateSlopeOffset(&g_CalibData.o2);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::calibrateO2Span(float target_pct)
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    int16_t raw = gSensorData.rawO2;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.o2.raw_span = (float)raw;
    g_CalibData.o2.ref_span = target_pct;
    Calib_CalculateSlopeOffset(&g_CalibData.o2);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::calibrateCO2Zero()
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    uint16_t raw = gSensorData.rawCO2;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.co2.raw_zero = (float)raw / 1000.0f;
    g_CalibData.co2.ref_zero = 0.0f;
    Calib_CalculateSlopeOffset(&g_CalibData.co2);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::calibrateCO2Span(float target_pct)
{
#ifndef SIMULATOR
    osMutexAcquire(sensorDataMutexHandle, osWaitForever);
    uint16_t raw = gSensorData.rawCO2;
    osMutexRelease(sensorDataMutexHandle);

    g_CalibData.co2.raw_span = (float)raw / 1000.0f;
    g_CalibData.co2.ref_span = target_pct;
    Calib_CalculateSlopeOffset(&g_CalibData.co2);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::resetCOCal()
{
#ifndef SIMULATOR
    Calib_ResetCO(&g_CalibData);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::resetO2Cal()
{
#ifndef SIMULATOR
    Calib_ResetO2(&g_CalibData);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::resetCO2Cal()
{
#ifndef SIMULATOR
    Calib_ResetCO2(&g_CalibData);
    Calib_SaveToEEPROM(&g_CalibData);
#endif
}

void Model::tick()
{
    if (!modelListener)
    {
        return;
    }

#ifndef SIMULATOR
    HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);

    modelListener->updateTime(RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);

    osMutexAcquire(sensorDataMutexHandle, osWaitForever);

    float temp = gSensorData.temperature;
    float hum = gSensorData.humidity;
    int16_t raw_co = gSensorData.rawCO;
    int16_t raw_o2 = gSensorData.rawO2;
    float cal_co = gSensorData.co_ppm;
    float cal_o2 = gSensorData.o2_percent;
    uint16_t raw_co2 = gSensorData.rawCO2;
    float cal_co2 = gSensorData.co2_p;
    float probe = gSensorData.probe_temp;

    osMutexRelease(sensorDataMutexHandle);

    modelListener->onSht22Updated(temp, hum);
    modelListener->onGasUpdated(cal_co, cal_o2);
    modelListener->onCO2Updated(cal_co2 * 10000.0f);

    if (probe != lastProbTemp)
    {
        lastProbTemp = probe;
        modelListener->onProbeUpdated(probe);
    }

    modelListener->onCOCalDataUpdated(raw_co, cal_co);
    modelListener->onO2CalDataUpdated(raw_o2, cal_o2);
    modelListener->onCO2CalDataUpdated(raw_co2, cal_co2);
#else
    modelListener->updateTime(12, 34, 56);
    modelListener->onSht22Updated(25.0f, 45.0f);
    modelListener->onGasUpdated(18.0f, 20.9f);
    modelListener->onCO2Updated(650.0f);
    modelListener->onProbeUpdated(26.5f);
    modelListener->onCOCalDataUpdated(500, 18.0f);
    modelListener->onO2CalDataUpdated(10190, 20.9f);
    modelListener->onCO2CalDataUpdated(1000, 10.0f);
#endif
}
bool Model::getEEPROMStatus()
{
#ifndef SIMULATOR
    return EEPROM_OK;
#else
    return true;
#endif
}
