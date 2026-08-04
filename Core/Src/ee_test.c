/*
 * ee_test.c
 *
 *  Created on: Jul 23, 2026
 *      Author: hosss
 */

#include "ee_test.h"
#include "at24cxx.h"

volatile bool EEPROM_OK = false;

void EEPROM_Test(void)
{
    /* Simply check if the EEPROM is connected and responding at its physical I2C address */
    EEPROM_OK = at24_isConnected();
}
