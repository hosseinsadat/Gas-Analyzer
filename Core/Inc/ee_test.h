/*
 * ee_test.h
 *
 *  Created on: Jul 23, 2026
 *      Author: hosss
 */

#ifndef INC_EE_TEST_H_
#define INC_EE_TEST_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile bool EEPROM_OK;

/**
 * @brief Performs a safe, non-destructive health check of the EEPROM.
 *        Updates the global EEPROM_OK status flag.
 */
void EEPROM_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_EE_TEST_H_ */
