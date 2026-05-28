/*
 * Clocks_init.c
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/clocks_init.h"

void Init_Clocks(void)
{

    /* ePWM clock */

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EDIS;

    /* LSPCLK = SYSCLK / 2 = 100M */

    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0x1;
    EDIS;

    /* CAN clock -----XTAL */
    EALLOW;
    ClkCfgRegs.CLKSRCCTL2.bit.CANBBCLKSEL = 1;
    CpuSysRegs.PCLKCR10.bit.CAN_B = 1;
    EDIS;

}
