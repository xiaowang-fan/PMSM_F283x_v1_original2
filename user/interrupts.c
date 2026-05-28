/*
 * int_fun.c
 *
 *  Created on: 18/4/2024
 *      Author: Haoyi Mu
 */

#include "../../PMSM_F283x_v1_original2/user/interrupts.h"

Uint32 Emergency_Flag = 0;
Uint16 SysEn = 1;// 0 means malfunction


// Globals

Uint16 Interrupt_Count = 0;
Uint16 sensorSample = 0, sensorTemp = 0;

VSD VSD_output;

Uint16 speed_step_con=0;
char flag_speed_step = 0;

char N_SpeedLoop = 0;
char N_PositionLoop = 0;

float aa = 14.45F;

float QR_lim = 0.5;

float Ud, Uq = 0;
float I_d,I_q,I_x,I_y = 0;
float Ia1,Ib1,Ic1 = 0;
float Ia2,Ib2,Ic2 = 0;
float Ia_prev_filtered = 0, Ib_prev_filtered = 0, Ic_prev_filtered = 0;
float Ia2_prev_filtered = 0, Ib2_prev_filtered = 0, Ic2_prev_filtered = 0;
float ALPHA = 0.61;//0.3857493;//500Hz_LP
float u_alpha,u_beta,u_A1,u_B1,u_A2,u_B2,u_x,u_y = 0;
float Ua1,Ub1,Ua2,Ub2;
float error = 0;
float A1,A2,B1,B2 = 0;
char ACR_flag = 0;
float Kp,Ki,Ess,Umax,Uimax = 0;
char flag_start_sweep = 0;
float final_n_ref = 0;
extern char sent_flag;

