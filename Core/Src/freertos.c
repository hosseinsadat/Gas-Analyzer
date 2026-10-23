/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include <stdio.h>
#include "sensor_data.h"
#include "sht22.h"   /* make sure this header exists and declares SHT22_t and SHT22_Read / SHT22_Init */
#include "LMP91000.h"
//#include "k33.h"
#include "gas_sensor.h"
#include "gas_calibration.h"
#include "slope_offset_calib.h"
#include <string.h>
#include "ADS1015_ADS1115.h"
#include "Max66.h"

#include "ee_test.h"
#include "k33_config.h"
#include "k33_modbus.h"
#include "k33_sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern I2C_HandleTypeDef hi2c2;
//extern K33_HandleTypeDef k33;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi6;

extern uint16_t rawCO,rawO2 ;



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CO_MEMORY_ENABLE		  HAL_GPIO_WritePin(CO_MEB_GPIO_Port,CO_MEB_Pin,GPIO_PIN_RESET)
#define CO_MEMORY_DISABLE		  HAL_GPIO_WritePin(CO_MEB_GPIO_Port,CO_MEB_Pin,GPIO_PIN_SET)

////////////////////////////////////

#define O2_MEMORY_ENABLE		  HAL_GPIO_WritePin(O2_MEB_GPIO_Port,O2_MEB_Pin,GPIO_PIN_RESET)
#define O2_MEMORY_DISABLE		  HAL_GPIO_WritePin(O2_MEB_GPIO_Port,O2_MEB_Pin,GPIO_PIN_SET)

ThermoCouple sensor_th1;
	float Th1_temp = 0;
	int intpart;
	void InitThermocouple1(void) {
		sensor_th1.Thx_CS_Pin = MAX_CS_Pin;			// CS Pin
		sensor_th1.Thx_CS_Port = MAX_CS_GPIO_Port;	// CS GPIO PORT
		sensor_th1.hspi = &hspi6;						// SPI6
	}


uint8_t co_data[2];
uint8_t o2_data[2];

ADS1xx5_I2C ads;





/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
LMP91000_Handle_t lmpCO =
{
    .hi2c      = &hi2c2,
    .menb_port = GPIOB,
    .menb_pin  = GPIO_PIN_13,   // CO MENB

    .tiacn =
        TI_LMP91000_TIA_GAIN_35_K_Ohm |
        TI_LMP91000_RLAOD_10_Ohm,

    .refcn =
        TI_LMP91000_INT_Z_50 |
        TI_LMP91000_BIAS_SIGN_POSITIVE |
        TI_LMP91000_BIAS_SELECTION_10_P,

    .modecn =
        TI_LMP91000_FET_SHORT_DISABLED |
        TI_LMP91000_OP_MODE_3_LEAD
};

