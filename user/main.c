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
volatile float Val1;
volatile float Val2;
volatile float Val3;
volatile float Val4;
volatile float Val5;
volatile float Val6;

//
//
void main(void)


{

    BSP_Init();

    Pos_Speed.init(&Pos_Speed);

    init_control_para();
    SpeedRamp_Init(&backlash_speed_ramp,0.0f, 200.0f, 200.0f, 0.0001f);

    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO72  = 0; //set 1pwm_\R\E\S\T high
    GpioCtrlRegs.GPCDIR.bit.GPIO72   = 1;

    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14   = 1;
    EDIS;
    GpioDataRegs.GPCSET.bit.GPIO72   = 1;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
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
//            EPwm4Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm4Regs.AQCSFRC.bit.CSFB = 0;
//            EPwm5Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm5Regs.AQCSFRC.bit.CSFB = 0;
//            EPwm6Regs.AQCSFRC.bit.CSFA = 0;
//            EPwm6Regs.AQCSFRC.bit.CSFB = 0;
            EDIS;
        } else if(SV_protect == 1){
            EALLOW;
            EPwm1Regs.AQCSFRC.bit.CSFA = 1;
            EPwm1Regs.AQCSFRC.bit.CSFB = 1;
            EPwm2Regs.AQCSFRC.bit.CSFA = 1;
            EPwm2Regs.AQCSFRC.bit.CSFB = 1;
            EPwm3Regs.AQCSFRC.bit.CSFA = 1;
            EPwm3Regs.AQCSFRC.bit.CSFB = 1;
//            EPwm4Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm4Regs.AQCSFRC.bit.CSFB = 1;
//            EPwm5Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm5Regs.AQCSFRC.bit.CSFB = 1;
//            EPwm6Regs.AQCSFRC.bit.CSFA = 1;
//            EPwm6Regs.AQCSFRC.bit.CSFB = 1;
            EDIS;
        }

//        if(start_osc_capture == 1U)
//        {
//            if((osc_capture_enable == 0U) && (osc_capture_done == 0U))
//            {
//                Start_Oscillation_Capture();
//
//                start_osc_capture = 0U;
//            }
//        }




//        if(frf_tx_state == FRF_TX_PENDING)
//        {
//            long safe_box[6];
//
//            frf_tx_state = FRF_TX_BUSY;
//
//            safe_box[0] = *(long *)&Val1;
//            safe_box[1] = *(long *)&Val2;
//            safe_box[2] = *(long *)&Val3;
//            safe_box[3] = *(long *)&Val4;
//            safe_box[4] = *(long *)&Val5;
//            safe_box[5] = *(long *)&Val6;
//
//            scic_msg1((char *)safe_box, 12);
//
//            g_current_sweep.result_ready_flag = 0U;
//
//            frf_tx_state = FRF_TX_IDLE;
//        }


        if(tx_ready == 1)
        {
            tx_ready = 0;
//
            if(start_backlash_test == 1)
            {
                long safe_box[5];
//
                safe_box[0] = *(long*)&Val1; // SpeedRpm_fr
                safe_box[1] = *(long*)&Val2; // I_q
                safe_box[2] = *(long*)&Val3; // I_qref
                safe_box[3] = *(long*)&Val4; // u_q
                safe_box[4] = *(long*)&Val5;

                scic_msg1((char*)safe_box, 10);
            }
        }


//        if(sent_flag==1)
//        {
//        if (g_current_sweep.result_ready_flag == 1)
//        {
//
//            // 2. 调用你的发送函数（比如通过串口、CAN或网络发给 VOFA+）
//                long safe_box[5];
////
//                safe_box[0] = *(long*)&Val1; // SpeedRpm_fr
//                safe_box[1] = *(long*)&Val2; // I_q
//                safe_box[2] = *(long*)&Val3; // I_qref
//                safe_box[3] = *(long*)&Val4; // u_q
//                safe_box[4] = *(long*)&Val5;
//
//                scic_msg1((char*)safe_box, 10);
//            // 3. 【至关重要】手动将标志位清零！
//            // 这样在下一个 0.8 秒结束前，main 函数都不会再进来重复发送了
//            g_current_sweep.result_ready_flag = 0;
//            tx_ready = 0;
//        }


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







