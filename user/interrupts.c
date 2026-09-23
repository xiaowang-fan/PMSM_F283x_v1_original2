/*
 * int_fun.c
 *
 *  Created on: 18/4/2024
 *      Author: Haoyi Mu
 */

#include "../../PMSM_F283x_v1_original2/user/interrupts.h"

/* .......................................*/
#define STATE_FIND_Z        0   // 正常运行/辨识模式
#define STATE_HOMING        1   // 自动归零中
#define STATE_HOME_DONE     2   // 归零已完成
#define STATE_BACKLASH_TEST 3   //齿隙迟滞回线测试
#define STATE_PARAM_IDENT   4   //动力学参数辨识测试
#define STATE_PARAM_IDENT_SPEED_C  5//动力学辨识速度恒定模式
#define STATE_CURRENT_FRF   6  //电流环扫频
#define STATE_CURRENT_FRF_ID 7
#define STATE_NORMAL        8   //正常运行模式
#define STATE_SPEED_OSC_TEST 9
int sys_state = STATE_NORMAL;   // 系统初始状态

int start_backlash_test = 0;
int start_param_ident  = 0;
int start_param_ident_speed_c = 0;
int start_current_frf = 0;
int start_current_frf_id = 0;
float z_timer_cnt = 0;
float idnt_time_4 = 0;
float idnt_time_5 = 0;
float idnt_time_6 = 0;
int ident_dir = 1; //方向控制
/*电流环iq扫频测试*/
float FRF_TIME = 8.0f;
float FRF_F0 = 2.0f;
float FRF_F1 = 300.0f;
float frf_time = 0.0f;
float IQ_BIAS = 0.1f;
float IQ_CHIRP_AMP = 0.05f;
float chirp_freq;
float chirp_phase;

float n_target = 0.0f;
float send_time = 0.0f;
/*电流环id扫频测试*/

static Uint16 sweep_initialized = 0U;
static float current_disturbance = 0.0f;

volatile Uint16 frf_tx_state = FRF_TX_IDLE;

static float frf_iq_base = 0.0f;
static float frf_iq_ac = 0.0f;


//外部编码器
volatile Uint32 ext_encoder_raw_shadow = 0UL;
volatile Uint16 ext_encoder_data_ready = 0U;

Uint32 encoder_position = 0UL;
float encoder_position_feedback = 0.0f;
float ext_encoder_error_cnt = 0.0f;


#define FRF_IQ_BIAS       0.20f
#define FRF_IQ_AMPLITUDE  0.05f

#define FRF_MAX_TRAVEL     33.0f

volatile Uint16 scib_test_trigger = 0U;
volatile Uint16 scib_test_state = 0U;

volatile Uint16 scib_test_count = 0U;
volatile Uint16 scib_test_status = 0U;
volatile Uint16 scib_test_data[8] = {0U};

volatile Uint32 scib_test_5byte_cnt = 0UL;
volatile Uint32 scib_test_6byte_cnt = 0UL;
volatile Uint32 scib_test_other_cnt = 0UL;


#define OSC_CAPTURE_DECIM    10U
#define OSC_CAPTURE_FS       1000.0f
#define OSC_CAPTURE_N        1024U


float arm_angle_deg;


volatile Uint16 start_osc_capture  = 0U;
volatile Uint16 osc_capture_enable = 0U;
volatile Uint16 osc_capture_done   = 0U;

volatile Uint16 osc_sample_index = 0U;
volatile Uint16 osc_decim_count  = 0U;

volatile Uint16 osc_tx_overrun = 0U;

static float osc_theta_sum = 0.0f;
static float osc_speed_sum = 0.0f;
static float osc_iqref_sum = 0.0f;
static float osc_iq_sum    = 0.0f;
static float osc_arm_sum   = 0.0f;













#define CHIRP_F0     2.0f
#define CHIRP_F1     150.0f
#define CHIRP_TIME   8.0f
const float CHIRP_PHI_FACTOR = 29.10842f;
const float CHIRP_EXP_FACTOR = 0.4317488f;
float Iq_bias = 0.5f;
float Iq_chirp_amp = 0.3f;

