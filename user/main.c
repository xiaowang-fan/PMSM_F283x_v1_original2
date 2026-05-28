// Author 锟斤拷 haomy mu
// 14/4/2024

//
// Included Files
//
#include "main.h"

extern char SV_protect;

extern int Current_regA , Current_regB;

char sent_flag = 0;

float count = 0;
float aaa = 0;
float bbb = 0;

float data1,data2;


//
// Function Prototypes
//

//
// Main

//void main(void)
//{
////
//// Step 1. Initialize System Control:
//
//   InitSysCtrl();
//   EALLOW;
//   ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
//   EDIS;
//   EALLOW;
//   ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0x1;
//   EDIS;
//
////
//// Step 2. Initialize GPIO:
////   InitEQep2Gpio();
////
////   InitEPwm1Gpio();
////   InitEPwm2Gpio();
////   InitEPwm3Gpio();
//
//   GPIO_SetupPinMux(55, GPIO_MUX_CPU1, 6); //R
//   GPIO_SetupPinOptions(55, GPIO_INPUT, GPIO_PUSHPULL);
//   GPIO_SetupPinMux(54, GPIO_MUX_CPU1, 6); //T
//   GPIO_SetupPinOptions(54, GPIO_OUTPUT, GPIO_ASYNC);
//
//   GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);
//   GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);
//   GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);
//   GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_ASYNC);
//
//   GPIO_SetupPinMux(57, GPIO_MUX_CPU1, 6);
//   GPIO_SetupPinOptions(57, GPIO_INPUT, GPIO_PUSHPULL);
//   GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 6);
//   GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);
//
//
////   EALLOW;
////   GpioDataRegs.GPASET.bit.GPIO8  = 1;
////   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
////   GpioCtrlRegs.GPADIR.bit.GPIO8  = 1;
////   EDIS;
//
////
//// Step 3. Clear all __interrupts and initialize PIE vector table:
//// Disable CPU __interrupts
////
//   DINT;
////
//// Initialize PIE control registers to their default state.
//// The default state is all PIE __interrupts disabled and flags
//// are cleared.
//// This function is found in the F2837xD_PieCtrl.c file.
////
//   InitPieCtrl();
////
//// Disable CPU __interrupts and clear all CPU __interrupt flags:
////
//   IER = 0x0000;
//   IFR = 0x0000;
//
////
//// Initialize the PIE vector table with pointers to the shell Interrupt
//// Service Routines (ISR).
//// This will populate the entire table, even if the __interrupt
//// is not used in this example.  This is useful for debug purposes.
//// The shell ISR routines are found in F2837xD_DefaultIsr.c.
//// This function is found in F2837xD_PieVect.c.
////
//    InitPieVectTable();
//
//    EALLOW;  // This is needed to write to EALLOW protected registers
////    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
//    PieVectTable.EPWM1_INT = &epwm1_isr;
//    EDIS;    // This is needed to disable write to EALLOW protected registers
//
////
//// Step 4. Initialize all the Device Peripherals:
////
////    InitCpuTimers();
////    ConfigCpuTimer(&CpuTimer0, 200, 100);
////    CpuTimer0Regs.TCR.all = 0x4001;
////    init_epwm_ch1_3();
////    ConfigureADC();
////    configureDAC(1);
////    configureDAC(2);
////    configureDAC(3);
////
////    scib_fifo_init();       // Initialize the SCI FIFO
////    scib_echoback_init();   // Initialize SCI for echoback
////    scia_fifo_init();       // Initialize the SCI FIFO
////    scia_echoback_init();   // Initialize SCI for echoback
////    scic_fifo_init();       // Initialize the SCI FIFO
////    scic_echoback_init();   // Initialize SCI for echoback
//
////    SetupADCSoftware();
//    //    EALLOW;
//    //    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
//    //    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
//    //    EDIS;
//
//    //
//    // Enable the temperature sensor and give it 500 us to power up
//    //
////    ASysCtl_enableTemperatureSensor();
////    DELAY_US(500);
//
////
//// Step 5. User specific code, enable __interrupts:
//// Enable CPU INT1 which is connected to CPU-Timer 0:
////
////    IER |= M_INT1;
//    IER |= M_INT3;
//
////
//// Enable  in the PIE:
////
////    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
//    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
//
//    Pos_Speed.init(&Pos_Speed);
//
//    init_control_para();
//
//    EALLOW;
//    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0; //set 1pwm_\R\E\S\T high
//    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;
//
//    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
//    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
//    EDIS;
//    GpioDataRegs.GPCSET.bit.GPIO72 = 1;
//
//    EALLOW;
//    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;
//    EDIS;
////
//// Enable global Interrupts and higher priority real-time debug events:
////
//    EINT;   // Enable Global __interrupt INTM
//    ERTM;   // Enable Global realtime __interrupt DBGM
//
//
//
//
//
//    while(1){
//        if (SV_protect == 0 && EPwm1Regs.AQCSFRC.bit.CSFA ==1 ){ //
////            init_control_para();
//            EPwm1Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm1Regs.AQCSFRC.bit.CSFB = 0;
//            EPwm2Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm2Regs.AQCSFRC.bit.CSFB = 0;
//            EPwm3Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm3Regs.AQCSFRC.bit.CSFB = 0;
//        } else if(SV_protect == 1){
//            EPwm1Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm1Regs.AQCSFRC.bit.CSFB = 1;
//            EPwm2Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm2Regs.AQCSFRC.bit.CSFB = 1;
//            EPwm3Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm3Regs.AQCSFRC.bit.CSFB = 1;
//        }
//
////        sprintf(send_str, "\n %d \n\0", (int)(Pos_Speed.SpeedRpm_fr*10));//(int)(I_CT3R2S.AS*100),(int)(I_CT3R2S.Bs*100));
////        scib_msg(send_str);//,(int)(I_CT3R2S.AS*100)
////        DELAY_US(10);
////        mesg[0] = Pos_Speed.SpeedRpm_fr;
////        scia_msg1(mes_p,2);
////        scib_msg1(mes_p,2);
//
//
////        if(sent_flag==1){
//            scic_msg1(mes_p,22);
////            sent_flag = 0;
////        }
//
//        DELAY_US(10);
//    }
//
//}



