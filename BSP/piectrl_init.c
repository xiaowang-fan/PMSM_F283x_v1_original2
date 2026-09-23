/*
 * piectrl_init.c
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/piectrl_init.h"

void Init_PieCtrl()
{

       DINT;
       /* disable CPU interrupts and initialize PIE control registers to their default state */

       InitPieCtrl();

       /* disable CPU __interrupts and clear all CPU __interrupt flags */

       IER = 0x0000;
       IFR = 0x0000;

       /* initialize the PIE vector table with pointers to the shell Interrupt */

        InitPieVectTable();

        /* assign interrupt vector */

        EALLOW;
        PieVectTable.ADCA1_INT = &adca1_isr;    //ADCA interrupt
        PieVectTable.EPWM1_INT = &epwm1_isr;    //EPWM1 interrupt
        PieVectTable.SCIB_RX_INT = &scib_isr;    //EPWM1 interrupt
//        PieVectTable.CANA0_INT = &cana_rx_isr;  //CANA_RX interrupt
//        PieVectTable.CANB0_INT = &canb_rx_isr;  //CANA_RX interrupt
//        PieVectTable.XINT1_INT = &fault_isr;    //fault interrupt
//        PieVectTable.XINT2_INT = &temp_isr;    //temp interrupt
//        PieVectTable.XINT3_INT = &ready_isr;    //ready interrupt
        EDIS;

        /* enable EPWM1 interrupt */

        IER |= M_INT3;
        PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

        /* enable SCIB interrupt */

        IER |= M_INT9;
        PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

        /* enable ADCA1 interrupt */

        IER |= M_INT1;
        PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

        /* enable CANA & CANB interrupt */
//
//        IER |= M_INT9;
//        PieCtrlRegs.PIEIER9.bit.INTx5 = 1;      //CANA
//        PieCtrlRegs.PIEIER9.bit.INTx7 = 1;      //CANB

        /* enable protect interrupt */

//       IER |= M_INT1;   //fault & temp
//       IER |= M_INT12;  //ready
//       PieCtrlRegs.PIEIER1.bit.INTx4 = 1;      //fault
//       PieCtrlRegs.PIEIER1.bit.INTx5 = 1;      //temp
//       PieCtrlRegs.PIEIER12.bit.INTx1 = 1;      //ready


}