/*动力学参数辨识测试*/
//float Iq_bias = 0.5f;       // 基础偏置电流 (根据实际情况给，让电机能稳速转在100 RPM左右)
float Iq_amp  = 0.1f;       // 核心修改：扫频交流振幅缩小到 0.1A（小锤子敲击，避免转速剧烈荡秋千）
float f_start = 5.0f;       // 起始频率 (直接从5Hz起步，完美避开0~5Hz的低频干扰)
float f_end   = 80.0f;      // 结束频率 (80Hz足够覆盖你19Hz附近的齿轮同步带谐振点了)


float gravity_comp = 0;
float K_g = 0.005;




int position_reset = 0;
uint32_t step_timer_cnt = 0;
uint16_t test_loop_cnt = 0;
/* .......................................*/
Uint32 Emergency_Flag = 0;
Uint16 SysEn = 1;// 0 means malfunction

volatile uint32_t adc_isr_count = 0;
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
float ALPHA = 0.5;//
float Ua1,Ub1,Ua2,Ub2;
float error = 0;
float A1,A2,B1,B2 = 0;
char ACR_flag = 0;
float Kp,Ki,Ess,Umax,Uimax = 0;
char flag_start_sweep = 0;
float final_n_ref = 0;
extern char sent_flag;

volatile uint16_t adc_ready_flag = 0;


float kaihuan = 0;


MOTOR identify_motor = MOTOR_DEFAULTS;


//uint32_t adc_freq = 0;

uint16_t cf_recv = 0;
uint16_t sf_recv = 0;
uint16_t df0 = 0;
uint16_t df1 = 0;
uint16_t df2 = 0;
uint16_t crc_recv = 0;
uint32_t crc_calc = 0;
uint32_t raw_temp = 0;
uint32_t raw_position;
Uint32 raw = 0;
int scib_rx_hw_error = 0;
int scib_error_flag = 0;
float Ts = 0.0001f;
float theta = 0;
float f = 0;
float U_amp = 0;

__interrupt void adca1_isr(void)
{

    adc_ready_flag = 1;

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}




__interrupt void epwm1_isr(void){

//    GpioDataRegs.GPASET.bit.GPIO14 = 1;


            /* Motor_mode */

    Single_phase_Motor_Control();
//    Dual_Phase_Motor_Control();

    EPwm1Regs.ETCLR.bit.INT = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

//    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;

}



