#ifndef K33_H_
#define K33_H_

#include "stm32h7xx_hal.h"

typedef struct {
    UART_HandleTypeDef *huart;
    uint16_t lastPPM;
} K33_HandleTypeDef;

void K33_Init(K33_HandleTypeDef *dev, UART_HandleTypeDef *huart);
<<<<<<< Updated upstream
uint16_t K33_ReadCO2(K33_HandleTypeDef *dev);

#endif /* K33_H_ */
