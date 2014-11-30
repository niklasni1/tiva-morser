//*****************************************************************************
//
// main.c; adapted from 
//   blinky.c, part of the TivaWare package from TI.
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"
#include "morse.h"

int
main(void)
{
    volatile uint32_t ui32Loop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R12;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ui32Loop = SYSCTL_RCGCGPIO_R;

    //
    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    // blinking two LEDs is twice as good as blinking one, hence 0x03 -- nn
    GPIO_PORTN_DIR_R = 0x03;
    GPIO_PORTN_DEN_R = 0x03;

    struct morser m;
    char *str = "emma";
    init_morser(&m, str);

    enum OUTPUT res;
    
    while(1)
    {
      res = tick(&m);
      switch(res) {
        case HIGH:
          GPIO_PORTN_DATA_R |= 0x03;
          break;
        case LOW:
          GPIO_PORTN_DATA_R &= ~(0x03);
          break;
        case END:
          GPIO_PORTN_DATA_R &= ~(0x03);
          init_morser(&m, str);
          break;
      };
      
      for(ui32Loop = 0; ui32Loop < 150000; ui32Loop++)
      {
      }
    }
}