void Dual_Phase_Motor_Control(void)
{
    if(adc_ready_flag)
        {

            Run_time_cal(&Run_time1);
            Pos_Speed.calc(&Pos_Speed);
            omega_ek = Pos_Speed.SpeedRpm_fr / 60.0f *2.0f * PI * PMSM.n ;
            float Angle = Pos_Speed.theta_elec * 2.0F * PI;


            Motor_Operation_Mode_Selection();

//            if(ext_encoder_data_ready == 1U)
//            {
//                encoder_position = ext_encoder_raw_shadow;
//                encoder_position_feedback = (float)encoder_position * 1.19842249e-5f;
//                ext_encoder_data_ready = 0U;
//            }

            arm_angle = encoder_position_feedback - external_encoder_offset;




            x_m = (Pos_Speed.theta_pos - motor_encoder_offset) * (5.0f / 720.0f);
            x_l = sqrtf(external_d * external_d + external_r * external_r - 2.0f * external_d * external_r * cosf(external_theta - arm_angle)) - external_Lo;
            /* current */

            Current_regA1 = AdcaResultRegs.ADCRESULT1; //ch D 0
            Current_regC1 = AdccResultRegs.ADCRESULT1; //ch D 1
            Current_regA2 = AdcaResultRegs.ADCRESULT0; //ch D 0
            Current_regC2 = AdccResultRegs.ADCRESULT0; //ch D 1

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

            /* VSD_tranform Iabc1,Iabc2 ---- < Id,Iq,Ix,Iy */

            VSD_Transform(I_abc1, I_abc2, Angle, &VSD_output);  //id,iq,ix,iy

            /* identify R L */

            //R_Identify(void);
            //L_Identify(void);
            //Phif_Identify(void);

//            if(sys_state == STATE_CURRENT_FRF)
//            {
//                if(sweep_initialized == 0U)
//                {
//                    frf_tx_state = FRF_TX_IDLE;
//                    g_current_sweep.result_ready_flag = 0U;
//
//                    Init_Log_Step_Sweep(
//                        &g_current_sweep,
//                        1.0f,       // 起始频率Hz
//                        150.0f,      // 终止频率Hz
//                        0.30f,      // Iq注入幅值A，先小幅测试
//                        50U,        // 对数频率点数
//                        10.0f,      // 正式测量周期
//                        3.0f,       // 暂态周期
//                        0.5f,       // 最短正式测量时间s
//                        10000.0f);  // ISR频率Hz
//
//                    sweep_initialized = 1U;
//                }
//
//                current_disturbance = Calc_Step_Log_Sweep( &g_current_sweep, Pos_Speed.SpeedRpm_fr);  // 【改动】速度输出
//
//                I_dref = 0.0f;
//                I_qref = current_disturbance;     // 【改动】Iq输入
//
//                // 最后一个频点发送完成后再退出状态
//                if((g_current_sweep.finished_flag == 1U) && (g_current_sweep.result_ready_flag == 0U) && (frf_tx_state == FRF_TX_IDLE))
//                {
//                    I_dref = 0.0f;
//                    I_qref = 0.0f;
//
//                    ACR_d.Ui = 0.0f;
//                    ACR_q.Ui = 0.0f;
//
//                    start_current_frf = 0U;
//                    sweep_initialized = 0U;
//                    g_current_sweep.finished_flag = 0U;
//
//                    sys_state = STATE_NORMAL;
//                }
//            }
//
//            if(sys_state == STATE_CURRENT_FRF)
//            {
//                /*
//                 * DSP已经完成一个频点，并且发送缓冲空闲。
//                 */
//                if((g_current_sweep.result_ready_flag == 1U) && (frf_tx_state == FRF_TX_IDLE))
//                {
//                    Val1 = g_current_sweep.result_freq;
//                    Val2 = g_current_sweep.result_mag_dB;
//                    Val3 = g_current_sweep.result_phase_deg;
//                    Val4 = g_current_sweep.A;
//                    Val5 = (float)g_current_sweep.finished_flag;
//
//                    /*
//                     * 必须最后置位，防止主循环读取到半更新数据。
//                     */
//                    frf_tx_state = FRF_TX_PENDING;
//                }
//            }

            /* FOC mode choose */





            FOC_controller(mode_ASR, mode_ACR, mode_POS);



//                 if (Run_time1.s % 2 == 1){
//        //             I_yref = 0;
//                     I_dref = 0;
//                 }else{
//        //             I_yref = 1;
//                     I_dref =1;
//                 }

            Proportional_limiting();

            send_time++;

            if(send_time >= 20)
            {
                send_time = 0;

                if (tx_ready == 0)
                {
                    Val1 = x_m;
                    Val2 = x_l;


                    tx_ready = 1;
                }

            }

            /* ua, ub, SVPWM */

            u_alpha = u_d * cosf(Angle) - u_q * sinf(Angle);
            u_beta  = u_d * sinf(Angle) + u_q * cosf(Angle);

            u_A1 = u_alpha + u_x;
            u_B1 = u_beta  - u_y;

            u_A2 = 0.866f * u_alpha + 0.5f * u_beta - 0.866f * u_x + 0.5f * u_y;
            u_B2 = - 0.5f * u_alpha + 0.866f * u_beta + 0.5f * u_x + 0.866f * u_y;


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
        //

        //    DAC_PTR[1]->DACVALS.all = (int)((Pos_Speed.SpeedRpm_fr/4000 + 0.5 ) *4095);//(int)((I_CT3R2S.Ds*0.2 +0.5)*4095) ;//
        //    DAC_PTR[2]->DACVALS.all = (int)((I_CT3R2S.Qs*0.05 +0.5)*4095) ;//(int)((I_dref*0.2 +0.5)*4095);
        //    DAC_PTR[3]->DACVALS.all = (int)((I_qref*0.05 +0.5)*4095) ;
            adc_ready_flag = 0; // 显式清零，等待下一次ADC转换完成触发

            arm_angle_deg = arm_angle * 180.0f / PI;

//            Oscillation_Capture_ISR();
//            if(ScibRegs.SCIFFTX.bit.TXFFST == 0)
//            {
                ScibRegs.SCITXBUF.all = 0x02;
//            }
        }
        else
        {
            error++;
        }

}


