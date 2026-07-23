/*
 * ee_test.c
 *
 *  Created on: Jul 23, 2026
 *      Author: hosss
 */

#include "ee_test.h"
#include "at24cxx.h"

#include <string.h>
#include <stdio.h>




void EEPROM_Test(void)
{
    uint8_t tx[32];
    uint8_t rx[32];

    for(uint8_t i=0;i<32;i++)
        tx[i]=i;

    memset(rx,0,sizeof(rx));

 //   printf("\r\n");
 //   printf("------ EEPROM TEST ------\r\n");

    if(!at24_isConnected())
    {
 //       printf("EEPROM Not Found\r\n");
        EEPROM_OK = false;

        return;
    }

 //   printf("EEPROM Connected\r\n");
    EEPROM_OK = true;

    if(!at24_write(0x0000,tx,sizeof(tx),1000))
    {
   //     printf("Write Failed\r\n");
        EEPROM_OK = false;

        return;
    }

    printf("Write OK\r\n");
    EEPROM_OK = true;

    if(!at24_read(0x0000,rx,sizeof(rx),1000))
    {
        printf("Read Failed\r\n");
        EEPROM_OK = false;

        return;
    }

    printf("Read OK\r\n");

    if(memcmp(tx,rx,sizeof(tx))==0)
    {
        printf("EEPROM PASS\r\n");
        EEPROM_OK = true;
    }
    else
    {
        printf("EEPROM FAIL\r\n");
        EEPROM_OK = false;
    }
}