__interrupt void epwm1_isr(void){
    GpioDataRegs.GPASET.bit.GPIO14 = 1;
    Run_time_cal(&Run_time1); // Cal CPU run time

    Pos_Speed.calc(&Pos_Speed);
    omega_ek = Pos_Speed.SpeedRpm_fr / 60 *2 * PI * PMSM.n ;

//     check EOC
    float Angle = Pos_Speed.theta_elec * 2.0F * PI;
        while(AdcaRegs.ADCINTFLG.bit.ADCINT1==0)
        {}

        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
//    if(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1) {
//        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
//        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        error++;




//    DELAY_US(1);

    Current_regA1 = AdcaResultRegs.ADCRESULT1; //ch D 0
    Current_regC1 = AdccResultRegs.ADCRESULT1; //ch D 1
    Current_regA2 = AdcaResultRegs.ADCRESULT0; //ch D 0
    Current_regC2 = AdccResultRegs.ADCRESULT0; //ch D 1

//    I_abc1[0] = ((float )(Current_regA1 - CurrentOffsetA1)) * 19/2048.0f; //Ia1  //19
//    I_abc1[2] = ((float )(Current_regC1 - CurrentOffsetC1)) * 19.2/2048.0f; //Ib1
//    I_abc1[1] = 0 - Ia1 - Ic1;                                                   //Ic1
//
//    I_abc2[0] = ((float )(Current_regA2 - CurrentOffsetA2)) * 19.5/2048.0f; //Ia2
//    I_abc2[2] = ((float )(Current_regC2 - CurrentOffsetC2)) * 19.5/2048.0f; //Ib2
//    I_abc2[1] = 0 - Ia2 - Ic2;


    Ia1 = ((float )(Current_regA1 - CurrentOffsetA1)) * 19/2048.0f; //Ia1  //19
    Ic1 = ((float )(Current_regC1 - CurrentOffsetC1)) * 19.2/2048.0f; //Ib1
    Ib1 = 0 - Ia1 - Ic1;                                                   //Ic1

    Ia2 = ((float )(Current_regA2 - CurrentOffsetA2)) * 19.5/2048.0f; //Ia2
    Ic2 = ((float )(Current_regC2 - CurrentOffsetC2)) * 19.5/2048.0f; //Ib2
    Ib2 = 0 - Ia2 - Ic2;

    I_abc1[0] = ALPHA * Ia1 + (1 - ALPHA) * Ia_prev_filtered;
    I_abc1[1] = ALPHA * Ib1 + (1 - ALPHA) * Ib_prev_filtered;
    I_abc1[2] = ALPHA * Ic1 + (1 - ALPHA) * Ic_prev_filtered;
    I_abc2[0] = ALPHA * Ia2 + (1 - ALPHA) * Ia2_prev_filtered;
    I_abc2[1] = ALPHA * Ib2 + (1 - ALPHA) * Ib2_prev_filtered;
    I_abc2[2] = ALPHA * Ic2 + (1 - ALPHA) * Ic2_prev_filtered;

    Ia_prev_filtered = I_abc1[0];
    Ib_prev_filtered = I_abc1[1];
    Ic_prev_filtered = I_abc1[2];
    Ia2_prev_filtered = I_abc2[0];
    Ib2_prev_filtered = I_abc2[1];
    Ic2_prev_filtered = I_abc2[2];


    VSD_Transform(I_abc1, I_abc2, Angle, &VSD_output);  //id,iq,ix,iy
//            mesg[0] = (float) 0.0;
//            mesg[1] = (float) 1.0;
//            mesg[2] = (float) 2.0;
//            mesg[3] = (float) 3.0;
//            mesg[4] = (float) 4.0;
//            mesg[5] = (float) 5.0;
//            mesg[6] = (float) 6.0;
//            mesg[7] = (float) 7.0;
//            mesg[8] = (float) 8.0;
//    mesg[0] = (float) n_ref;
//    mesg[1] = (float) Pos_Speed.SpeedRpm_fr;
//    mesg[2] = (float) I_dref;
//    mesg[3] = (float) VSD_output.I_d;
//    mesg[4] = (float) I_yref;
//    mesg[5] = (float) VSD_output.I_y;
//    mesg[6] = (float)u_d; // previous moment ud
//    mesg[7] = (float)u_q; // previous moment uq
//    mesg[8] = (float)ACR_d.Ui; // previous moment uq


//
//    if (pos_flag == 1)
//    {
//        pos_offset = Pos_Speed.theta_pos;
//        pos_flag = 0 ;
//    }
//    pos_feedback = Pos_Speed.theta_pos - pos_offset;


    FOC_controller(mode_ASR, mode_ACR, mode_POS);

//
//         if (Run_time1.s % 2 == 1){
//             I_yref = 0;
//             I_dref = 0;
//         }else{
//             I_yref = 1;
//             I_dref =1;
//         }

    Proportional_limiting();

    u_alpha = u_d * cosf(Angle) - u_q * sinf(Angle);
    u_beta  = u_d * sinf(Angle) + u_q * cosf(Angle);

    u_A1 = u_alpha + u_x;
    u_B1 = u_beta  - u_y;

    u_A2 = 0.5 * (1.732 * u_alpha + u_beta - 1.732 * u_x + u_y);
    u_B2 = 0.5 * (-u_alpha + 1.732 * u_beta + u_x + 1.732 * u_y);

    U_CT2S2R1.Alpha = u_A1;
    U_CT2S2R1.Beta  = u_B1;
    U_CT2S2R2.Alpha = u_A2;
    U_CT2S2R2.Beta  = u_B2;

    SVpwm1.U_syn(&SVpwm1, &U_CT2S2R1);
    SVpwm1.calc(&SVpwm1);
    SVpwm1.gen1(&SVpwm1);

    SVpwm2.U_syn(&SVpwm2, &U_CT2S2R2);
    SVpwm2.calc(&SVpwm2);
    SVpwm2.gen2(&SVpwm2);


    DAC_PTR[1]->DACVALS.all = (int)((Pos_Speed.SpeedRpm_fr/4000 + 0.5 ) *4095);//(int)((I_CT3R2S.Ds*0.2 +0.5)*4095) ;//
    DAC_PTR[2]->DACVALS.all = (int)((I_CT3R2S.Qs*0.05 +0.5)*4095) ;//(int)((I_dref*0.2 +0.5)*4095);
    DAC_PTR[3]->DACVALS.all = (int)((I_qref*0.05 +0.5)*4095) ;


    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;


    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
}
//

