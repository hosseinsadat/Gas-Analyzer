/**
 ******************************************************************************
 * @file    k33_sensor.c
 * @brief   Senseair K33 CO2 sensor driver implementation over Modbus RTU.
 ******************************************************************************
 */
#include <string.h>
#include <stdio.h>
#include "k33_sensor.h"
#include "stm32h7xx_hal.h"

#if (_EEPROM_USE_FREERTOS == 1)
#include "FreeRTOS.h"
#include "task.h"
#define k33_delay(x) vTaskDelay(pdMS_TO_TICKS(x))
#else
#define k33_delay(x) HAL_Delay(x)
#endif

extern UART_HandleTypeDef huart4;

/* Register addresses = (register number - 1), per datasheet convention */
#define REG_IR_METER_STATUS   0x0000u  /* IR1 */
#define REG_IR_SPACE_CO2      0x0003u  /* IR4 */
#define REG_HR_ACK            0x0000u  /* HR1 */
#define REG_HR_SPECIAL_CMD    0x0001u  /* HR2 */
#define REG_HR_ABC_PERIOD     0x001Fu  /* HR32 */

#define SPECIAL_CMD_CALIBRATE        0x7Cu
#define SPECIAL_CMD_PARAM_BACKGROUND  0x06u
#define SPECIAL_CMD_PARAM_ZERO         0x07u

/**
 * @brief Computes standard Modbus RTU CRC-16 (polynomial 0xA001).
 */
