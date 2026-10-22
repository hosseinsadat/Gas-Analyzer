/*
 * lmp91000_board.c
 *
 *  Created on: Dec 20, 2025
 *      Author: Manufact
 */

#include "lmp91000_board.h"
#include "LMP91000.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"



#define LMP91000_REG_TIACN   0x10
#define LMP91000_REG_REFCN   0x11
#define LMP91000_REG_MODECN  0x12

extern I2C_HandleTypeDef hi2c2;


HAL_StatusTypeDef LMP91000_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = { reg, value };
    return HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, data, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LMP91000_Init(LMP91000_Handle_t *dev)
{
    /* Enable only this LMP */
    LMP_ENABLE(dev);
    osDelay(5);

    if (LMP91000_WriteReg(LMP91000_REG_TIACN, dev->tiacn) != HAL_OK)
        return HAL_ERROR;

    if (LMP91000_WriteReg(LMP91000_REG_REFCN, dev->refcn) != HAL_OK)
        return HAL_ERROR;

    if (LMP91000_WriteReg(LMP91000_REG_MODECN, dev->modecn) != HAL_OK)
        return HAL_ERROR;

    /* Disable after config */
    LMP_DISABLE(dev);

    return HAL_OK;
}
