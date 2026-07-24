#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "XPT2046.h"

#define ORIENTATION 2
#define READ_X 0xD0
#define READ_Y 0x90

#define TDIN1 HAL_GPIO_WritePin(T_MOSI_GPIO_Port, T_MOSI_Pin, GPIO_PIN_SET)
#define TDIN0 HAL_GPIO_WritePin(T_MOSI_GPIO_Port, T_MOSI_Pin, GPIO_PIN_RESET)

#define TCLK1 HAL_GPIO_WritePin(T_SCK_GPIO_Port, T_SCK_Pin, GPIO_PIN_SET)
#define TCLK0 HAL_GPIO_WritePin(T_SCK_GPIO_Port, T_SCK_Pin, GPIO_PIN_RESET)

#define TCS1 HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET)
#define TCS0 HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_RESET)

#define DOUT HAL_GPIO_ReadPin(MISO_GPIO_Port, MISO_Pin)

// Filter configuration constants
#define MAX_SAMPLES       8
#define MIN_VALID_SAMPLES 5
#define DEADBAND_PIXELS   3

// Static variables to persist filter state
static int32_t filt_x = -1;
static int32_t filt_y = -1;
static uint16_t last_x = 0;
static uint16_t last_y = 0;

void TP_Write_Byte(uint8_t num)    
{  
    uint8_t count = 0;
    for(count = 0; count < 8; count++)
    {
        if(num & 0x80)
        {
            TDIN1;
        }
        else
        {
            TDIN0;
        }
        num <<= 1;
        TCLK0;
        DWT_Delay_us(20);
        TCLK1;
        DWT_Delay_us(20);
    }
}

uint16_t TP_Read_AD(uint8_t CMD)	  
{ 	 
    uint8_t count = 0;
    uint16_t Num = 0;
    TCLK0;
    TDIN0;
    TCS0;
    TP_Write_Byte(CMD);
    DWT_Delay_us(50);
    TCLK0;
    DWT_Delay_us(20);
    TCLK1;
    DWT_Delay_us(20);
    TCLK0;
    for(count = 0; count < 16; count++)
    {
        Num <<= 1;
        TCLK0;
        DWT_Delay_us(20);
        TCLK1;
        if(HAL_GPIO_ReadPin(T_MISO_GPIO_Port, T_MISO_Pin))
        {
            Num++;
        }
    }
    Num >>= 4;
    TCS1;
    return Num;
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
    if (!XPT2046_TouchPressed())
    {
        // Reset filter state when the screen is released
        filt_x = -1;
        filt_y = -1;
        return false;
    }

    XPT2046_TouchSelect();

    uint16_t xs[MAX_SAMPLES];
    uint16_t ys[MAX_SAMPLES];
    uint8_t nsamples = 0;
    
    for(uint8_t i = 0; i < MAX_SAMPLES; i++)
    {
        if(!XPT2046_TouchPressed())
        {
            break;
        }

        ys[nsamples] = TP_Read_AD(READ_Y);
        xs[nsamples] = TP_Read_AD(READ_X);
        nsamples++;
    }

    XPT2046_TouchUnselect();

    if(nsamples < MIN_VALID_SAMPLES)
    {
        filt_x = -1;
        filt_y = -1;
        return false;
    }

    // Sort samples to apply median/trimmed mean filtering
    for(uint8_t i = 0; i < nsamples - 1; i++)
    {
        for(uint8_t j = i + 1; j < nsamples; j++)
        {
            if(xs[j] < xs[i])
            {
                uint16_t temp = xs[i];
                xs[i] = xs[j];
                xs[j] = temp;
            }
            if(ys[j] < ys[i])
            {
                uint16_t temp = ys[i];
                ys[i] = ys[j];
                ys[j] = temp;
            }
        }
    }

    // Calculate sum excluding the outliers (trimmed mean)
    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t valid_sum_count = 0;

    // Discard lowest and highest values
    for(uint8_t i = 1; i < nsamples - 1; i++)
    {
        avg_x += xs[i];
        avg_y += ys[i];
        valid_sum_count++;
    }

    uint32_t raw_x = avg_x / valid_sum_count;
    uint32_t raw_y = avg_y / valid_sum_count;

    // Boundary constraints
    if(raw_x < XPT2046_MIN_RAW_X) raw_x = XPT2046_MIN_RAW_X;
    if(raw_x > XPT2046_MAX_RAW_X) raw_x = XPT2046_MAX_RAW_X;
    if(raw_y < XPT2046_MIN_RAW_Y) raw_y = XPT2046_MIN_RAW_Y;
    if(raw_y > XPT2046_MAX_RAW_Y) raw_y = XPT2046_MAX_RAW_Y;

    // Coordinate conversion based on Orientation
    uint16_t screen_x = 0;
    uint16_t screen_y = 0;

#if (ORIENTATION == 0)
    screen_x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    screen_y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 1)
    screen_x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    screen_y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 2)
    screen_x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    screen_y = XPT2046_SCALE_Y - (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#elif (ORIENTATION == 3)
    screen_x = XPT2046_SCALE_X - (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    screen_y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
#endif

    // EMA Filter implementation: Out = (3 * Prev + New) / 4
    if(filt_x < 0)
    {
        filt_x = screen_x;
        filt_y = screen_y;
    }
    else
    {
        filt_x = (3 * filt_x + screen_x) >> 2;
        filt_y = (3 * filt_y + screen_y) >> 2;
    }

    // Deadband threshold logic to prevent jitter
    uint16_t out_x = (uint16_t)filt_x;
    uint16_t out_y = (uint16_t)filt_y;

    if(abs((int32_t)out_x - (int32_t)last_x) < DEADBAND_PIXELS)
    {
        out_x = last_x;
    }
    if(abs((int32_t)out_y - (int32_t)last_y) < DEADBAND_PIXELS)
    {
        out_y = last_y;
    }

    last_x = out_x;
    last_y = out_y;

    *x = out_x;
    *y = out_y;

    return true;
}