LMP91000_Handle_t lmpO2 =
{
    .hi2c      = &hi2c2,
    .menb_port = GPIOB,
    .menb_pin  = GPIO_PIN_14,   // O2 MENB

    .tiacn =
        TI_LMP91000_TIA_GAIN_120_K_Ohm |
        TI_LMP91000_RLAOD_10_Ohm,

    .refcn =
        TI_LMP91000_INT_Z_O2_GROUDN_REFERRED |
        TI_LMP91000_BIAS_SELECTION_0_P,   // NO bias

    .modecn =
        TI_LMP91000_FET_SHORT_DISABLED |
        TI_LMP91000_OP_MODE_3_LEAD
};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .stack_size = 8192 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for SensorTask_I2C */
osThreadId_t SensorTask_I2CHandle;
const osThreadAttr_t SensorTask_I2C_attributes = {
  .name = "SensorTask_I2C",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorTask_UART */
osThreadId_t SensorTask_UARTHandle;
const osThreadAttr_t SensorTask_UART_attributes = {
  .name = "SensorTask_UART",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorTask_SPI */
osThreadId_t SensorTask_SPIHandle;
const osThreadAttr_t SensorTask_SPI_attributes = {
  .name = "SensorTask_SPI",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CommTask_NRT_SD */
osThreadId_t CommTask_NRT_SDHandle;
const osThreadAttr_t CommTask_NRT_SD_attributes = {
  .name = "CommTask_NRT_SD",
  .stack_size = 1536 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for sensorDataMutex */
osMutexId_t sensorDataMutexHandle;
const osMutexAttr_t sensorDataMutex_attributes = {
  .name = "sensorDataMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osMutexId_t i2c2MutexHandle;
const osMutexAttr_t i2c2Mutex_attributes = {
    .name = "i2c2Mutex"
};





osMutexId_t i2cBusMutexHandle;
const osMutexAttr_t i2cBusMutex_attributes = {
  .name = "i2cBusMutex"
};




/* USER CODE END FunctionPrototypes */

void StartControlTask(void *argument);
extern void TouchGFX_Task(void *argument);
void StartSensorTask_I2C(void *argument);
void StartSensorTask_UART(void *argument);
void StartSensorTask_SPI(void *argument);
void StartCommTask_NRF_SD(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of sensorDataMutex */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

	  sensorDataMutexHandle = osMutexNew(&sensorDataMutex_attributes);
	  i2cBusMutexHandle = osMutexNew(&i2cBusMutex_attributes);

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartControlTask, NULL, &ControlTask_attributes);

  /* creation of GUI_Task */
  GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

  /* creation of SensorTask_I2C */
  SensorTask_I2CHandle = osThreadNew(StartSensorTask_I2C, NULL, &SensorTask_I2C_attributes);

  /* creation of SensorTask_UART */
  SensorTask_UARTHandle = osThreadNew(StartSensorTask_UART, NULL, &SensorTask_UART_attributes);

  /* creation of SensorTask_SPI */
  SensorTask_SPIHandle = osThreadNew(StartSensorTask_SPI, NULL, &SensorTask_SPI_attributes);

  /* creation of CommTask_NRT_SD */
  CommTask_NRT_SDHandle = osThreadNew(StartCommTask_NRF_SD, NULL, &CommTask_NRT_SD_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartControlTask */
/**
  * @brief  Function implementing the ControlTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
  /* USER CODE BEGIN StartControlTask */

  /* Infinite loop */
  for(;;)
  {




	  osDelay(1);
  }
  /* USER CODE END StartControlTask */
}

/* USER CODE BEGIN Header_StartSensorTask_I2C */



/**
* @brief Function implementing the SensorTask_I2C thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorTask_I2C */
void StartSensorTask_I2C(void *argument)
{
  /* USER CODE BEGIN StartSensorTask_I2C */
  /* Infinite loop */


	/* -------- Init ONCE -------- */




	O2_MEMORY_DISABLE;
	osDelay(1000);

	CO_MEMORY_ENABLE;
	osDelay(1000);
	///////////////////////
	co_data[0] = TI_LMP91000_LOCK_REG;
	co_data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, co_data, 2, 50);

	// Configure TIA
	co_data[0] = TI_LMP91000_TIACN_REG;
	co_data[1] = 0x18; // 7kOhm, 10Ohm load
	HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, co_data, 2, 50);

	// Configure Reference
	co_data[0] = TI_LMP91000_REFCN_REG;
	co_data[1] = 0x10; // Internal zero 20%, Bias 0%
	HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, co_data, 2, 50);

	// Set Mode
	co_data[0] = TI_LMP91000_MODECN_REG;
	co_data[1] = 0x03; // 3-lead mode
	HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, co_data, 2, 50);

	// Lock registers (optional)
	co_data[0] = TI_LMP91000_LOCK_REG;
	co_data[1] = 0x01;
	HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, co_data, 2, 50);
	/////////////////////
	CO_MEMORY_DISABLE;

	osDelay(1000);
	O2_MEMORY_ENABLE;
		osDelay(2000);

		o2_data[0] = TI_LMP91000_LOCK_REG;
		o2_data[1] = 0x00;
		HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, o2_data, 2, 50);

		o2_data[0] = TI_LMP91000_TIACN_REG;
		o2_data[1] = 0b00000101; // 2.75kΩ, 10Ω
		HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, o2_data, 2, 50);

		o2_data[0] = TI_LMP91000_REFCN_REG;
		o2_data[1] = 0b01010000; // Bias +10%, Internal zero 20%
		HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, o2_data, 2, 50);

		o2_data[0] = TI_LMP91000_MODECN_REG;
		o2_data[1] = 0x03; // 3-lead amperometric cell
		HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, o2_data, 2, 50);

		o2_data[0] = TI_LMP91000_LOCK_REG;
		o2_data[1] = 0x01;
		HAL_I2C_Master_Transmit(&hi2c2, LMP91000_I2C_Address, o2_data, 2, 50);

		O2_MEMORY_DISABLE;

	 extern osMutexId_t i2cBusMutexHandle;

        GasCalibration_Init();

        ADS1115(&ads, &hi2c2, ADS_ADDR_GND);

        SHT2x_Init(&hi2c2);
        SHT2x_SoftReset();
        osDelay(20);

        InitThermocouple1();
        char uart_buffer[100];
        float temp, hum;
        uint16_t ch0 = 0, ch1 = 0;

        float o2_pct , co_ppm ;


  for(;;)
  {

	  if (osMutexAcquire(sensorDataMutexHandle, 500) == osOK)
	  	  	      {

	  	  	        hum = SHT2x_GetRelativeHumidity(1);
	  	  	        temp = SHT2x_GetTemperature(1);


	  	  	        gSensorData.temperature = temp;
	  	  	        gSensorData.humidity    = hum;

			        osMutexRelease(sensorDataMutexHandle);
	  	  	      }

	    if (osMutexAcquire(i2cBusMutexHandle, pdMS_TO_TICKS(200)) == osOK) {
	            ch0 = ADSreadADC_SingleEnded(&ads, 0);
	            osDelay(20);
	            ch1 = ADSreadADC_SingleEnded(&ads, 1);
	            osMutexRelease(i2cBusMutexHandle);
	        }


	        if (osMutexAcquire(sensorDataMutexHandle, pdMS_TO_TICKS(50)) == osOK) {
	            gSensorData.rawCO  = ch0;
	            gSensorData.rawO2  = ch1;

	            co_ppm = CO_CalcPPM(gSensorData.rawCO);
	            o2_pct = O2_CalcPercent(gSensorData.rawO2);

	            gSensorData.co_ppm = co_ppm;
	            gSensorData.o2_percent = o2_pct;

	            g_co_x10  = (int16_t)(co_ppm  * 10.0f);
	            g_o2_x10  = (int16_t)(o2_pct  * 10.0f);

	            osMutexRelease(sensorDataMutexHandle);
	        }


	        if (osMutexAcquire(sensorDataMutexHandle, pdMS_TO_TICKS(200)) == osOK) {
	       		        ReadThermoCouple(&sensor_th1);
	       		       	  		if (sensor_th1.connected) {
	       		       	  			 gSensorData.probe_temp =sensor_th1.Thx_celcius;

	       		       	  		}else{

	       		       	  			gSensorData.probe_temp = 10.0f;
	       		       	  		}
	       			            osMutexRelease(sensorDataMutexHandle);

	       		        }


	  	  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);






      /* -------- Proper sampling period -------- */
      osDelay(pdMS_TO_TICKS(500));   // 1 Hz (perfect)
      }
  /* USER CODE END StartSensorTask_I2C */
}

