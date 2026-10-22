/*
 * lmp91000_board.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Manufact
 */

#ifndef INC_LMP91000_BOARD_H_
#define INC_LMP91000_BOARD_H_


#pragma once
#include "stm32h7xx_hal.h"


#define LMP_ENABLE(dev)   HAL_GPIO_WritePin((dev)->menb_port, (dev)->menb_pin, GPIO_PIN_RESET)
#define LMP_DISABLE(dev)  HAL_GPIO_WritePin((dev)->menb_port, (dev)->menb_pin, GPIO_PIN_SET)

#endif /* INC_LMP91000_BOARD_H_ */
