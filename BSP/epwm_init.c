/*
 * epwm_init.c
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/epwm_init.h"

void Init_EPWM()
{

    /* init epwm_gpio */

    Init_EPWM1_GPIO();
    Init_EPWM2_GPIO();
    Init_EPWM3_GPIO();
//    Init_EPWM4_GPIO();
//    Init_EPWM5_GPIO();
//    Init_EPWM6_GPIO();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;



    /* init EPWM  ----EPWM1 interrupt & SOC */

    InitEPwm1();
    InitEPwm2();
    InitEPwm3();
    InitEPwm4();
    InitEPwm5();
    InitEPwm6();

    /* initialize output low for protection */

    EPwm1Regs.AQCSFRC.bit.CSFA = 1;
    EPwm1Regs.AQCSFRC.bit.CSFB = 1;
    EPwm2Regs.AQCSFRC.bit.CSFA = 1;
    EPwm2Regs.AQCSFRC.bit.CSFB = 1;
    EPwm3Regs.AQCSFRC.bit.CSFA = 1;
    EPwm3Regs.AQCSFRC.bit.CSFB = 1;
    EPwm4Regs.AQCSFRC.bit.CSFA = 1;
    EPwm4Regs.AQCSFRC.bit.CSFB = 1;
    EPwm5Regs.AQCSFRC.bit.CSFA = 1;
    EPwm5Regs.AQCSFRC.bit.CSFB = 1;
    EPwm6Regs.AQCSFRC.bit.CSFA = 1;
    EPwm6Regs.AQCSFRC.bit.CSFB = 1;

}

void Init_EPWM1_GPIO()
{

    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO0  = 1;    // EPWM1A
    GpioCtrlRegs.GPAPUD.bit.GPIO1  = 1;    // EPWM1B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // EPWM1B

    EDIS;

}


void InitEPwm1()
{

    /* setup epwm1 mode */

    EPwm1Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
    EPwm1Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR                = 0x0000;           // Clear counter

    /* setup TBCLK */

    EPwm1Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
    EPwm1Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV     = TB_DIV1;
    EPwm1Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;

    /* setup shadow CMP */

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    /* setup compare */

    EPwm1Regs.CMPA.bit.CMPA = 3000;

    /* set actions */

    EPwm1Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAD       = AQ_CLEAR;

    /* setup Deadband */

    EPwm1Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;//DB_ACTV_LO;
    EPwm1Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
    EPwm1Regs.DBRED.bit.DBRED      = DBTIME_RED;
    EPwm1Regs.DBFED.bit.DBFED      = DBTIME_FED;

    /* setup EPWM1 interrupt */

    EPwm1Regs.ETSEL.bit.INTSEL     = ET_CTR_ZERO;      // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN      = 1;                // enable INT
    EPwm1Regs.ETPS.bit.INTPRD      = ET_1ST;           // Generate INT on 1rd event
    EPwm1Regs.ETCLR.bit.INT        = 1;

    /* setup EPWM1 ADC SOC */

    EALLOW;

    EPwm1Regs.ETSEL.bit.SOCAEN     = 1;                // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL    = 2;                // Select SOC on prd
    EPwm1Regs.ETPS.bit.SOCAPRD     = 1;                // Generate pulse on 1st event

    EDIS;

}




void Init_EPWM2_GPIO()
{

    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO2  = 1;    // EPWM2A
    GpioCtrlRegs.GPAPUD.bit.GPIO3  = 1;    // EPWM2B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;    // EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;    // EPWM2B

    EDIS;

}


void InitEPwm2()
{

    EPwm2Regs.TBPRD = 10000;                       // Set timer period  Up and Down Count mode
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;


    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Setup compare
    //
    EPwm2Regs.CMPA.bit.CMPA = 4000;

    //
    // Set actions
    //
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on Zero
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    //
    // Active Low PWMs - Setup Deadband

    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;//DB_ACTV_LO;
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm2Regs.DBRED.bit.DBRED = DBTIME_RED;
        EPwm2Regs.DBFED.bit.DBFED = DBTIME_FED;
//    /* setup epwm2 mode */
//
//    EPwm2Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
//    EPwm2Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
//    EPwm2Regs.TBCTR                = 0x0000;           // Clear counter
//
//    /* setup TBCLK */
//
//    EPwm2Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
//    EPwm2Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
//    EPwm2Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
//    EPwm2Regs.TBCTL.bit.CLKDIV     = TB_DIV1;
//
//    /* setup shadow CMP */
//
//    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
//    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
//    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
//
//    /* setup compare */
//
//    EPwm2Regs.CMPA.bit.CMPA        = 3000;
//
//    /* setup actions */
//
//    EPwm2Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
//    EPwm2Regs.AQCTLA.bit.CAD       = AQ_CLEAR;
//
//    /* setup Deadband */
//
//    EPwm2Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
//    EPwm2Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;      //DB_ACTV_LO;
//    EPwm2Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
//    EPwm2Regs.DBRED.bit.DBRED      = DBTIME_RED;
//    EPwm2Regs.DBFED.bit.DBFED      = DBTIME_FED;

}