/* USER CODE BEGIN Header_StartSensorTask_UART */
/**
* @brief Function implementing the SensorTask_UART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorTask_UART */
void StartSensorTask_UART(void *argument)
{
  /* USER CODE BEGIN StartSensorTask_UART */
  /* Infinite loop */

	K33_Modbus_Init(&huart4);
	K33_Sensor_Init();
	uint32_t s_co2StartTick = osKernelGetTickCount();

	    for (;;)
	    {
	        K33_CO2Reading_t co2_read;
	        K33_Status_t st = K33_ReadCO2(&co2_read);
	        if (st == K33_OK)
	        {
	            float co2_pct = CO2_CalcPercent(co2_read.value);

	            /* Startup hold: right after boot the cell is not flushed yet and the
	             * sensor reports an at/over-ceiling value (shown as 30%). Hide it as 0. */
	            if (((osKernelGetTickCount() - s_co2StartTick) < pdMS_TO_TICKS(K33_CO2_STARTUP_HOLD_MS)) &&
	                (co2_read.raw >= K33_CO2_RAW_MAX))
	            {
	                co2_read.raw = 0u;
	                co2_pct = 0.0f;
	            }

	            if (osMutexAcquire(sensorDataMutexHandle, pdMS_TO_TICKS(50)) == osOK)
	            {
	                gSensorData.rawCO2 = co2_read.raw;
	                gSensorData.co2_p = co2_pct;
	                gSensorData.co2_ppm = (uint16_t)(co2_pct * 10000.0f);
	                g_co2_x10 = (uint32_t)(gSensorData.co2_ppm * 10.0f);

	                osMutexRelease(sensorDataMutexHandle);
	            }
	        }

	        osDelay(pdMS_TO_TICKS(2000)); // K33 recommended interval
	    }
  /* USER CODE END StartSensorTask_UART */
}

/* USER CODE BEGIN Header_StartSensorTask_SPI */
/**
* @brief Function implementing the SensorTask_SPI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorTask_SPI */
void StartSensorTask_SPI(void *argument)
{
  /* USER CODE BEGIN StartSensorTask_SPI */
  /* Infinite loop */

	osDelay(pdMS_TO_TICKS(100));

	    EEPROM_Test();




  for(;;)
  {


	  //////////TEST EEPROM//////////////


	          osDelay(pdMS_TO_TICKS(30000));
  }
  /* USER CODE END StartSensorTask_SPI */
}

/* USER CODE BEGIN Header_StartCommTask_NRF_SD */
/**
* @brief Function implementing the CommTask_NRT_SD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommTask_NRF_SD */
void StartCommTask_NRF_SD(void *argument)
{
  /* USER CODE BEGIN StartCommTask_NRF_SD */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCommTask_NRF_SD */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

