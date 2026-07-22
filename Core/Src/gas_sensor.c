#include "gas_sensor.h"
#include "gas_calibration.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include "config.h"

#include "LMP91000.h"
#include "lmp91000_board.h"
#include "sensor_data.h"
/* ================= FILTER HELPERS ================= */






/* ================= MODULE DATA ================= */


extern uint16_t adc_raw[2];
extern SensorData_t gSensorData;

static GasSensorData_t sensorData;
static osMutexId_t gasDataMutex;



static volatile bool req_co_zero = false;
static volatile bool req_co_span = false;
static volatile bool req_o2_zero = false;
static float co_span_ppm = 0.0f;

/* ================= API ================= */

void GasSensor_Init(void)
{
    gasDataMutex = osMutexNew(NULL);
}

void GasSensor_Update(void)
{




}

GasSensorData_t GasSensor_GetData(void)
{
    GasSensorData_t copy;
    osMutexAcquire(gasDataMutex, osWaitForever);
    copy = sensorData;
    osMutexRelease(gasDataMutex);
    return copy;
}

/* ================= CALIBRATION REQUESTS ================= */

void GasSensor_RequestCOZero(void)
{
    req_co_zero = true;
}

void GasSensor_RequestCOSpan(float known_ppm)
{
    co_span_ppm = known_ppm;
    req_co_span = true;
}

void GasSensor_RequestO2Zero(void)
{
    req_o2_zero = true;
}

