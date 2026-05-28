/*
 * gpio_output.c
 *
 *  Created on: 2026Äê3ÔÂ13ÈÕ
 *      Author: 86135
 */

#include "gpio_output_init.h"

void Init_GPIO_OUTPUT(void)
{
    Init_BRK_GPIO_OUTPUT();
    Init_IGBT_GPIO_OUTPUT();
    Init_FLTCLR_GPIO_OUTPUT();
}

void Init_BRK_GPIO_OUTPUT(void)
{
    /* GPIO20_BRK ---- output */
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO20  = 1;  // clear
    GpioCtrlRegs.GPAMUX2.bit.GPIO20   = 0;  // Set GPIO20 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO20    = 1;  // Set GPIO20 as output direction
    EDIS;
}

void Init_IGBT_GPIO_OUTPUT(void)
{
    /* GPIO9_IGBT ---- output */
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO9   = 1;  // clear
    GpioCtrlRegs.GPAMUX1.bit.GPIO9    = 0;  // Set GPIO9 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO9     = 1;  // Set GPIO9 as output direction
    EDIS;
}

void Init_FLTCLR_GPIO_OUTPUT(void)
{
    /* GPIO21_FLTCLR ---- output */
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO21  = 1;  // clear
    GpioCtrlRegs.GPAMUX2.bit.GPIO21   = 0;  // Set GPIO21 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO21    = 1;  // Set GPIO21 as output direction
    EDIS;
}

