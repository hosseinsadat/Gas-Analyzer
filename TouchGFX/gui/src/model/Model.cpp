#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "sensor_data.h"
#include "stm32h7xx_hal.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef RTC_Time;
extern RTC_DateTypeDef RTC_Date;

extern "C" {
#include "gas_sensor.h"
}
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
// Span Functions
void Model::saveCOSpan(int raw, int target) {


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
    float co = gSensorData.rawCO;
    float o2 = gSensorData.rawO2;
    float co2 = gSensorData.co2_ppm;
    float probe = gSensorData.probe_temp;

    osMutexRelease(sensorDataMutexHandle);

    modelListener->onSht22Updated(temp, hum);
    modelListener->onGasUpdated(co, o2);
    modelListener->onCO2Updated(co2);
//    if (co2 >= 0.0f && co2 <= 30000.0f)
//    {
//        if (co2 != lastCO2ppm)
//        {
//            lastCO2ppm = co2;
//
//        }
//    }
//    else
//    {
//        modelListener->onCO2Updated(0.0f);
//    }

    if (probe != lastProbTemp)
    {
        lastProbTemp = probe;
        modelListener->onProbeUpdated(probe);
    }
#else
    modelListener->updateTime(12, 34, 56);
    modelListener->onSht22Updated(25.0f, 45.0f);
    modelListener->onGasUpdated(18.0f, 20.9f);
    modelListener->onCO2Updated(650.0f);
    modelListener->onProbeUpdated(26.5f);
#endif
}
