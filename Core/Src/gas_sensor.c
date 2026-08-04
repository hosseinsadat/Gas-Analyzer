/**
 ******************************************************************************
 * @file    gas_sensor.c
 * @brief   Implementation of gas sensor module tasks and utilities.
 ******************************************************************************
 */
#include "gas_sensor.h"
#include "cmsis_os.h"

static GasSensorData_t g_LatestGasSensorData;
static osMutexId_t g_GasSensorMutex;

void GasSensor_Init(void)
{
    g_GasSensorMutex = osMutexNew(NULL);
}

void GasSensor_Update(void)
{
    /* Utility function reserved for periodic updates */
}

GasSensorData_t GasSensor_GetData(void)
{
    GasSensorData_t data;
    if (g_GasSensorMutex != NULL)
    {
        osMutexAcquire(g_GasSensorMutex, osWaitForever);
        data = g_LatestGasSensorData;
        osMutexRelease(g_GasSensorMutex);
    }
    return data;
}
