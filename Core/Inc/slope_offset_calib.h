#ifndef INC_SLOPE_OFFSET_CALIB_H_
#define INC_SLOPE_OFFSET_CALIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define CALIB_MAGIC_NUMBER 0xCA1B2026
#define EEPROM_CALIB_ADDR  0x0100

typedef struct {
    float raw_zero;
    float raw_span;
    float ref_zero;
    float ref_span;
    float slope;
    float offset;
} SensorCalibParams_t;

typedef struct {
    uint32_t magic;
    SensorCalibParams_t co;
    SensorCalibParams_t o2;
    SensorCalibParams_t co2;
    uint32_t checksum;
} EEPROM_CalibData_t;

extern EEPROM_CalibData_t g_CalibData;

void Calib_ResetDefaults(EEPROM_CalibData_t *data);
void Calib_ResetCO(EEPROM_CalibData_t *data);
void Calib_ResetO2(EEPROM_CalibData_t *data);
void Calib_ResetCO2(EEPROM_CalibData_t *data);

uint32_t Calib_CalculateChecksum(const EEPROM_CalibData_t *data);
bool Calib_LoadFromEEPROM(EEPROM_CalibData_t *data);
bool Calib_SaveToEEPROM(EEPROM_CalibData_t *data);
void Calib_CalculateSlopeOffset(SensorCalibParams_t *params);
float Calib_Apply(const SensorCalibParams_t *params, float raw);

#ifdef __cplusplus
}
#endif

#endif /* INC_SLOPE_OFFSET_CALIB_H_ */
