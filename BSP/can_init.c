/*
 * can_init.c
 *
 *  Created on: 2026年1月27日
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/can_init.h"

volatile struct CAN_REGS *RealCanB = (struct CAN_REGS *)0x4B000;

void Init_can()
{
//    Init_canA_GPIO();
//    canA_configuration();
    Init_canB_GPIO();
    canB_configuration();
}


void Init_canA_GPIO()
{

    /* CANA_RX */

    GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_ASYNC);

    /* CANA_TX */

    GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PUSHPULL);

}

/*--------------------------------------------------
 *--------------------------------------------------
 *--------------------------------------------------
 *                 change GPIO
 *--------------------------------------------------
 *--------------------------------------------------
 *--------------------------------------------------
 */
void Init_canB_GPIO()
{

    /* CANB_RX */

    GPIO_SetupPinMux(17, GPIO_MUX_CPU1, 2);
    GPIO_SetupPinOptions(17, GPIO_INPUT, GPIO_PUSHPULL);

    /* CANB_TX */

    GPIO_SetupPinMux(12, GPIO_MUX_CPU1, 2);
    GPIO_SetupPinOptions(12, GPIO_OUTPUT, GPIO_PUSHPULL);

//    /* CANB_RX */
//
//    GPIO_SetupPinMux(39, GPIO_MUX_CPU1, 6);
//    GPIO_SetupPinOptions(39, GPIO_INPUT, GPIO_PUSHPULL);
//
//    /* CANB_TX */
//
//    GPIO_SetupPinMux(38, GPIO_MUX_CPU1, 6);
//    GPIO_SetupPinOptions(38, GPIO_OUTPUT, GPIO_PUSHPULL);

}


void canA_configuration(void)
{
    EALLOW;

    /* Initialize RAM */
    CanaRegs.CAN_RAM_INIT.all = 0x001A;
    while(CanaRegs.CAN_RAM_INIT.bit.CAN_RAM_INIT  != 0);
    while(CanaRegs.CAN_RAM_INIT.bit.RAM_INIT_DONE != 1);

    /* Set configuration mode */
    CanaRegs.CAN_CTL.bit.Init = 1;
    CanaRegs.CAN_CTL.bit.CCE  = 1;

    /* bode ----250k */
    CanaRegs.CAN_BTR.all = 0x00000509;

    /* loop */
    CanaRegs.CAN_CTL.bit.Test    = 1;
    CanaRegs.CAN_TEST.bit.LBACK  = 1;
    CanaRegs.CAN_TEST.bit.SILENT = 0;

    /* TX ---- mail3 */
    while(CanaRegs.CAN_IF1CMD.bit.Busy);
    CanaRegs.CAN_IF1ARB.all  = 0xA0040000;    //ID = 0x4
    CanaRegs.CAN_IF1MCTL.all = 0x00000088;
    CanaRegs.CAN_IF1CMD.all  = 0x00B00003;
    while(CanaRegs.CAN_IF1CMD.bit.Busy);

    /* RX ---- mail4 */
    while(CanaRegs.CAN_IF2CMD.bit.Busy);

    CanaRegs.CAN_IF2MSK.all = 0;              // Enable exact identifier matching for the CAN receive mailbox
    CanaRegs.CAN_IF2ARB.all = 0x80040000;     // MsgVal = 1, ID = 0x4, Dir = 0

    CanaRegs.CAN_IF2MCTL.bit.DLC   = 8;
    CanaRegs.CAN_IF2MCTL.bit.RxIE  = 1;
    CanaRegs.CAN_IF2MCTL.bit.EoB   = 1;
    CanaRegs.CAN_IF2MCTL.bit.UMask = 0;

    CanaRegs.CAN_IF2CMD.all = 0x00F80004;
    while(CanaRegs.CAN_IF2CMD.bit.Busy);

    /* Exit initialization mode and enable interrupts */
    CanaRegs.CAN_CTL.all        = 0x00001482;
    CanaRegs.CAN_GLB_INT_EN.all = 1;

    EDIS;
}


void canB_configuration(void)
{
    EALLOW;

    /* Initialize RAM */
    CanbRegs.CAN_RAM_INIT.all = 0x001A;
    while(CanbRegs.CAN_RAM_INIT.bit.CAN_RAM_INIT  != 0);
    while(CanbRegs.CAN_RAM_INIT.bit.RAM_INIT_DONE != 1);

    /* Set configuration mode */
    CanbRegs.CAN_CTL.bit.Init = 1;
    CanbRegs.CAN_CTL.bit.CCE  = 1;

    /* bode ----250k */
    CanbRegs.CAN_BTR.all = 0x00000509;

    /* loop */
    CanbRegs.CAN_CTL.bit.Test    = 1;
    CanbRegs.CAN_TEST.bit.LBACK  = 1;
    CanbRegs.CAN_TEST.bit.SILENT = 0;

    /* TX ---- mail3 */
    while(CanbRegs.CAN_IF1CMD.bit.Busy);
    CanbRegs.CAN_IF1ARB.all  = 0xA0040000;    //ID = 0x4
    CanbRegs.CAN_IF1MCTL.all = 0x00000088;
    CanbRegs.CAN_IF1CMD.all  = 0x00B00003;
    while(CanbRegs.CAN_IF1CMD.bit.Busy);

    /* RX ---- mail4 */
    while(CanbRegs.CAN_IF2CMD.bit.Busy);

    CanbRegs.CAN_IF2MSK.all = 0;              // Enable exact identifier matching for the CAN receive mailbox
    CanbRegs.CAN_IF2ARB.all = 0x80040000;     // MsgVal = 1, ID = 0x4, Dir = 0

    CanbRegs.CAN_IF2MCTL.bit.DLC   = 8;
    CanbRegs.CAN_IF2MCTL.bit.RxIE  = 1;
    CanbRegs.CAN_IF2MCTL.bit.EoB   = 1;
    CanbRegs.CAN_IF2MCTL.bit.UMask = 0;

    CanbRegs.CAN_IF2CMD.all = 0x00F80004;
    while(CanbRegs.CAN_IF2CMD.bit.Busy);

    /* Exit initialization mode and enable interrupts */
    CanbRegs.CAN_CTL.all        = 0x00001482;
    CanbRegs.CAN_GLB_INT_EN.all = 1;

    EDIS;
}