//__interrupt void epwm1_isr(void){
//    GpioDataRegs.GPASET.bit.GPIO14 = 1;
//    Run_time_cal(&Run_time1); // Cal CPU run time
//
//    Pos_Speed.calc(&Pos_Speed);
//    omega_ek = Pos_Speed.SpeedRpm_fr / 60 *2 * PI * PMSM.n ;
//
//
////    while(AdcdRegs.ADCINTFLG.bit.ADCINT1==0)
////    {}
//////        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
////    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
////    DELAY_US(1);
//
//
//    Current_regA1 = AdcaResultRegs.ADCRESULT1; //ch D 0
//    Current_regC1 = AdccResultRegs.ADCRESULT1; //ch D 1
//
//    Ia = ((float )(Current_regA1 - CurrentOffsetA1)) * 19/2048.0f;
//    Ic = ((float )(Current_regC1 - CurrentOffsetC1)) * 19.2/2048.0f;
//    Ib = 0 - Ia -Ic;
//
//    I_CT3R2S.AS = Ia;
//    I_CT3R2S.Bs = Ib;
//    I_CT3R2S.Angle = Pos_Speed.theta_elec;
//    I_CT3R2S.calc(&I_CT3R2S); // current from 3R --> 2S
//
////        mesg[0] = (float) I_dref;
////        mesg[1] = (float) I_CT3R2S.Ds;
//////        mesg[0] = (float) final_n_ref;
//////        mesg[1] = (float) Pos_Speed.SpeedRpm_fr;
////        mesg[2] = (float) g_current_sweep.freq_current;
////        mesg[3] = (float) I_CT3R2S.Ds;
////        mesg[4] = (float) I_CT3R2S.Qs;
////        mesg[5] = (float) ACR_ESO_q.z2;
////        mesg[6] = (float) I_CT3R2S.Angle;
////        mesg[7] = (float) ASR_ESO.z2;
////        mesg[8] = (float)u_d; // previous moment ud
//
//
//
//
//    DAC_PTR[1]->DACVALS.all = (int)((Pos_Speed.SpeedRpm_fr/4000 + 0.5 ) *4095);//(int)((I_CT3R2S.Ds*0.2 +0.5)*4095) ;//
//    DAC_PTR[2]->DACVALS.all = (int)((I_CT3R2S.Qs*0.05 +0.5)*4095) ;//(int)((I_dref*0.2 +0.5)*4095);
//    DAC_PTR[3]->DACVALS.all = (int)((I_qref*0.05 +0.5)*4095) ;
//
//    if (Run_time1.h == 0 && Run_time1.m ==0 && Run_time1.s <= 3){
//        u_d = 0;
//        u_q = -5; //Pre-Position
//    }else{
//
//        FOC_controller(mode_ASR, mode_ACR, mode_POS);
//
////        if (Run_time1.s % 2 == 1){
//////            I_yref = 0;
////            I_dref = 0;
////        }else{
//////            I_yref = 1;
////            I_dref =1;
////        }
//
//    }
//
//
//
//    Proportional_limiting();
//    U_CT2S2R1.Ds = u_d;
//    U_CT2S2R1.Qs = u_q;
//    U_CT2S2R1.angle_syn(&U_CT2S2R1, &I_CT3R2S);
//    U_CT2S2R1.calc(&U_CT2S2R1);// voltage 2S --> 2R
//
//    SVpwm1.U_syn(&SVpwm1, &U_CT2S2R1 );
//    SVpwm1.calc(&SVpwm1);
//    SVpwm1.gen1(&SVpwm1);
//
//    EPwm1Regs.ETCLR.bit.INT = 1;
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
//
//    aa += 0.1;
//
//    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
//}