void Init_EPWM3_GPIO()
{
    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO4  = 1;    // EPWM3A
    GpioCtrlRegs.GPAPUD.bit.GPIO5  = 1;    // EPWM3B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // EPWM3B

    EDIS;
}

void InitEPwm3()
{

    /* setup epwm2 mode */

    EPwm3Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
    EPwm3Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
    EPwm3Regs.TBCTR                = 0x0000;           // Clear counter

    /* setup TBCLK */

    EPwm3Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
    EPwm3Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
    EPwm3Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

    /* setup shadow CMP */

    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    /* setup compare */

    EPwm3Regs.CMPA.bit.CMPA        = 3000;

    /* setup actions */

    EPwm3Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
    EPwm3Regs.AQCTLA.bit.CAD       = AQ_CLEAR;

    /* setup Deadband */

    EPwm3Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
    EPwm3Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;      //DB_ACTV_LO;
    EPwm3Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
    EPwm3Regs.DBRED.bit.DBRED      = DBTIME_RED;
    EPwm3Regs.DBFED.bit.DBFED      = DBTIME_FED;

}

void Init_EPWM4_GPIO()
{
    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO6  = 1;    // EPWM4A
    GpioCtrlRegs.GPAPUD.bit.GPIO7  = 1;    // EPWM4B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // EPWM4B

    EDIS;
}

void InitEPwm4()
{

    /* setup epwm2 mode */

    EPwm4Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
    EPwm4Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
    EPwm4Regs.TBCTR                = 0x0000;           // Clear counter

    /* setup TBCLK */

    EPwm4Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
    EPwm4Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
    EPwm4Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

    /* setup shadow CMP */

    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    /* setup compare */

    EPwm4Regs.CMPA.bit.CMPA        = 3000;

    /* setup actions */

    EPwm4Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
    EPwm4Regs.AQCTLA.bit.CAD       = AQ_CLEAR;

    /* setup Deadband */

    EPwm4Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;      //DB_ACTV_LO;
    EPwm4Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
    EPwm4Regs.DBRED.bit.DBRED      = DBTIME_RED;
    EPwm4Regs.DBFED.bit.DBFED      = DBTIME_FED;

}


void Init_EPWM5_GPIO()
{
    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO8  = 1;    // EPWM5A
    GpioCtrlRegs.GPAPUD.bit.GPIO9  = 1;    // EPWM5B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // EPWM5B

    EDIS;
}

void InitEPwm5()
{

    /* setup epwm2 mode */

    EPwm5Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
    EPwm5Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
    EPwm5Regs.TBCTR                = 0x0000;           // Clear counter

    /* setup TBCLK */

    EPwm5Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
    EPwm5Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
    EPwm5Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

    /* setup shadow CMP */

    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    /* setup compare */

    EPwm5Regs.CMPA.bit.CMPA        = 3000;

    /* setup actions */

    EPwm5Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
    EPwm5Regs.AQCTLA.bit.CAD       = AQ_CLEAR;

    /* setup Deadband */

    EPwm5Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
    EPwm5Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;      //DB_ACTV_LO;
    EPwm5Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
    EPwm5Regs.DBRED.bit.DBRED      = DBTIME_RED;
    EPwm5Regs.DBFED.bit.DBFED      = DBTIME_FED;

}


void Init_EPWM6_GPIO()
{
    EALLOW;

    /* disable pull_up */

    GpioCtrlRegs.GPAPUD.bit.GPIO10  = 1;    // EPWM6A
    GpioCtrlRegs.GPAPUD.bit.GPIO11  = 1;    // EPWM6B

    /* Configure EPWM-1 pins using GPIO regs */

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // EPWM6B

    EDIS;
}

void InitEPwm6()
{

    /* setup epwm2 mode */

    EPwm6Regs.TBPRD                = 10000;            // Set timer period  Up and Down Count mode
    EPwm6Regs.TBPHS.bit.TBPHS      = 0x0000;           // Phase is 0
    EPwm6Regs.TBCTR                = 0x0000;           // Clear counter

    /* setup TBCLK */

    EPwm6Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;  // Count up and down
    EPwm6Regs.TBCTL.bit.PHSEN      = TB_DISABLE;       // Disable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;          // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

    /* setup shadow CMP */

    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;        // Load registers every ZERO
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    /* setup compare */

    EPwm6Regs.CMPA.bit.CMPA        = 3000;

    /* setup actions */

    EPwm6Regs.AQCTLA.bit.CAU       = AQ_SET;           // Set PWM1A on Zero
    EPwm6Regs.AQCTLA.bit.CAD       = AQ_CLEAR;

    /* setup Deadband */

    EPwm6Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;
    EPwm6Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;      //DB_ACTV_LO;
    EPwm6Regs.DBCTL.bit.IN_MODE    = DBA_ALL;
    EPwm6Regs.DBRED.bit.DBRED      = DBTIME_RED;
    EPwm6Regs.DBFED.bit.DBFED      = DBTIME_FED;

}