float frf_speed_base = 0.0f;
float frf_speed_ac = 0.0f;

float frf_pos_base = 0.0f;
float frf_pos_ac = 0.0f;

void Motor_Operation_Mode_Selection(void)
{
    if(position_reset == 1)
    {
        sys_state = STATE_FIND_Z;
        position_reset = 0;
    }

    switch(sys_state)
    {

        case STATE_FIND_Z:
            mode_POS = POS_open;
            mode_ASR = ASR_cls;
            mode_ACR = ACR_open;
            Ud = 5.0f;
            Uq = -5.0f;
            z_timer_cnt++;
            if(z_timer_cnt >= 30000)
            {
                Ud = 0.0f;
                Uq = 0.0f;
                z_timer_cnt = 0.0;
                sys_state = STATE_HOMING;
            }
            break;

        case STATE_HOMING:
            mode_POS = POS_PI;
            POS.Kp   = 3.0f;
            mode_ASR = ASR_PI;
            mode_ACR = ACR_PI;
            pos_ref = 0.0f;
//            pos_ref  = -25.0f;
            pos_feedback = arm_angle * (180.0f / PI);
            if(fabsf(pos_ref - pos_feedback ) < 0.05f && fabsf(Pos_Speed.SpeedRpm_fr) < 1.0f)
            {
                motor_encoder_offset = Pos_Speed.theta_pos;
                sys_state = STATE_HOME_DONE;
                POS.Ui    = 0.0f;
                POS.Out   = 0.0f;
                ASR.Ui    = 0.0f;
                ASR.Out   = 0.0f;
                mode_POS = POS_cls;
            }
            break;

        case STATE_HOME_DONE:

            if(start_backlash_test == 1)
            {
                sys_state = STATE_BACKLASH_TEST;
            }

            else if(start_param_ident == 1)
            {
                sys_state = STATE_PARAM_IDENT;
            }
            else if(start_param_ident_speed_c ==1)
            {
                sys_state = STATE_PARAM_IDENT_SPEED_C;
            }
            else if(start_current_frf_id == 1)
            {
                sys_state = STATE_CURRENT_FRF_ID;
            }
            else if(start_current_frf ==1)
            {
                sys_state = STATE_CURRENT_FRF;
            }
            break;

        case STATE_BACKLASH_TEST:

           mode_POS = POS_open;
           mode_ASR = ASR_PI;
           mode_ACR = ACR_PI;

           pos_feedback = Pos_Speed.theta_pos - motor_encoder_offset;

           if (test_loop_cnt >= 6)
              {
                  n_target = 0.0f;

                  n_ref = SpeedRamp_Update( &backlash_speed_ramp, n_target);

                  if (fabsf(n_ref) < 0.1f)
                  {
                      n_ref = 0.0f;
                      backlash_speed_ramp.output = 0.0f;

                      step_timer_cnt = 0;
                      test_loop_cnt = 0;
                      start_backlash_test = 0;

                      sys_state = STATE_NORMAL;
                  }

                  break;
              }


           step_timer_cnt++;

           if (step_timer_cnt < 10000)
           {
               n_target = 0.0f;
           }
           else if (step_timer_cnt < 30000)
           {
               n_target = 100.0f;
           }
           else if (step_timer_cnt < 40000)
           {
               n_target = 0.0f;
           }
           else if (step_timer_cnt < 60000)
           {
               n_target = -100.0f;
           }
           else if (step_timer_cnt < 70000)
           {
               n_target = 0.0f;
           }

           n_ref = SpeedRamp_Update( &backlash_speed_ramp, n_target);


           if (step_timer_cnt >= 70000)
           {
               step_timer_cnt = 0;
               test_loop_cnt++;
           }
           break;

        case STATE_PARAM_IDENT:
            mode_POS = POS_open;
            mode_ASR = ASR_open;
            mode_ACR = ACR_PI;

            I_dref = 0.0f;
            idnt_time_4 += Ts;

            float phi = CHIRP_PHI_FACTOR * (expf(CHIRP_EXP_FACTOR * idnt_time_4) - 1.0f);
            float chirp_signal = Iq_chirp_amp * sinf(phi);
            if(ident_dir ==1)
            {
                I_qref = Iq_bias + chirp_signal;
            }
            else
            {
                I_qref = - Iq_bias + chirp_signal;
            }
            if(I_qref > 6.0f)   I_qref = 6.0f;
            if(I_qref < -6.0f)  I_qref = -6.0f;

            if(idnt_time_4 >= CHIRP_TIME)
            {
                I_qref = 0.0f;
                start_param_ident = 0;
                idnt_time_4 = 0;
                sys_state = STATE_NORMAL;

            }
            break;

        case STATE_CURRENT_FRF:

            mode_POS = POS_open;
            mode_ASR = ASR_open;
            mode_ACR = ACR_PI;

            if(sweep_initialized == 0U)
            {
                frf_tx_state = FRF_TX_IDLE;

                g_current_sweep.result_ready_flag = 0U;
                g_current_sweep.finished_flag = 0U;

                frf_speed_base = Pos_Speed.SpeedRpm_frf;
                frf_pos_base = Pos_Speed.theta_pos;

                frf_iq_base = VSD_output.I_q;

                Init_Log_Step_Sweep(
                    &g_current_sweep,
                    102.0f,
                    102.0f,
                    FRF_IQ_AMPLITUDE,
                    3U,          /* 只测3个连续窗口 */
                    12.0f,       /* 每个窗口12周期 */
                    30.0f,       /* 最开始等待30周期 */
                    0.3f,
                    10000.0f);

                g_current_sweep.phase = 0.0f;
                g_current_sweep.current_step = 0U;
                g_current_sweep.measure_started = 0U;
                g_current_sweep.step_time_count = 0.0f;

                sweep_initialized = 1U;
            }


            frf_speed_base += 0.001256f * (Pos_Speed.SpeedRpm_frf - frf_speed_base);

            frf_speed_ac = Pos_Speed.SpeedRpm_frf - frf_speed_base;

            frf_pos_base += 0.001256f * (Pos_Speed.theta_pos - frf_pos_base);

            frf_pos_ac = Pos_Speed.theta_pos - frf_pos_base;

            frf_iq_base += 0.001256f * (VSD_output.I_q - frf_iq_base);

            frf_iq_ac = VSD_output.I_q - frf_iq_base;

//            current_disturbance = Calc_Step_Log_Sweep( &g_current_sweep, frf_iq_ac, frf_pos_ac);
            current_disturbance = Calc_Fixed_Freq_Repeat( &g_current_sweep, frf_iq_ac, frf_pos_ac, Pos_Speed.SpeedRpm_frf, arm_angle * 180.0f / PI);
            I_dref = 0.0f;

            if(g_current_sweep.finished_flag == 0U)
            {
                I_qref = FRF_IQ_BIAS + current_disturbance;
            }
            else
            {
                I_qref = 0.0f;
            }

            if((arm_angle * 180.0f / PI) >= FRF_MAX_TRAVEL)
            {
                I_dref = 0.0f;
                I_qref = 0.0f;

                ACR_d.Ui = 0.0f;
                ACR_q.Ui = 0.0f;

                g_current_sweep.is_running = 0U;
                g_current_sweep.result_ready_flag = 0U;
                g_current_sweep.finished_flag = 0U;

                frf_tx_state = FRF_TX_IDLE;

                start_current_frf = 0U;
                sweep_initialized = 0U;

                sys_state = STATE_NORMAL;
            }
            else
            {
                if((g_current_sweep.result_ready_flag == 1U) && (frf_tx_state == FRF_TX_IDLE))
                {
                    Val1 = g_current_sweep.result_freq;
                    Val2 = g_current_sweep.result_mag_dB;
                    Val3 = g_current_sweep.result_phase_deg;
                    Val4 = g_current_sweep.result_input_amp;
                    Val5 = g_current_sweep.result_output_amp;
                    Val6 = g_current_sweep.result_quality;

                    frf_tx_state = FRF_TX_PENDING;
                }

                if((g_current_sweep.finished_flag == 1U) && (g_current_sweep.result_ready_flag == 0U) && (frf_tx_state == FRF_TX_IDLE))
                {
                    I_dref = 0.0f;
                    I_qref = 0.0f;

                    ACR_d.Ui = 0.0f;
                    ACR_q.Ui = 0.0f;

                    start_current_frf = 0U;
                    sweep_initialized = 0U;

                    g_current_sweep.finished_flag = 0U;

                    sys_state = STATE_NORMAL;
                }
            }

            break;

        case STATE_CURRENT_FRF_ID:

            break;


        case STATE_PARAM_IDENT_SPEED_C:
            mode_POS = POS_open;
            mode_ASR = ASR_PI;
            mode_ACR = ACR_PI;
            I_dref = 0.0f;

            idnt_time_5 += 0.0001f;

            if (idnt_time_5 < 2.0f)       n_ref = 100.0f;
            else if (idnt_time_5 < 4.0f)  n_ref = 150.0f;
            else if (idnt_time_5 < 6.0f)  n_ref = 200.0f;
            else if (idnt_time_5 < 8.0f)  n_ref = 250.0f;
            else if (idnt_time_5 < 10.0f) n_ref = 300.0f;

            if (idnt_time_5 >= 10.0f)
            {
                n_ref = 0.0f;
                idnt_time_5 = 0.0f;
                start_param_ident_speed_c = 0;
                sys_state = STATE_NORMAL;
            }
            break;

        case STATE_SPEED_OSC_TEST:

            mode_POS = POS_open;

            mode_ASR = ASR_PI;

            mode_ACR = ACR_PI;


            break;

        case STATE_NORMAL:
//            mode_POS = POS_open;
//            mode_ASR = ASR_open;
//            mode_ACR = ACR_open;
            break;

        default:
            mode_POS = POS_PI;
            mode_ASR = ASR_PI;
            mode_ACR = ACR_PI;
            pos_feedback = Pos_Speed.theta_pos - motor_encoder_offset;
            break;
    }
}



