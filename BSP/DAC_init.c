/*
 * DAC_init.c
 *
 *  Created on: 2026Äê1ÔÂ26ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/DAC_init.h"

volatile struct DAC_REGS* DAC_PTR[4] = {0x0,&DacaRegs,&DacbRegs,&DaccRegs};


void configureDAC(Uint16 dac_num)
{
    EALLOW;
    DAC_PTR[dac_num]->DACCTL.bit.DACREFSEL  = 0;
    DAC_PTR[dac_num]->DACOUTEN.bit.DACOUTEN = 1;
    DAC_PTR[dac_num]->DACVALS.all           = 0;
    DELAY_US(10); // Delay for buffered DAC to power up
    EDIS;
}

