/*
 * ee_test.h
 *
 *  Created on: Jul 23, 2026
 *      Author: hosss
 */

#ifndef INC_EE_TEST_H_
#define INC_EE_TEST_H_



#include "at24cxx.h"
#include <stdbool.h>

extern volatile bool EEPROM_OK;


void EEPROM_Test(void);



#endif /* INC_EE_TEST_H_ */