void Single_phase_Motor_Control(void)
{

        Run_time_cal(&Run_time1); // Cal CPU run time

        Pos_Speed.calc(&Pos_Speed);
        omega_ek = Pos_Speed.SpeedRpm_fr / 60 *2 * PI * PMSM.n ;


    //    while(AdcdRegs.ADCINTFLG.bit.ADCINT1==0)
    //    {}
    ////        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    //    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    //    DELAY_US(1);


        Current_regA1 = AdcaResultRegs.ADCRESULT1; //ch D 0
        Current_regC1 = AdccResultRegs.ADCRESULT1; //ch D 1
        IA = ((float )(Current_regA1 - CurrentOffsetA1)) * 19/2048.0f;
        IC = ((float )(Current_regC1 - CurrentOffsetC1)) * 19.2/2048.0f;
        IB = 0 - IA -IC;


    //    Current_regA2 = AdcaResultRegs.ADCRESULT0; //ch D 0
    //    Current_regC2 = AdccResultRegs.ADCRESULT0; //ch D 1
    //    IA = ((float )(Current_regA2 - CurrentOffsetA2)) * 19.5/2048.0f;
    //    IC = ((float )(Current_regC2 - CurrentOffsetC2)) * 19.5/2048.0f;
    //    IB = 0 - IA -IC;
    //
        Ia = ALPHA * IA + (1 - ALPHA) * Ia_prev_filtered;
        Ib = ALPHA * IB + (1 - ALPHA) * Ib_prev_filtered;
        Ic = ALPHA * IC + (1 - ALPHA) * Ic_prev_filtered;

        Ia_prev_filtered = Ia;
        Ib_prev_filtered = Ib;
        Ic_prev_filtered = Ic;




        I_CT3R2S.AS = Ia;
        I_CT3R2S.Bs = Ib;
        I_CT3R2S.Angle = Pos_Speed.theta_elec;
        I_CT3R2S.calc(&I_CT3R2S); // current from 3R --> 2S

    //    Val1 = (float) Pos_Speed.SpeedRpm_fr;
    //    Val2 = (float) n_ref;
    //    Val3 = (float) I_dref;
    //    Val4 = (float) I_CT3R2S.Ds;
    //    Val4 = (float) VSD_output.I_d;

        tx_ready = 1;



    //    if (Run_time1.h == 0 && Run_time1.m ==0 && Run_time1.s <= 3){
    //        u_d = 0;
    //        u_q = -5; //Pre-Position
    //    }else{

            FOC_controller(mode_ASR, mode_ACR, mode_POS);

//            if (Run_time1.s % 2 == 1){
//    //            I_yref = 0;
//                I_dref = 0;
//            }else{
//    //            I_yref = 1;
//                I_dref =1;
//            }

    //    }



        Proportional_limiting();

        U_CT2S2R1.Ds = u_d;
        U_CT2S2R1.Qs = u_q;
        U_CT2S2R1.angle_syn(&U_CT2S2R1, &I_CT3R2S);
        U_CT2S2R1.calc(&U_CT2S2R1);// voltage 2S --> 2R

        SVpwm1.U_syn(&SVpwm1, &U_CT2S2R1 );
        SVpwm1.calc(&SVpwm1);
        SVpwm1.gen1(&SVpwm1);

        ScibRegs.SCITXBUF.all = 0x02;

        arm_angle = encoder_position_feedback - external_encoder_offset;
        arm_angle_deg = arm_angle * 180.0f / PI;


}


