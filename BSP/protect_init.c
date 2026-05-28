/*
 * protect.c
 *
 *  Created on: 2026年2月2日
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/protect_init.h"


void Init_protect_GPIO(void)
{
    Init_protect_fault_GPIO();
    Init_protect_temp_GPIO();
    Init_protect_ready_GPIO();
}

void Init_protect_fault_GPIO(void)
{
    EALLOW;
    /* GPIO22 ---- input */
    GpioCtrlRegs.GPAMUX2.bit.GPIO22   = 0;  // Set GPIO22 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO22    = 0;  // Set GPIO22 as input direction

//    GpioCtrlRegs.GPAQSEL2.bit.GPIO22  = 2; // 6次采样限定（Qualify）
//    GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0xFF; // 设置采样周期，进一步增强滤波

    InputXbarRegs.INPUT4SELECT        = 22; // Assign GPIO22 to InputXbar INPUT4 (XINT1 source)

    XintRegs.XINT1CR.bit.POLARITY     = 0;  // Falling edge
    XintRegs.XINT1CR.bit.ENABLE       = 1;
    EDIS;
}

void Init_protect_temp_GPIO(void)
{
    EALLOW;
    /* GPIO23 ---- input */
    GpioCtrlRegs.GPAMUX2.bit.GPIO23   = 0;  // Set GPIO23 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO23    = 0;  // Set GPIO23 as input direction

//    GpioCtrlRegs.GPAQSEL2.bit.GPIO23  = 2; // 6次采样限定
//    GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0xFF; // 采样周期设为最长

    InputXbarRegs.INPUT5SELECT        = 23; // Assign GPIO23 to InputXbar INPUT5 (XINT2 source)

    // 4. 配置 XINT2 触发模式
    XintRegs.XINT2CR.bit.POLARITY     = 0;  // Falling edge
    XintRegs.XINT2CR.bit.ENABLE       = 1;
    EDIS;
}

void Init_protect_ready_GPIO(void)
{
    EALLOW;
    /* GPIO24 ---- input */
    GpioCtrlRegs.GPAMUX2.bit.GPIO24   = 0;  // Set GPIO24 to general purpose I/O mode
    GpioCtrlRegs.GPADIR.bit.GPIO24    = 0;  // Set GPIO24 as input direction

//    GpioCtrlRegs.GPAQSEL2.bit.GPIO24  = 2; // 6次采样限定
//    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0xFF; // 采样周期

    InputXbarRegs.INPUT6SELECT        = 24; // Assign GPIO24 to InputXbar INPUT6 (XINT3 source)

    XintRegs.XINT3CR.bit.POLARITY     = 0;  // Falling edge
    XintRegs.XINT3CR.bit.ENABLE       = 1;
    EDIS;
}

