//
// Created by Administrator on 2024/4/25.
//

#include "init.h"

extern POSSPEED Pos_Speed;
char flag = 0, flag_en =0;

void main(void)
{
//
// Step 1. Initialize System Control:
    InitSysCtrl();
    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EDIS;

//
// Step 2. Initialize GPIO:
    InitEQep1Gpio();
//
// Step 3. Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
//
    DINT;
//
// Initialize PIE control registers to their default state.
// The default state is all PIE __interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
    InitPieCtrl();
//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the __interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
    InitPieVectTable();

//    EALLOW;  // This is needed to write to EALLOW protected registers
////    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
//    EDIS;    // This is needed to disable write to EALLOW protected registers

//
// Step 4. Initialize all the Device Peripherals:
//
//    InitCpuTimers();
//    ConfigCpuTimer(&CpuTimer0, 200, 100);
//    CpuTimer0Regs.TCR.all = 0x4001;
//    init_epwm_ch1_3();
    ConfigureADC();
//    SetupADCSoftware();
    EALLOW;
//    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0; //trigger on soft
//    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0;
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 0; //
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 0;
    EDIS;

    //
    // Enable the temperature sensor and give it 500 us to power up
    //
//    ASysCtl_enableTemperatureSensor();
//    DELAY_US(500);

//
// Step 5. User specific code, enable __interrupts:
// Enable CPU INT1 which is connected to CPU-Timer 0:
//
//    IER |= M_INT1;
//    IER |= M_INT3;

//
// Enable  in the PIE:
//
//    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
//    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;


//
// Enable global Interrupts and higher priority real-time debug events:
//
    EINT;   // Enable Global __interrupt INTM
    ERTM;   // Enable Global realtime __interrupt DBGM

    Pos_Speed.init(&Pos_Speed);

    init_control_para();

    EALLOW;
    GpioCtrlRegs.GPAMUX1.all &= 0xFFC0;
    GpioCtrlRegs.GPADIR.all |= 0x3F;

    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0; //set 1pwm_\R\E\S\T high
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    EDIS;

    GpioDataRegs.GPACLEAR.all = 0x3F;
    GpioDataRegs.GPCSET.bit.GPIO72 = 1;

    while(1){
        GpioDataRegs.GPASET.bit.GPIO14 = 1;
        if (flag_en == 1 ){
            flag_en = 0;
            switch (flag) {
                case 1:
                    GpioDataRegs.GPACLEAR.all = 0x3F;
                    GpioDataRegs.GPASET.all |= 0x29;
                    break;
                case 2:
                    GpioDataRegs.GPACLEAR.all = 0x3F;
                    GpioDataRegs.GPASET.all |= 0x26;
                    break;
                case 3:
                    GpioDataRegs.GPACLEAR.all = 0x3F;
                    GpioDataRegs.GPASET.all |= 0x1A;
                    break;
                default:
                    GpioDataRegs.GPACLEAR.all = 0x3F;
            }
        }

        //start conversions immediately via software, ADCA
        //
//        AdcaRegs.ADCSOCFRC1.all = 0x0003; //SOC0 and SOC1
        AdcdRegs.ADCSOCFRC1.all = 0x0003; //SOC0 and SOC1
        while(AdcdRegs.ADCINTFLG.bit.ADCINT1==0)
        {}
//        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

        Current_regA = AdcdResultRegs.ADCRESULT0; //ch D 0
        Current_regB = AdcdResultRegs.ADCRESULT1; //ch D 1
//        Current_regC = AdcaResultRegs.ADCRESULT0; //ch A 0

        Ia = (float )(Current_regA - CurrentOffsetA) / CurrentCoef;
        Ib = (float )(Current_regB - CurrentOffsetB) / CurrentCoef;
        Ic = 0 - Ia -Ib;

        Pos_Speed.calc(&Pos_Speed);

        GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
        DELAY_US(100);

    }

}