uint16_t computeModbusCRC16(const uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];
        for (uint8_t i = 0; i < 8; i++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief Transmits a raw Modbus RTU frame over UART and returns received bytes.
 *        Logs raw TX/RX frames in hexadecimal format.
 */
int sendModbusFrame(const uint8_t *tx_frame, uint16_t tx_len, uint8_t *rx_frame, uint16_t rx_cap, uint32_t timeout_ms)
{
    if ((tx_frame == NULL) || (tx_len == 0) || (rx_frame == NULL) || (rx_cap == 0))
    {
        return -1;
    }

#ifndef SIMULATOR
    /* Log raw TX frame */
    printf("[K33 Modbus TX]: ");
    for (uint16_t i = 0; i < tx_len; i++)
    {
        printf("%02X ", tx_frame[i]);
    }
    printf("\r\n");

    /* Flush UART DR register before transfer */
    __HAL_UART_FLUSH_DRREGISTER(&huart4);

    if (HAL_UART_Transmit(&huart4, (uint8_t *)tx_frame, tx_len, timeout_ms) != HAL_OK)
    {
        printf("[K33 Modbus Error]: UART TX Failed\r\n");
        return -1;
    }

    /* Wait for response */
    if (HAL_UART_Receive(&huart4, rx_frame, rx_cap, timeout_ms) != HAL_OK)
    {
        printf("[K33 Modbus Error]: UART RX Timeout / Error\r\n");
        return -1;
    }

    /* Log raw RX frame */
    printf("[K33 Modbus RX]: ");
    for (uint16_t i = 0; i < rx_cap; i++)
    {
        printf("%02X ", rx_frame[i]);
    }
    printf("\r\n");

    /* Validate CRC of received frame */
    if (rx_cap >= 4)
    {
        uint16_t rx_crc = computeModbusCRC16(rx_frame, rx_cap - 2);
        uint16_t frame_crc = (uint16_t)rx_frame[rx_cap - 2] | ((uint16_t)rx_frame[rx_cap - 1] << 8);
        if (rx_crc != frame_crc)
        {
            printf("[K33 Modbus Error]: RX CRC Mismatch\r\n");
            return -1;
        }
    }

    return (int)rx_cap;
#else
    /* Simulator stub: copy request back as echo for write commands, or dummy status for read */
    memcpy(rx_frame, tx_frame, tx_len < rx_cap ? tx_len : rx_cap);
    return (int)(tx_len < rx_cap ? tx_len : rx_cap);
#endif
}

/**
 * @brief Performs "Zero in Air" (fresh air / background) calibration on Senseair K33 over UART.
 */
bool calibrateZeroAir(void)
{
    uint8_t tx[8];
    uint8_t rx[8];

    /* Step 1: Clear HR1 (Acknowledgement register) */
    /* Command: FE 06 00 00 00 00 -> CRC: 9D C5 */
    tx[0] = K33_MODBUS_ADDR_BROADCAST;
    tx[1] = 0x06;
    tx[2] = (uint8_t)(K33_REG_HR_ACK >> 8);
    tx[3] = (uint8_t)(K33_REG_HR_ACK & 0xFF);
    tx[4] = 0x00;
    tx[5] = 0x00;
    uint16_t crc1 = computeModbusCRC16(tx, 6);
    tx[6] = (uint8_t)(crc1 & 0xFF);
    tx[7] = (uint8_t)((crc1 >> 8) & 0xFF);

    memset(rx, 0, sizeof(rx));
    if (sendModbusFrame(tx, 8, rx, 8, 300) != 8)
    {
        return false;
    }
    /* Verify echo reply */
    if (memcmp(tx, rx, 8) != 0)
    {
        return false;
    }

    /* Step 2: Send calibration command (HR2 = 0x7C06 for Air / Background) */
    /* Command: FE 06 00 01 7C 06 -> CRC: 6C C7 */
    tx[0] = K33_MODBUS_ADDR_BROADCAST;
    tx[1] = 0x06;
    tx[2] = (uint8_t)(K33_REG_HR_SPECIAL_CMD >> 8);
    tx[3] = (uint8_t)(K33_REG_HR_SPECIAL_CMD & 0xFF);
    tx[4] = K33_CMD_CALIBRATE;
    tx[5] = K33_PARAM_CALIB_AIR;
    uint16_t crc2 = computeModbusCRC16(tx, 6);
    tx[6] = (uint8_t)(crc2 & 0xFF);
    tx[7] = (uint8_t)((crc2 >> 8) & 0xFF);

    memset(rx, 0, sizeof(rx));
    if (sendModbusFrame(tx, 8, rx, 8, 300) != 8)
    {
        return false;
    }
    /* Verify echo reply */
    if (memcmp(tx, rx, 8) != 0)
    {
        return false;
    }

    /* Step 3: Wait at least 2.5 seconds for calibration sequence to execute */
    k33_delay(2500);

    /* Step 4: Poll HR1 to verify acknowledgement bit CI6 (0x0020) */
    /* Read Command: FE 03 00 00 00 01 -> CRC: 90 05 */
    uint8_t read_tx[6] = { K33_MODBUS_ADDR_BROADCAST, 0x03, 0x00, 0x00, 0x00, 0x01 };
    uint16_t crc_read = computeModbusCRC16(read_tx, 6);
    uint8_t read_tx_full[8];
    memcpy(read_tx_full, read_tx, 6);
    read_tx_full[6] = (uint8_t)(crc_read & 0xFF);
    read_tx_full[7] = (uint8_t)((crc_read >> 8) & 0xFF);

    uint8_t read_rx[7];
    bool success = false;

    /* Up to 3 retry budget attempts, 1 second apart */
    for (int retry = 0; retry < 3; retry++)
    {
        memset(read_rx, 0, sizeof(read_rx));
        if (sendModbusFrame(read_tx_full, 8, read_rx, 7, 300) == 7)
        {
            if (read_rx[0] == K33_MODBUS_ADDR_BROADCAST && read_rx[1] == 0x03 && read_rx[2] == 0x02)
            {
                uint16_t ack_val = ((uint16_t)read_rx[3] << 8) | read_rx[4];
                if ((ack_val & K33_ACK_BIT_AIR) != 0)
                {
                    success = true;
                    break;
                }
            }
        }
        k33_delay(1000);
    }

    return success;
}

/**
 * @brief Performs "Zero in N2" (Nitrogen / true zero) calibration on Senseair K33 over UART.
 */
bool calibrateZeroN2(void)
{
    uint8_t tx[8];
    uint8_t rx[8];

    /* Step 1: Clear HR1 (Acknowledgement register) */
    /* Command: FE 06 00 00 00 00 -> CRC: 9D C5 */
    tx[0] = K33_MODBUS_ADDR_BROADCAST;
    tx[1] = 0x06;
    tx[2] = (uint8_t)(K33_REG_HR_ACK >> 8);
    tx[3] = (uint8_t)(K33_REG_HR_ACK & 0xFF);
    tx[4] = 0x00;
    tx[5] = 0x00;
    uint16_t crc1 = computeModbusCRC16(tx, 6);
    tx[6] = (uint8_t)(crc1 & 0xFF);
    tx[7] = (uint8_t)((crc1 >> 8) & 0xFF);

    memset(rx, 0, sizeof(rx));
    if (sendModbusFrame(tx, 8, rx, 8, 300) != 8)
    {
        return false;
    }
    /* Verify echo reply */
    if (memcmp(tx, rx, 8) != 0)
    {
        return false;
    }

    /* Step 2: Send calibration command (HR2 = 0x7C07 for N2 / Zero) */
    /* Command: FE 06 00 01 7C 07 -> CRC: AD 07 */
    tx[0] = K33_MODBUS_ADDR_BROADCAST;
    tx[1] = 0x06;
    tx[2] = (uint8_t)(K33_REG_HR_SPECIAL_CMD >> 8);
    tx[3] = (uint8_t)(K33_REG_HR_SPECIAL_CMD & 0xFF);
    tx[4] = K33_CMD_CALIBRATE;
    tx[5] = K33_PARAM_CALIB_N2;
    uint16_t crc2 = computeModbusCRC16(tx, 6);
    tx[6] = (uint8_t)(crc2 & 0xFF);
    tx[7] = (uint8_t)((crc2 >> 8) & 0xFF);

    memset(rx, 0, sizeof(rx));
    if (sendModbusFrame(tx, 8, rx, 8, 300) != 8)
    {
        return false;
    }
    /* Verify echo reply */
    if (memcmp(tx, rx, 8) != 0)
    {
        return false;
    }

    /* Step 3: Wait at least 2.5 seconds for calibration sequence to execute */
    k33_delay(2500);

    /* Step 4: Poll HR1 to verify acknowledgement bit CI7 (0x0040) */
    /* Read Command: FE 03 00 00 00 01 -> CRC: 90 05 */
    uint8_t read_tx[6] = { K33_MODBUS_ADDR_BROADCAST, 0x03, 0x00, 0x00, 0x00, 0x01 };
    uint16_t crc_read = computeModbusCRC16(read_tx, 6);
    uint8_t read_tx_full[8];
    memcpy(read_tx_full, read_tx, 6);
    read_tx_full[6] = (uint8_t)(crc_read & 0xFF);
    read_tx_full[7] = (uint8_t)((crc_read >> 8) & 0xFF);

    uint8_t read_rx[7];
    bool success = false;

    /* Up to 3 retry budget attempts, 1 second apart */
    for (int retry = 0; retry < 3; retry++)
    {
        memset(read_rx, 0, sizeof(read_rx));
        if (sendModbusFrame(read_tx_full, 8, read_rx, 7, 300) == 7)
        {
            if (read_rx[0] == K33_MODBUS_ADDR_BROADCAST && read_rx[1] == 0x03 && read_rx[2] == 0x02)
            {
                uint16_t ack_val = ((uint16_t)read_rx[3] << 8) | read_rx[4];
                if ((ack_val & K33_ACK_BIT_N2) != 0)
                {
                    success = true;
                    break;
                }
            }
        }
        k33_delay(1000);
    }

    return success;
}

static K33_Status_t read_registers(uint8_t func_code, uint16_t start_addr, uint16_t qty,
                                    uint8_t *out_data, uint16_t out_data_cap, uint16_t *out_bytes)
{
    uint8_t req[6] = {
        K33_SLAVE_ADDRESS, func_code,
        (uint8_t)(start_addr >> 8), (uint8_t)(start_addr & 0xFF),
        (uint8_t)(qty >> 8),        (uint8_t)(qty & 0xFF),
    };
    uint8_t resp[K33_MAX_FRAME_LEN];
    uint16_t expLen = 3u + (2u * qty) + 2u;
    uint16_t actLen = 0;

    K33_Status_t st = K33_Modbus_Transact(req, sizeof(req), resp, sizeof(resp), expLen, &actLen);
    if (st != K33_OK)
    {
        return st;
    }

    uint8_t byteCount = resp[2];
    if ((byteCount > out_data_cap) || ((3u + byteCount + 2u) > actLen))
    {
        return K33_ERR_LEN;
    }
    memcpy(out_data, &resp[3], byteCount);
    if (out_bytes) { *out_bytes = byteCount; }
    return K33_OK;
}

static K33_Status_t write_register(uint16_t addr, uint16_t value)
{
    uint8_t req[6] = {
        K33_SLAVE_ADDRESS, 0x06u,
        (uint8_t)(addr >> 8), (uint8_t)(addr & 0xFF),
        (uint8_t)(value >> 8), (uint8_t)(value & 0xFF),
    };
    uint8_t resp[K33_MAX_FRAME_LEN];
    uint16_t actLen = 0;
    return K33_Modbus_Transact(req, sizeof(req), resp, sizeof(resp), 8u, &actLen);
}

static float scale_co2(uint16_t raw)
{
    return (float)raw / (float)K33_CO2_RAW_DIVISOR;
}

K33_Status_t K33_Sensor_Init(void)
{
    return K33_OK;
}

K33_Status_t K33_ReadCO2(K33_CO2Reading_t *out)
{
    if (out == NULL) { return K33_ERR_PARAM; }

#ifndef SIMULATOR
    /* Documented Modbus RTU read: function 0x04 (Read Input Registers),
     * IR4 = Space CO2, address 0x0003, quantity 1.
     * Request:  FE 04 00 03 00 01 D5 C5
     * Response: FE 04 02 <CO2 Hi> <CO2 Lo> <CRC Lo> <CRC Hi>  (7 bytes) */
    static const uint8_t req[8] = {0xFE, 0x04, 0x00, 0x03, 0x00, 0x01, 0xD5, 0xC5};
    uint8_t resp[7] = {0};

    __HAL_UART_FLUSH_DRREGISTER(&huart4);

    if (HAL_UART_Transmit(&huart4, (uint8_t *)req, sizeof(req), 100) == HAL_OK) {
        if (HAL_UART_Receive(&huart4, resp, sizeof(resp), 200) == HAL_OK) {
            uint16_t rxCrc = computeModbusCRC16(resp, sizeof(resp) - 2u);
            uint16_t gotCrc = (uint16_t)resp[sizeof(resp) - 2u] | ((uint16_t)resp[sizeof(resp) - 1u] << 8);
            if (resp[0] == 0xFE && resp[1] == 0x04 && resp[2] == 0x02 && rxCrc == gotCrc) {
                out->raw = ((uint16_t)resp[3] << 8) | resp[4];
                out->value = scale_co2(out->raw);
                return K33_OK;
            }
        }
    }
    return K33_ERR_TIMEOUT;
#else
    out->raw = 1000;
    out->value = 1.0f;
    return K33_OK;
#endif
}

K33_Status_t K33_ReadStatus(uint16_t *status_bits)
{
    if (status_bits == NULL) { return K33_ERR_PARAM; }
    uint8_t data[2];
    uint16_t nbytes = 0;
    K33_Status_t st = read_registers(0x04u, REG_IR_METER_STATUS, 1u, data, sizeof(data), &nbytes);
    if (st != K33_OK) { return st; }
    if (nbytes != 2u) { return K33_ERR_LEN; }

    *status_bits = ((uint16_t)data[0] << 8) | data[1];
    return K33_OK;
}

K33_Status_t K33_ReadStatusAndCO2(uint16_t *status_bits, K33_CO2Reading_t *co2)
{
    if ((status_bits == NULL) || (co2 == NULL)) { return K33_ERR_PARAM; }
    uint8_t data[8];
    uint16_t nbytes = 0;
    K33_Status_t st = read_registers(0x04u, REG_IR_METER_STATUS, 4u, data, sizeof(data), &nbytes);
    if (st != K33_OK) { return st; }
    if (nbytes != 8u) { return K33_ERR_LEN; }

    *status_bits = ((uint16_t)data[0] << 8) | data[1];
    co2->raw     = ((uint16_t)data[6] << 8) | data[7];
    co2->value   = scale_co2(co2->raw);
    return K33_OK;
}

K33_Status_t K33_ClearAckRegister(void)
{
    return write_register(REG_HR_ACK, 0x0000u);
}

K33_Status_t K33_StartBackgroundCalibration(void)
{
    uint16_t cmd = ((uint16_t)SPECIAL_CMD_CALIBRATE << 8) | SPECIAL_CMD_PARAM_BACKGROUND;
    return write_register(REG_HR_SPECIAL_CMD, cmd);
}

K33_Status_t K33_StartZeroCalibration(void)
{
    uint16_t cmd = ((uint16_t)SPECIAL_CMD_CALIBRATE << 8) | SPECIAL_CMD_PARAM_ZERO;
    return write_register(REG_HR_SPECIAL_CMD, cmd);
}

K33_Status_t K33_CheckCalibrationAck(K33_AckBit_t bit, bool *is_set)
{
    if (is_set == NULL) { return K33_ERR_PARAM; }
    uint8_t data[2];
    uint16_t nbytes = 0;
    K33_Status_t st = read_registers(0x03u, REG_HR_ACK, 1u, data, sizeof(data), &nbytes);
    if (st != K33_OK) { return st; }
    if (nbytes != 2u) { return K33_ERR_LEN; }

    uint16_t ack = ((uint16_t)data[0] << 8) | data[1];
    *is_set = (ack & (uint16_t)bit) != 0;
    return K33_OK;
}

K33_Status_t K33_GetABCPeriod(uint16_t *hours)
{
    if (hours == NULL) { return K33_ERR_PARAM; }
    uint8_t data[2];
    uint16_t nbytes = 0;
    K33_Status_t st = read_registers(0x03u, REG_HR_ABC_PERIOD, 1u, data, sizeof(data), &nbytes);
    if (st != K33_OK) { return st; }
    if (nbytes != 2u) { return K33_ERR_LEN; }

    *hours = ((uint16_t)data[0] << 8) | data[1];
    return K33_OK;
}

K33_Status_t K33_SetABCPeriod(uint16_t hours)
{
    return write_register(REG_HR_ABC_PERIOD, hours);
}
