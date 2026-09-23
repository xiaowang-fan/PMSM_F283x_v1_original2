/*
 * bsp_init.c
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/user/bsp_init.h"


/*--------------------------------------------------
 *--------------------------------------------------
 *--------------------------------------------------
 *                 change XTAL_OSC
 *--------------------------------------------------
 *--------------------------------------------------
 *--------------------------------------------------
 */
void BSP_Init(void)
{



    InitSysCtrl();  //200M

    Init_Clocks();

    Init_EPWM();
    ConfigureADC();
//    Init_can();
    Init_sci();

//    Init_LED_GPIO();
//    Init_GPIO_OUTPUT();
    Init_EQep_GPIO();
//

//    configureDAC(1);
//    configureDAC(2);
//    configureDAC(3);


//    Init_spi();
//    Init_protect_GPIO();
    Init_PieCtrl();

    /* open EPWM TBCLK */

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;
    EDIS;





}
