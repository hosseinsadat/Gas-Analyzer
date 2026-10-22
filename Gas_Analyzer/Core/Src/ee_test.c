/*
 * ee_test.c
 *
 *  Created on: Jul 23, 2026
 *      Author: hosss
 */

#include "ee_test.h"
#include "at24cxx.h"
#include <string.h>

#if (_EEPROM_USE_FREERTOS == 1)
#include "FreeRTOS.h"
#include "task.h"
#define ee_test_delay(x) vTaskDelay(pdMS_TO_TICKS(x))
#else
#include "main.h"
#define ee_test_delay(x) HAL_Delay(x)
#endif

/* Safe, unused temporary test address on EEPROM far from the calibration sector to avoid overwrite */
#define EEPROM_TEST_ADDR 0x0000

void EEPROM_Test(void)
{
    uint8_t tx[4];
    uint8_t rx[4];
    uint8_t original[4];

    /* Initialize buffers */
    tx[0] = 0xAA;
    tx[1] = 0x55;
    tx[2] = 0x12;
    tx[3] = 0x34;

    memset(rx, 0, sizeof(rx));
    memset(original, 0, sizeof(original));

    if (!at24_isConnected())
    {
        EEPROM_OK = false;
        return;
    }

    /* Save original content to restore later */
    if (!at24_read(EEPROM_TEST_ADDR, original, sizeof(original), 1000))
    {
        EEPROM_OK = false;
        return;
    }

    /* Perform a dry-run safe write sequence */
    if (!at24_write(EEPROM_TEST_ADDR, tx, sizeof(tx), 1000))
    {
        EEPROM_OK = false;
        return;
    }

    /* Wait for the EEPROM's internal write cycle to complete (typically 5-10ms max) */
    ee_test_delay(15);

    /* Read back and compare */
    if (!at24_read(EEPROM_TEST_ADDR, rx, sizeof(rx), 1000))
    {
        EEPROM_OK = false;
        return;
    }

    /* Restore original data */
    if (at24_write(EEPROM_TEST_ADDR, original, sizeof(original), 1000))
    {
        /* Wait for write cycle */
        ee_test_delay(15);
    }

    /* Verify if written and read data match */
    if (memcmp(tx, rx, sizeof(tx)) == 0)
    {
        EEPROM_OK = true;
    }
    else
    {
        EEPROM_OK = false;
    }
}