//
//
void main(void)
{




    BSP_Init();


//
    Pos_Speed.init(&Pos_Speed);


//
    init_control_para();

//

    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0; //set 1pwm_\R\E\S\T high
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    EDIS;
    GpioDataRegs.GPCSET.bit.GPIO72 = 1;

//
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;
    EDIS;

    EINT;   // Enable Global __interrupt INTM
    ERTM;   // Enable Global realtime __interrupt DBGM


    while(1){
        if (SV_protect == 0 && EPwm1Regs.AQCSFRC.bit.CSFA ==1 ){ //
//            init_control_para();
            EALLOW;
            EPwm1Regs.AQCSFRC.bit.CSFA = 0;
            EPwm1Regs.AQCSFRC.bit.CSFB = 0;
            EPwm2Regs.AQCSFRC.bit.CSFA = 0;
            EPwm2Regs.AQCSFRC.bit.CSFB = 0;
            EPwm3Regs.AQCSFRC.bit.CSFA = 0;
            EPwm3Regs.AQCSFRC.bit.CSFB = 0;
            EPwm4Regs.AQCSFRC.bit.CSFA = 0;
            EPwm4Regs.AQCSFRC.bit.CSFB = 0;
            EPwm5Regs.AQCSFRC.bit.CSFA = 0;
            EPwm5Regs.AQCSFRC.bit.CSFB = 0;
            EPwm6Regs.AQCSFRC.bit.CSFA = 0;
            EPwm6Regs.AQCSFRC.bit.CSFB = 0;
            EDIS;
        } else if(SV_protect == 1){
            EALLOW;
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
            EDIS;
        }


//        if(sent_flag==1){
        if (g_current_sweep.result_ready_flag == 1)
        {
            // 1. 装载最新的计算结果
            mesg[0] = (float)g_current_sweep.result_freq;
            mesg[1] = (float)g_current_sweep.result_mag_dB;
            mesg[2] = (float)g_current_sweep.result_phase_deg;

            // 2. 调用你的发送函数（比如通过串口、CAN或网络发给 VOFA+）
            scic_msg1(mes_p,18);

            // 3. 【至关重要】手动将标志位清零！
            // 这样在下一个 0.8 秒结束前，main 函数都不会再进来重复发送了
            g_current_sweep.result_ready_flag = 0;
        }
//            scic_msg1(mes_p,18);
//        if(count++ >= 10000) { // 简单降频
//            CANB_Send_mesg(1.23f, 4.56f);
//            count = 0;
//        }
//            sent_flag = 0;
//        }
//        Pos_Speed.calc(&Pos_Speed);

//        CANB_Send_mesg(data1, data2);
        aaa ++;
        DELAY_US(10);
    }

}


//





