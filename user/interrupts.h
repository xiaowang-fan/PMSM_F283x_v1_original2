/*
 * int_fun.h
 *
 *  Created on: 18/4/2024
 *      Author: Haoyi Mu
 */

#ifndef USER_INT_FUN_H_
#define USER_INT_FUN_H_



#include "F28x_Project.h"
#include "eQEP_position_speed.h"
#include "coordinate_transformation.h"
#include "Control_PI.h"
#include "Control_ESO.h"
#include "Control_DPC.h"
#include "Control_QPR.h"
#include "Control_Dual_Obs.h"
#include <math.h>
#include <stdio.h>
#include "SVPWM.h"
#include "ADC_init.h"
#include "DAC_init.h"
#include "CAN_protocol.h"
#include "main.h"
#include "sys_globals.h"
#include "chirp.h"
#include "identify_R.h"
#include "identify_L.h"
#include "identify_Phif.h"
#include "motor_control_algorithm.h"



extern int CurrentOffsetA, CurrentOffsetB;
extern int CurrentOffsetC;
extern float CurrentCoef, CurrentCoef_p;
extern float CurrentCoef_A, CurrentCoef_B, CurrentCoef_C;
extern int CurrentOffsetAA, CurrentOffsetBB, CurrentOffsetAA2, CurrentOffsetBB2;
extern int CurrentOffsetA1, CurrentOffsetB1, CurrentOffsetC1;
extern int CurrentOffsetA2, CurrentOffsetB2, CurrentOffsetC2;
extern int CurrentOffsetC;
extern int Current_regA ,Current_regB , Current_regC;
extern int Current_regA1 ,Current_regB1 , Current_regC1;
extern int Current_regA2 ,Current_regB2 , Current_regC2;
extern float Ia, Ib, Ic;
extern float IA, IB, IC;
extern float I_abc1[3], I_abc2[3];
extern float Imax;


extern PIREG3 ASR,ACR_d,ACR_q,ACR_x,ACR_y,POS;
extern POSSPEED Pos_Speed;
extern SVPWM_2L SVpwm1;
extern SVPWM_2L SVpwm2;
extern float U_svMax;
extern char SPEED_LOOP;
extern char POSITION_LOOP;

extern ESO ASR_ESO, ACR_ESO_d, ACR_ESO_q;
extern float ESO_lambda_c, ESO_lambda_w;

extern QR ASR_QR, ACR_d_QR, ACR_q_QR;

extern Dual_O Dual_obs_id, Dual_obs_iq, Dual_obs_wm;

extern LPF ACR_id_ref_LPF;

__interrupt void cpu_timer0_isr(void);
__interrupt void epwm1_isr(void);
__interrupt void adca1_isr(void);
__interrupt void scib_isr(void);
__interrupt void cana_rx_isr(void);
__interrupt void canb_rx_isr(void);
__interrupt void fault_isr(void);
__interrupt void temp_isr(void);
__interrupt void ready_isr(void);


extern Uint32 Emergency_Flag;
extern Uint16 SysEn;// 0 means malfunction

extern float Id,Iq,Ix,Iy;


extern volatile uint32_t adc_isr_count;

extern uint32_t adc_freq;
extern uint32_t raw_position;
extern uint16_t cf_recv;
extern uint16_t sf_recv;
extern uint16_t df0;
extern uint16_t df1;
extern uint16_t df2;
extern uint16_t crc_recv0;
extern int start_step_test;
extern int start_param_ident;
extern int start_param_ident_speed_c;
extern int start_current_frf;
extern int start_current_frf_id;
extern int start_backlash_test;

#define SWEEP_PI        (3.14159265358979323846f)
#define SWEEP_RAD2DEG   (57.29577951308232f)

typedef struct
{
    /* ---------- 用户配置 ---------- */
    float f_start;
    float f_end;
    float f_step;
    float amplitude;
    float bias;
    float Ts;

    Uint16 settle_min_cycles;
    Uint16 measure_min_cycles;

    float settle_min_time_s;
    float measure_min_time_s;
    float pause_time_s;

    /* ---------- 当前频率点 ---------- */
    float current_f_cmd;
    float current_f_actual;

    float theta;
    float theta_step;

    Uint32 samples_per_cycle;
    Uint32 pause_samples;
    Uint32 settle_samples;
    Uint32 measure_samples;

    Uint32 point_sample_count;

    float sum_id_sin;
    float sum_id_cos;

    float Id_ref_inject;

    float result_f;
    float result_gain_db;
    float result_phase_deg;
    float result_id_amp;

    Uint16 data_ready;
    Uint16 is_active;
    Uint16 finished;

} Sweep_Id_t;

extern Sweep_Id_t sweep_id;

static Uint32 Sweep_MaxU32(Uint32 a, Uint32 b);



static void Sweep_LoadPoint(Sweep_Id_t *sw);

#define FRF_TX_IDLE       0U   // 没有待发送数据
#define FRF_TX_PENDING    1U   // 新结果已装载，等待main发送
#define FRF_TX_BUSY       2U   // main正在发送
extern volatile Uint16 frf_tx_state;


extern volatile Uint16 scib_capture[6];
extern volatile Uint16 scib_capture_cnt;
extern volatile Uint16 scib_send_once;
extern volatile Uint16 scib_capture_done;
extern volatile Uint16 scib_capture_crc;





void Sweep_Init(Sweep_Id_t *sw,float Ts,float f_start,float f_end,float f_step,float amplitude);

void Sweep_Update(Sweep_Id_t *sw,float id_measured);

void Single_phase_Motor_Control(void);
void Dual_Phase_Motor_Control(void);
void Motor_Operation_Mode_Selection(void);

void FOC_controller(ASR_mode_ALL s_mode, ACR_mode_ALL c_mode, POS_mode_ALL p_mode);

void Proportional_limiting();

void speed_test_step(float start, float end, float step, Uint16 step_time,float *n, Uint16 *cont, char *flag);


static inline void Oscillation_Capture_ISR(void);

#define OSC_CAPTURE_DECIM       10U
#define OSC_CAPTURE_FS          1000.0f
#define OSC_CAPTURE_N           2048U

void Start_Oscillation_Capture(void);

extern volatile Uint16 start_osc_capture;
extern volatile Uint16 osc_capture_enable;
extern volatile Uint16 osc_capture_done;

extern volatile Uint16 osc_sample_index;
extern volatile Uint16 osc_decim_count;

extern volatile Uint16 osc_tx_overrun;




#endif /* USER_INT_FUN_H_ */
