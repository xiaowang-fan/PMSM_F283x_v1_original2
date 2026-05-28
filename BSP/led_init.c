/*
 * led_init.c
 *
 *  Created on: 2026Äê3ÔÂ13ÈÕ
 *      Author: 86135
 */

#include "led_init.h"

void Init_LED_GPIO(void)
{
    Init_DSP_PowerOK_LED_GPIO();
}

void Init_DSP_PowerOK_LED_GPIO(void)
{
    /* GPIO8_LED ---- output, Power-on Light Up */
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO8  = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO8  = 1;
    EDIS;

}

