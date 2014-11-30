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

#include <stdbool.h>
#include <stdint.h>

#include "morse.h"

#include "inc/hw_memmap.h"
#include "inc/tm4c1294ncpdt.h"
/*#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
*/

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

uint32_t g_ui32SysClock;

struct morser m;
enum OUTPUT res; 

void
Timer0IntHandler(void) { 
  ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

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
      init_morser(&m, "emma");
      break;
  };
}

int
main(void)
{
    volatile uint32_t ui32Loop;
    char *str = "emma";
    init_morser(&m, str); 

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R12;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ui32Loop = SYSCTL_RCGCGPIO_R;
    
    // blinking two LEDs is twice as good as blinking one, hence 0x03 -- nn
    GPIO_PORTN_DIR_R = 0x03;
    GPIO_PORTN_DEN_R = 0x03;
    
    //
    // Set the clocking to run directly from the crystal at 120MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 12000000);
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1)
    {
    }
}
