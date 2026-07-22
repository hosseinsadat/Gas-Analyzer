#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "XPT2046.h"


#define ORIENTATION 2
#define READ_X 0xD0
#define READ_Y 0x90

#define TDIN1 HAL_GPIO_WritePin (T_MOSI_GPIO_Port,T_MOSI_Pin,GPIO_PIN_SET)
#define TDIN0 HAL_GPIO_WritePin (T_MOSI_GPIO_Port,T_MOSI_Pin,GPIO_PIN_RESET)

#define TCLK1 HAL_GPIO_WritePin (T_SCK_GPIO_Port,T_SCK_Pin,GPIO_PIN_SET)
#define TCLK0 HAL_GPIO_WritePin (T_SCK_GPIO_Port,T_SCK_Pin,GPIO_PIN_RESET)

#define TCS1 HAL_GPIO_WritePin (T_CS_GPIO_Port,T_CS_Pin,GPIO_PIN_SET)
#define TCS0 HAL_GPIO_WritePin (T_CS_GPIO_Port,T_CS_Pin,GPIO_PIN_RESET)

#define DOUT HAL_GPIO_ReadPin(MISO_GPIO_Port, MISO_Pin)


void TP_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN1;  
		else TDIN0;   
		num<<=1;    
		TCLK0; 
		//delay_us(20);
		DWT_Delay_us(20);
		TCLK1;		//Valid on rising edge
		DWT_Delay_us(20);

	}		 			    
} 		 
//SPI read data
// Read adc value from touch screen IC
// CMD: instruction
// Return value: the data read   
uint16_t TP_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK0;		//Pull the clock low first	 
	TDIN0; 	//Pull down the data line
	TCS0; 		//Select touch screen IC
	TP_Write_Byte(CMD);//Send command word
	//delay_us(50);// ADS7846 conversion time is up to 6us
	DWT_Delay_us(50);
	TCLK0; 	     	    
	//delay_us(20);
	DWT_Delay_us(20);
	TCLK1;		//Give 1 clock, clear BUSY
	//delay_us(20);
	DWT_Delay_us(20);
	TCLK0; 	     	    
	for(count=0;count<16;count++)//Read 16-bit data, only the upper 12 bits are valid
	{ 				  
		Num<<=1; 	 
		TCLK0;	//Falling edge valid  	    	   
		//delay_us(20);
		DWT_Delay_us(20);
 		TCLK1;
 		if(HAL_GPIO_ReadPin (T_MISO_GPIO_Port , T_MISO_Pin))
                {
                  Num++;
                }
	}  	
	Num>>=4;   	//Only the upper 12 bits are valid.
	TCS1;		//Release Chip Select	 
	return(Num);   
}



static void XPT2046_TouchSelect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_RESET);
}

static void XPT2046_TouchUnselect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_SET);
}

bool XPT2046_TouchPressed()
{
    return HAL_GPIO_ReadPin(XPT2046_IRQ_GPIO_Port, XPT2046_IRQ_Pin) == GPIO_PIN_RESET;
}

bool XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y)
{
    XPT2046_TouchSelect();

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;
    
    uint16_t temp=0;
    
    for(uint8_t i = 0; i < 16; i++)
    {
        if(!XPT2046_TouchPressed())
            break;

        nsamples++;

        //HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);  cmd_read_y
        //TP_Write_Byte (READ_Y);
        //HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);
        //temp=TP_Read_AD(0X00);
        temp=TP_Read_AD (READ_Y);
        avg_y +=temp;
        
        
        //HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        //TP_Write_Byte (READ_X);
        //HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);
        //temp=TP_Read_AD(0X00);
        temp=TP_Read_AD(READ_X);       
        avg_x +=temp;
        //avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        
        //avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

    XPT2046_TouchUnselect();

    if(nsamples < 16)
        return false;

    uint32_t raw_x = (avg_x / 16);
   // *x=raw_x;
    if(raw_x < XPT2046_MIN_RAW_X) raw_x = XPT2046_MIN_RAW_X;
    if(raw_x > XPT2046_MAX_RAW_X) raw_x = XPT2046_MAX_RAW_X;

    uint32_t raw_y = (avg_y / 16);
   // *y=raw_y;
    if(raw_y < XPT2046_MIN_RAW_Y) raw_y = XPT2046_MIN_RAW_Y;
    if(raw_y > XPT2046_MAX_RAW_Y) raw_y = XPT2046_MAX_RAW_Y;

    // Uncomment this line to calibrate touchscreen:
    //printf("raw_x = %d, raw_y = %d\r\n", (int) raw_x, (int) raw_y);

   // *x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
  //  *y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);

#if (ORIENTATION == 0)
	*x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	*y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 1)
	*x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	*y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 2)
    *x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    *y = XPT2046_SCALE_Y - (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 3)
    *x = XPT2046_SCALE_X - (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    *y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#endif
    return true;
}
