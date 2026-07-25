#include "k33.h"

static const uint8_t K33_REQ_CMD[7] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25};

void K33_Init(K33_HandleTypeDef *dev, UART_HandleTypeDef *huart)
{
    if (dev == NULL || huart == NULL) {
        return;
    }
    dev->huart = huart;
    dev->lastPPM = 0;
}

<<<<<<< Updated upstream
uint16_t K33_ReadCO2(K33_HandleTypeDef *dev)
=======
uint16_t K33_ReadCO2_normal(K33_HandleTypeDef *dev)
>>>>>>> Stashed changes
{
    uint8_t rxBuffer[7] = {0};

    if (dev == NULL || dev->huart == NULL) {
        return 0;
    }

    __HAL_UART_FLUSH_DRREGISTER(dev->huart);

    if (HAL_UART_Transmit(dev->huart, (uint8_t *)K33_REQ_CMD, 7, 100) == HAL_OK) {
        if (HAL_UART_Receive(dev->huart, rxBuffer, 7, 200) == HAL_OK) {
            if (rxBuffer[0] == 0xFE && rxBuffer[1] == 0x44) {
                dev->lastPPM = ((uint16_t)rxBuffer[3] << 8) | rxBuffer[4];
                return dev->lastPPM;
            }
        }
    }

    return dev->lastPPM;
}