__interrupt void cpu_timer0_isr(void)
{
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    Pos_Speed.calc(&Pos_Speed);
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



//
__interrupt void scib_isr(void)
{
    cf_recv = ScibRegs.SCIRXBUF.all;
    sf_recv = ScibRegs.SCIRXBUF.all;
    df0 = ScibRegs.SCIRXBUF.all;
    df1 = ScibRegs.SCIRXBUF.all;
    df2 = ScibRegs.SCIRXBUF.all;
    crc_recv = ScibRegs.SCIRXBUF.all;
    raw_position = ((uint32_t)df2 << 16) | ((uint32_t)df1 << 8) | (uint32_t)df0;
    encoder_position = raw_position & 0x0007FFFF;
    encoder_position_feedback = (float)encoder_position / 524288.0f * 2.0f * PI;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

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



void FOC_controller(ASR_mode_ALL s_mode, ACR_mode_ALL c_mode, POS_mode_ALL p_mode)
{

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
//                POS.calc_se(&POS, &pos_ramp.Setpoint, &pos_feedback, &n_ref);
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
//                                 if (flag_start_sweep == 1)
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
//                I_dref = 0.0f;
                ASR.calc_se(&ASR, &n_ref, &Pos_Speed.SpeedRpm_fr, &I_qref);
//                gravity_comp = K_g * sin(arm_angle);
//                I_qref = I_qref + gravity_comp; // 叠加前馈补偿
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
////            // 1. 检查是否点火
//                    if (flag_start_sweep == 1)
//                    {
//                        Init_Log_Step_Sweep(&g_current_sweep, 10.0f, 700.0f, 1.2f, 100, 20.0f, 5.0f, 0.2f, 10000.0f);
//                        flag_start_sweep = 0;
//                    }
//
//                    // 用一个静态变量记录上一时刻的运行状态，用来捕捉“结束的瞬间”
//                    static int last_running_state = 0;
//
//                    // 2. 实时计算扰动
//                    float current_disturbance = Calc_Step_Log_Sweep(&g_current_sweep, VSD_output.I_d);
//
//                    // 3. 给定值分流
//                    if (g_current_sweep.is_running)
//                    {
//                        I_qref = 0.0f;
//                        I_dref = 0.0f + current_disturbance;
//                    }
//                    else
//                    {
//                        I_dref = 0.0f;
//                        I_qref = 0.0f;
//
//                        // 【核心安全保护】：捕捉扫频刚刚结束的这一个控制周期（下降沿）
//                        if (last_running_state == 1)
//                        {
//                            // 强行把电流环调节器的内部积分项清零，防止扫频残余导致停不下来
//                            // 注：请根据你实际的 TI PI 调节器结构体数变量名进行微调（通常是 .Ui 或 .i_state）
//                            ACR_d.Ui = 0.0f;
//                            ACR_q.Ui = 0.0f;
//                            ACR_d.Out = 0.0f;
//                            ACR_q.Out = 0.0f;
//
//                        }
//                    }
////
//////                     更新状态历史
//                    last_running_state = g_current_sweep.is_running;
//
//                    Val1 = (float)g_current_sweep.result_freq;
//                    Val2 = (float)g_current_sweep.result_mag_dB;
//                    Val3 = (float)g_current_sweep.result_phase_deg;


            ACR_d.calc_se(&ACR_d, &I_dref, &I_CT3R2S.Ds, &u_d);
            ACR_q.calc_se(&ACR_q, &I_qref, &I_CT3R2S.Qs, &u_q);
//            ACR_d.calc_se(&ACR_d, &I_dref, &VSD_output.I_d, &u_d);
//            ACR_q.calc_se(&ACR_q, &I_qref, &VSD_output.I_q, &u_q);
//            ACR_x.calc_se(&ACR_x, &I_xref, &VSD_output.I_x, &u_x);
//            ACR_y.calc_se(&ACR_y, &I_yref, &VSD_output.I_y, &u_y);
            break;
    }

}


static inline void Oscillation_Capture_ISR(void)
{
        Uint16 current_index;
        float inv_decim;

        float theta_sample;
        float speed_sample;
        float iqref_sample;
        float iq_sample;
        float arm_sample;

        if(osc_capture_enable == 0U)
        {
            return;
        }

        osc_theta_sum += Pos_Speed.theta_pos;
        osc_speed_sum += Pos_Speed.SpeedRpm_fr;
        osc_iqref_sum += I_qref;
        osc_iq_sum += VSD_output.I_q;

        osc_arm_sum += arm_angle * 180.0f / PI;

        osc_decim_count++;

        if(osc_decim_count < OSC_CAPTURE_DECIM)
        {
            return;
        }

        inv_decim = 1.0f / (float)OSC_CAPTURE_DECIM;

        theta_sample = osc_theta_sum * inv_decim;

        speed_sample = osc_speed_sum * inv_decim;

        iqref_sample = osc_iqref_sum * inv_decim;

        iq_sample = osc_iq_sum * inv_decim;

        arm_sample = osc_arm_sum * inv_decim;

        osc_theta_sum = 0.0f;
        osc_speed_sum = 0.0f;
        osc_iqref_sum = 0.0f;
        osc_iq_sum = 0.0f;
        osc_arm_sum = 0.0f;

        osc_decim_count = 0U;

        current_index = osc_sample_index;
        osc_sample_index++;

        if(frf_tx_state == FRF_TX_IDLE)
        {
            Val1 = (float)current_index / OSC_CAPTURE_FS;
            Val2 = theta_sample;
            Val3 = speed_sample;
            Val4 = iqref_sample;
            Val5 = iq_sample;
            Val6 = arm_sample;

            frf_tx_state = FRF_TX_PENDING;
        }
        else
        {

            osc_tx_overrun++;
        }

        if(osc_sample_index >= OSC_CAPTURE_N)
        {
            osc_capture_enable = 0U;
            osc_capture_done = 1U;
        }
}

void Start_Oscillation_Capture(void)
{
        osc_capture_enable = 0U;

        osc_sample_index = 0U;
        osc_decim_count = 0U;
        osc_tx_overrun = 0U;

        osc_theta_sum = 0.0f;
        osc_speed_sum = 0.0f;
        osc_iqref_sum = 0.0f;
        osc_iq_sum = 0.0f;
        osc_arm_sum = 0.0f;

        osc_capture_done = 0U;

        osc_capture_enable = 1U;
}