__interrupt void cpu_timer0_isr(void)
{
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    Pos_Speed.calc(&Pos_Speed);
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



void Run_time_cal(Run_time *p){
    p->tick +=  1;
    if (p->tick == 10000){
        p->tick = 0;
        p->s += 1;

        if (p->s == 60){
            p->s = 0;
            p->m += 1;

            if (p->m == 60){
                p->m = 0;
                p->h += 1;
            }
        }
    }
}

void FOC_controller(ASR_mode_ALL s_mode, ACR_mode_ALL c_mode, POS_mode_ALL p_mode){

    if (s_mode == ASR_DP_ESO || s_mode == ASR_DP_ESO_FW || s_mode == ASR_DP_ESO_QR \
        || s_mode == ASR_DP_ESO_QR_FW || s_mode == ASR_DP_ESO_QR_FW_2 || c_mode == ACR_Obs_test){
        ASR_ESO.cal_p2(&ASR_ESO, &ESO_lambda_w);
        ASR_ESO.calc_we(&ASR_ESO, &PMSM, &I_CT3R2S.Qs, &omega_ek);
    }

    if (c_mode == ACR_DP_ESO || s_mode == ASR_DP_ESO_FW || c_mode == ACR_DP_ESO_QR \
            || s_mode == ASR_DP_ESO_QR_FW_2 || c_mode == ACR_Obs_test){
        //// 锟斤拷 ESO 锟斤拷锟铰诧拷锟街放癸拷锟斤拷
        ACR_ESO_d.cal_p1(&ACR_ESO_d, &ACR_ESO_q, &PMSM, &ESO_lambda_c, &omega_ek);
        ACR_ESO_d.calc_id(&ACR_ESO_d, &ACR_ESO_q, &PMSM, &u_d, &omega_ek, &I_CT3R2S.Ds);
        ACR_ESO_q.calc_iq(&ACR_ESO_d, &ACR_ESO_q, &PMSM, &u_q, &omega_ek, &I_CT3R2S.Qs);
    }

    if (c_mode == ACR_ULM_DP_ESO){ //锟斤拷锟街诧拷模锟酵诧拷锟斤拷
        ACR_ESO_d.cal_p2(&ACR_ESO_d, &ESO_lambda_c);
        ACR_ESO_q.cal_p2(&ACR_ESO_q, &ESO_lambda_c);

        ACR_ESO_d.cal_ULM(&ACR_ESO_d);
        ACR_ESO_q.cal_ULM(&ACR_ESO_q);
    }
    if (s_mode == ASR_DP_ESO_QR ||s_mode == ASR_DP_ESO_QR_FW){ //
        ASR_QR.w = Pos_Speed.SpeedRpm_fr/6.0 *2*PI ; // np/60 *2  2锟斤拷锟斤拷锟斤拷  默锟较诧拷锟斤拷480rpm operation point
        ASR_QR.para(&ASR_QR);
        float QR_temp;
        QR_temp = (n_ref - Pos_Speed.SpeedRpm_fr) *0.10472;//0.10472 = 2PI /60
        Limiting_amplitude(&QR_temp,QR_lim);
        ASR_QR.in1 =  QR_temp;
        ASR_QR.cal(&ASR_QR);
    }

    if (c_mode == ACR_DP_ESO_QR){
        ACR_d_QR.w = Pos_Speed.SpeedRpm_fr/6.0 *6*PI ;
        ACR_d_QR.w = 2/ACR_d_QR.Ts *tanf(ACR_d_QR.w *ACR_d_QR.Ts/2);
        ACR_d_QR.para(&ACR_d_QR);
        float QR_temp1, QR_temp2;
        QR_temp1 = (I_dref - I_CT3R2S.Ds);
        Limiting_amplitude(&QR_temp1,QR_lim);
        ACR_d_QR.in1 = QR_temp1;
        ACR_d_QR.cal(&ACR_d_QR);

        ACR_q_QR.zeta1 = ACR_d_QR.zeta1;
        ACR_q_QR.zeta2 = ACR_d_QR.zeta2;

        ACR_q_QR.w = ACR_d_QR.w;
        ACR_q_QR.para(&ACR_q_QR);
        QR_temp2 = (I_qref - I_CT3R2S.Qs);
        Limiting_amplitude(&QR_temp2,QR_lim);
        ACR_q_QR.in1 = QR_temp2;
        ACR_q_QR.cal(&ACR_q_QR);

    }

    if (N_PositionLoop == POSITION_LOOP){
        N_PositionLoop = 0;
        switch (p_mode) {
            case POS_cls:
                pos_ref = Pos_Speed.theta_pos;
                break;
            case POS_open:
                break;
            case POS_PI:
                POS.calc_se(&POS, &pos_ref, &pos_feedback, &n_ref);
                break;
        }
    }


    if (N_SpeedLoop == SPEED_LOOP){
        N_SpeedLoop = 0;
        switch (s_mode) {
            case ASR_cls:
//                I_dref = 0;
//                I_qref = 0;
                break;
            case ASR_open:
                break;
            case ASR_PI:

//                // 1. 检查你是否在 CCS Expressions 窗口里按下了点火开关
//                if (flag_start_sweep == 1)
//                {
//                    // 初始化对数阶梯扫频：5~42Hz，幅值35rpm，25个点，每个点3秒，1000Hz采样率
//                    Init_Log_Step_Sweep(&g_speed_sweep, 5.0f, 42.0f, 35.0f, 25, 3.0f, 1000.0f);
//
//                    flag_start_sweep = 0; // 单片机吃下指令，按钮自动弹回 0
//                }
//
//                // 2. 实时计算当前 1ms 的扫频正弦扰动
//                float speed_disturbance = Calc_Step_Log_Sweep(&g_speed_sweep);
//
//                // 3. 叠加得到最终的闭环速度指令
//                final_n_ref = n_ref + speed_disturbance; // 提示：n_ref在Debug里给 0
                ASR.calc_se(&ASR, &n_ref, &Pos_Speed.SpeedRpm_fr, &I_qref);
                break;
            case ASR_DP_ESO:
                controller_DP_S_ESO(&ASR_ESO, &PMSM, &n_ref, &I_qref ,&Imax, 1e-3);
                break;
            case ASR_DP_ESO_FW:
                controller_DP_S_ESO_FW(&ACR_ESO_d, &ACR_ESO_q, &ASR_ESO, &PMSM, &ACR_id_ref_LPF,\
                        &n_ref, &I_dref, &I_qref, &Imax, &U_svMax, 1e-3);
                break;
            case ASR_DP_ESO_QR:
                controller_DP_S_ESO_QR(&ASR_ESO, &ASR_QR, &PMSM, &n_ref, &I_qref, &Imax, 1e-3);
                break;
            case ASR_DP_ESO_QR_FW:
                controller_DP_S_ESO_QR_FW(&ACR_ESO_d, &ACR_ESO_q, &ASR_ESO, &ASR_QR, &PMSM, &ACR_id_ref_LPF, \
                    &n_ref, &I_dref, &I_qref, &Imax, &U_svMax, 1e-3);
                break;
            case ASR_DP_ESO_QR_FW_2:
                controller_DP_S_ESO_QR_FW_2(&ACR_ESO_d, &ACR_ESO_q, &ASR_ESO, &ACR_d_QR, &ACR_q_QR, &PMSM, &ACR_id_ref_LPF, \
            &n_ref, &I_dref, &I_qref, &Imax, &U_svMax, 1e-3);
                break;
        }
    }



    N_SpeedLoop += 1;
    N_PositionLoop +=1;
    switch (c_mode) {
        case ACR_cls:
            u_d = 0;
            u_q = 0;
            u_x = 0;
            u_y = 0;
            break;
        case ACR_open:
//            DELAY_US(1);
            u_d = Ud;
            u_q = Uq;
            break;
        case ACR_PI:
            // 1. 检查是否点火
                    if (flag_start_sweep == 1)
                    {
                        Init_Log_Step_Sweep(&g_current_sweep, 10.0f, 400.0f, 1.2f, 40, 1.0f, 0.2f, 10000.0f);
                        flag_start_sweep = 0;
                    }

                    // 用一个静态变量记录上一时刻的运行状态，用来捕捉“结束的瞬间”
                    static int last_running_state = 0;

                    // 2. 实时计算扰动
                    float current_disturbance = Calc_Step_Log_Sweep(&g_current_sweep, VSD_output.I_d);

                    // 3. 给定值分流
                    if (g_current_sweep.is_running)
                    {
                        I_qref = 0.0f;
                        I_dref = 0.0f + current_disturbance;
                    }
                    else
                    {
                        I_dref = 0.0f;
                        I_qref = 0.0f;

                        // 【核心安全保护】：捕捉扫频刚刚结束的这一个控制周期（下降沿）
                        if (last_running_state == 1)
                        {
                            // 强行把电流环调节器的内部积分项清零，防止扫频残余导致停不下来
                            // 注：请根据你实际的 TI PI 调节器结构体数变量名进行微调（通常是 .Ui 或 .i_state）
                            ACR_d.Ui = 0.0f;
                            ACR_q.Ui = 0.0f;


                            // 如果你们的 PI 库有预留的 reset 函数，也可以直接调用：
                            // ACR_d.reset(&ACR_d);
                            // ACR_q.reset(&ACR_q);
                        }
                    }

                    // 更新状态历史
                    last_running_state = g_current_sweep.is_running;


//            ACR_d.calc_se(&ACR_d, &I_dref, &I_CT3R2S.Ds, &u_d);
//            ACR_q.calc_se(&ACR_q, &I_qref, &I_CT3R2S.Qs, &u_q);
            ACR_d.calc_se(&ACR_d, &I_dref, &VSD_output.I_d, &u_d);
            ACR_q.calc_se(&ACR_q, &I_qref, &VSD_output.I_q, &u_q);
            ACR_x.calc_se(&ACR_x, &I_xref, &VSD_output.I_x, &u_x);
            ACR_y.calc_se(&ACR_y, &I_yref, &VSD_output.I_y, &u_y);
            break;
        case ACR_DP_without_observer:
            controller_DPCC_without_Observer(&I_CT3R2S, &PMSM, \
                    &omega_ek, &I_dref, &I_qref, 1e-4, &u_d, &u_q);
            break;
        case ACR_DP_ESO:
            controller_DPCC_ESO(&ACR_ESO_d, &ACR_ESO_q, &PMSM, \
         &omega_ek, &I_dref, &I_qref, 1e-4, &u_d, &u_q);
            break;
        case ACR_ULM_DP_ESO:
            controller_DPCC_ULM_ESO(&ACR_ESO_d, &ACR_ESO_q, &PMSM, \
                             &I_dref, &I_qref, 1e-4, &u_d, &u_q);
            break;
        case ACR_DP_ESO_QR:
            controller_DPCC_ESO_QR(&ACR_ESO_d, &ACR_ESO_q, &ACR_d_QR, &ACR_q_QR, &PMSM, \
         &omega_ek, &I_dref, &I_qref, 1e-4, &u_d, &u_q);
            break;
        case ACR_Obs_test:

            ACR_d.calc_se(&ACR_d, &I_dref, &I_CT3R2S.Ds, &u_d);
            ACR_q.calc_se(&ACR_q, &I_qref, &I_CT3R2S.Qs, &u_q);

            Dual_obs_id.calc(&Dual_obs_id, &PMSM, &I_CT3R2S.Ds, &I_CT3R2S.Qs, &u_d, &omega_ek);
            Dual_obs_iq.calc(&Dual_obs_iq, &PMSM, &I_CT3R2S.Ds, &I_CT3R2S.Qs, &u_q, &omega_ek);
            Dual_obs_wm.calc(&Dual_obs_wm, &PMSM, &I_CT3R2S.Qs, &omega_ek);

            ACR_ESO_d.updata(&ACR_ESO_d);
            ACR_ESO_q.updata(&ACR_ESO_q);

            break;
    }


    if (s_mode == ASR_DP_ESO || s_mode == ASR_DP_ESO_FW || s_mode == ASR_DP_ESO_QR \
        || s_mode == ASR_DP_ESO_QR_FW|| s_mode == ASR_DP_ESO_QR_FW_2 || c_mode == ACR_Obs_test){
        ASR_ESO.updata(&ASR_ESO);
    }
}

//void Proportional_limiting(){
//    float u_mag;
//    u_mag = sqrtf(u_d*u_d + u_q*u_q);
//    if(u_mag > U_svMax){
//        u_d = u_d *U_svMax /u_mag;
//        u_q = u_q *U_svMax /u_mag;
//    }
//}

void Proportional_limiting(){
    float u_mag;
    u_mag = sqrtf(u_d*u_d + u_q*u_q);
    if(u_mag > U_svMax){
        u_d = u_d *U_svMax /u_mag;
        u_q = u_q *U_svMax /u_mag;
    }

}



void speed_test_step(float start, float end, float step, Uint16 step_time,float *n, Uint16 *cont, char *flag){
    (*cont) ++;
    if ((*n) != start && (*flag) <2 ){
        (*n) = start;
        (*flag) = 2;
    }
    if ((*cont) ==  10000){
        (*cont) = 0;
        (*n) += step;
        if ((*n) >end){
            (*n) = end;
            (*flag) = 0;
        }
    }

}

__interrupt void cana_rx_isr(void)
{
    uint32_t intr_status;

    intr_status = CanaRegs.CAN_INT.all;  // read the CAN interrupt register to identify the interrupt source

    if (intr_status == 0)                // If no interrupt is pending, break out of the loop
    {

    }

    if (intr_status == 0x8000)           // Handle status interrupts (e.g., errors, TxErr, RxErr, etc.)
    {
        volatile uint32_t es_status = CanaRegs.CAN_ES.all;  // clear status interrupts
    }

    /* RX ---- mail4 */

    else if (intr_status == 4)
    {
        aa++;

        while(CanaRegs.CAN_IF2CMD.bit.Busy);

        CanaRegs.CAN_IF2CMD.bit.DIR     = 0;     // read
        CanaRegs.CAN_IF2CMD.bit.DATA_A  = 1;
        CanaRegs.CAN_IF2CMD.bit.DATA_B  = 1;
        CanaRegs.CAN_IF2CMD.bit.Control = 1;
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = 4;

        while(CanaRegs.CAN_IF2CMD.bit.Busy);

        CANA_Receive_data_Mail2.reg.L = CanaRegs.CAN_IF2DATA.all;
        CANA_Receive_data_Mail2.reg.H = CanaRegs.CAN_IF2DATB.all;

    /* if you have other mails */

    // else if (intr_status == ...) { ... }
    }

    CanaRegs.CAN_GLB_INT_CLR.all = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


__interrupt void canb_rx_isr(void)
{
    uint32_t intr_status;

    intr_status = CanbRegs.CAN_INT.all;  // read the CAN interrupt register to identify the interrupt source

    if (intr_status == 0)                // If no interrupt is pending, break out of the loop
    {

    }

    if (intr_status == 0x8000)           // Handle status interrupts (e.g., errors, TxErr, RxErr, etc.)
    {
        volatile uint32_t es_status = CanbRegs.CAN_ES.all;  // clear status interrupts
    }

    /* RX ---- mail4 */

    else if (intr_status == 4)
    {
        aa++;

        while(CanbRegs.CAN_IF2CMD.bit.Busy);

        CanbRegs.CAN_IF2CMD.bit.DIR     = 0;     // read
        CanbRegs.CAN_IF2CMD.bit.DATA_A  = 1;
        CanbRegs.CAN_IF2CMD.bit.DATA_B  = 1;
        CanbRegs.CAN_IF2CMD.bit.Control = 1;
        CanbRegs.CAN_IF2CMD.bit.MSG_NUM = 4;

        while(CanbRegs.CAN_IF2CMD.bit.Busy);

        CANB_Receive_data_Mail4.reg.L = CanbRegs.CAN_IF2DATA.all;
        CANB_Receive_data_Mail4.reg.H = CanbRegs.CAN_IF2DATB.all;

    /* if you have other mails */

    // else if (intr_status == ...) { ... }
    }

    CanbRegs.CAN_GLB_INT_CLR.all = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}




__interrupt void fault_isr(void)
{
    SysEn                   = 0; // disable system
    EPwm1Regs.TZFRC.bit.OST = 1; // disable pwm
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    Emergency_Flag          = 1; // 1:fault
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP1;
}

__interrupt void temp_isr(void)
{
    SysEn                   = 0; // disable system
    EPwm1Regs.TZFRC.bit.OST = 1; // disable pwm
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    Emergency_Flag          = 2; // 2:temp
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP1;
}


__interrupt void ready_isr(void)
{
    SysEn                   = 0; // disable system
    EPwm1Regs.TZFRC.bit.OST = 1; // disable pwm
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    Emergency_Flag          = 3; // 3:ready
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP12;
}











