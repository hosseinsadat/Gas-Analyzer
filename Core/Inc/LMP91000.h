/*
 * LMP91000.h
 *
 *  Created on: Sep 19, 2025
 *      Author: Manufact
 */

#ifndef INC_LMP91000_H_
#define INC_LMP91000_H_

#pragma once
#include "stm32h7xx_hal.h"
#include <stdint.h>

/// LMP91000 definitions
#define LMP91000_I2C_Address                           					(0x91)                     // Device Address
#define TI_LMP91000_STATUS_REG                         					(0x00)                  /* Read only status register */
#define TI_LMP91000_LOCK_REG                           					(0x01)                  /* Protection Register */
#define TI_LMP91000_TIACN_REG                        				  	(0x10)                  /* TIA Control Register */
#define TI_LMP91000_REFCN_REG                          					(0x11)                  /* Reference Control Register*/
#define TI_LMP91000_MODECN_REG                         					(0x12)                  /* Mode Control Register */


/////// STATUS //////////
#define TI_LMP91000_STATUS_NOT_READY 									(0b00000000)
#define TI_LMP91000_STATUS_READY										(0b00000001)

/////// LOCK   //////////
#define TI_LMP91000_LOCK_WRITE_MODE 									(0b00000000)
#define TI_LMP91000_LOCK_READ_ONLY  									(0b00000001)	 //read only mode  --- default

/////// TIACN   //////////(NOTE) Must TIA_GAIN | with RLOAD

///TIA_GAIN
#define TI_LMP91000_TIA_GAIN_EXTERNAL_RESISTANCE						(0b00000000)	//Default External resistance
#define TI_LMP91000_TIA_GAIN_2_75_K_Ohm									(0b00000100)
#define TI_LMP91000_TIA_GAIN_3_5_K_Ohm									(0b00001000)
#define TI_LMP91000_TIA_GAIN_7_K_Ohm									(0b00001100)
#define TI_LMP91000_TIA_GAIN_14_K_Ohm									(0b00010000)
#define TI_LMP91000_TIA_GAIN_35_K_Ohm									(0b00010100)
#define TI_LMP91000_TIA_GAIN_120_K_Ohm									(0b00011000)
#define TI_LMP91000_TIA_GAIN_350_K_Ohm									(0b00011100)

///RLOAD

#define TI_LMP91000_RLAOD_10_Ohm										(0b00000000)
#define TI_LMP91000_RLAOD_33_Ohm										(0b00000001)
#define TI_LMP91000_RLAOD_50_Ohm										(0b00000010)
#define TI_LMP91000_RLAOD_100_Ohm										(0b00000011)		//Default

/////// REFCN   //////////(NOTE) INT_Z | SIGN | BIAS

///INT_Z Internal zero selection
#define TI_LMP91000_INT_Z_20											(0b00000000)
#define TI_LMP91000_INT_Z_50											(0b00100000)
#define TI_LMP91000_INT_Z_67											(0b01000000)
#define TI_LMP91000_INT_Z_O2_GROUDN_REFERRED  							(0b01100000)

///BIAS_SIGN polarity
#define TI_LMP91000_BIAS_SIGN_NEGATIVE									(0b00000000)
#define TI_LMP91000_BIAS_SIGN_POSITIVE									(0b00001000)

///BIAS percentage of the source reference
#define TI_LMP91000_BIAS_SELECTION_0_P									(0b00000000)    ///Default
#define TI_LMP91000_BIAS_SELECTION_1_P									(0b00000001)
#define TI_LMP91000_BIAS_SELECTION_2_P									(0b00000010)
#define TI_LMP91000_BIAS_SELECTION_4_P									(0b00000011)
#define TI_LMP91000_BIAS_SELECTION_6_P									(0b00000100)
#define TI_LMP91000_BIAS_SELECTION_8_P									(0b00000101)
#define TI_LMP91000_BIAS_SELECTION_10_P									(0b00000110)
#define TI_LMP91000_BIAS_SELECTION_12_P									(0b00000111)
#define TI_LMP91000_BIAS_SELECTION_14_P									(0b00001000)
#define TI_LMP91000_BIAS_SELECTION_16_P									(0b00001001)
#define TI_LMP91000_BIAS_SELECTION_18_P									(0b00001010)
#define TI_LMP91000_BIAS_SELECTION_20_P									(0b00001011)
#define TI_LMP91000_BIAS_SELECTION_22_P									(0b00001100)
#define TI_LMP91000_BIAS_SELECTION_24_P									(0b00001101)

/////// MODECN   ////////// Operation MODE

///FET_SHORT
#define TI_LMP91000_FET_SHORT_DISABLED									(0b00000000)    ///Default
#define TI_LMP91000_FET_SHORT_ENABLED									(0b10000000)

///OP_MODE mode of operation
#define TI_LMP91000_OP_MODE_DEEP_SLEEP									(0b00000000)    ///Default
#define TI_LMP91000_OP_MODE_2_LEAD										(0b00000001)		///2-lead ground referred galvanic cell
#define TI_LMP91000_OP_MODE_STAND_BY									(0b00000010)		///Standby
#define TI_LMP91000_OP_MODE_3_LEAD										(0b00000011)		///3-lead amperometric cell
#define TI_LMP91000_OP_MODE_TEMP_TIA_OFF								(0b00000110)		///Temperature measurment TIA OFF
#define TI_LMP91000_OP_MODE_TEMP_TIA_ON									(0b00000111)		///Temperature measurment TIA ON


/// Functions :

typedef struct
{
    I2C_HandleTypeDef *hi2c;

    GPIO_TypeDef *menb_port;
    uint16_t menb_pin;

    uint8_t tiacn;
    uint8_t refcn;
    uint8_t modecn;

} LMP91000_Handle_t;

/* API */
HAL_StatusTypeDef LMP91000_Init(LMP91000_Handle_t *dev);
HAL_StatusTypeDef LMP91000_WriteReg(uint8_t reg, uint8_t value);



#endif /* INC_LMP91000_H_ */
