/**
 ******************************************************************************
 * @file    k33_sensor.h
 * @brief   K33-specific register access, built on k33_modbus.
 *          Register map per Senseair TDE2336 rev 11 ("Modbus on Senseair
 *          K30, K33 and eSENSE"), sections 3 & 7.
 ******************************************************************************
 */
#ifndef K33_SENSOR_H
#define K33_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "k33_modbus.h"

/* Named constants for K33 Modbus Calibration */
#define K33_MODBUS_ADDR_BROADCAST    0xFEu
#define K33_REG_HR_ACK               0x0000u  /* HR1 - Acknowledgement register */
#define K33_REG_HR_SPECIAL_CMD       0x0001u  /* HR2 - Special Command register */

#define K33_CMD_CALIBRATE            0x7Cu
#define K33_PARAM_CALIB_AIR          0x06u    /* Background / fresh-air calibration */
#define K33_PARAM_CALIB_N2           0x07u    /* Nitrogen / zero calibration */

#define K33_ACK_BIT_AIR              0x0020u  /* CI6 - bit 5 set on fresh-air calibration success */
#define K33_ACK_BIT_N2               0x0040u  /* CI7 - bit 6 set on nitrogen zero calibration success */

/* MeterStatus (IR1) bit meanings, datasheet Table 3 */
typedef enum
{
    K33_STATUS_FATAL_ERROR       = (1u << 0),
    K33_STATUS_OFFSET_REG_ERROR  = (1u << 1),
    K33_STATUS_ALGORITHM_ERROR   = (1u << 2),
    K33_STATUS_OUTPUT_ERROR      = (1u << 3),
    K33_STATUS_SELF_DIAG_ERROR   = (1u << 4),
    K33_STATUS_OUT_OF_RANGE      = (1u << 5),
    K33_STATUS_MEMORY_ERROR      = (1u << 6),
} K33_StatusBit_t;

/* Acknowledgement register (HR1) bit meanings, datasheet Table 4 */
typedef enum
{
    K33_ACK_BACKGROUND_CAL_DONE = K33_ACK_BIT_AIR,
    K33_ACK_NITROGEN_ZERO_CAL_DONE = K33_ACK_BIT_N2,
} K33_AckBit_t;

typedef struct
{
    uint16_t raw;     /* raw register value as returned by the sensor */
    float    value;   /* scaled per K33_CO2_RAW_DIVISOR */
} K33_CO2Reading_t;

/**
 * @brief Must be called after K33_Modbus_Init().
 */
K33_Status_t K33_Sensor_Init(void);

/** Read Space CO2 (IR4, function 04). */
K33_Status_t K33_ReadCO2(K33_CO2Reading_t *out);

/** Read MeterStatus (IR1, function 04). Compare bits against K33_StatusBit_t. */
K33_Status_t K33_ReadStatus(uint16_t *status_bits);

/** Read CO2 (IR4) + status (IR1) in a single Modbus transaction (IR1..IR4). */
K33_Status_t K33_ReadStatusAndCO2(uint16_t *status_bits, K33_CO2Reading_t *co2);

/** Clear the acknowledgement register (HR1) before requesting a calibration. */
K33_Status_t K33_ClearAckRegister(void);

/** Write the Special Command Register (HR2) to start a background calibration. */
K33_Status_t K33_StartBackgroundCalibration(void);

/** Write the Special Command Register (HR2) to start a zero/nitrogen calibration. */
K33_Status_t K33_StartZeroCalibration(void);

/** Read HR1 and report whether the given K33_AckBit_t is set. */
K33_Status_t K33_CheckCalibrationAck(K33_AckBit_t bit, bool *is_set);

/** ABC (automatic background calibration) period, HR32, in hours. 0 = disabled. */
K33_Status_t K33_GetABCPeriod(uint16_t *hours);
K33_Status_t K33_SetABCPeriod(uint16_t hours);
static inline K33_Status_t K33_DisableABC(void) { return K33_SetABCPeriod(0); }

/* Generic Modbus CRC-16 calculation function (Modbus polynomial 0xA001) */
uint16_t computeModbusCRC16(const uint8_t *buf, uint16_t len);

/* Transmits the given raw byte array over UART and returns received bytes */
int sendModbusFrame(const uint8_t *tx_frame, uint16_t tx_len, uint8_t *rx_frame, uint16_t rx_cap, uint32_t timeout_ms);

/* Senseair K33 Calibration Triggers */
bool calibrateZeroAir(void);
bool calibrateZeroN2(void);

#ifdef __cplusplus
}
#endif

#endif /* K33_SENSOR_H */
