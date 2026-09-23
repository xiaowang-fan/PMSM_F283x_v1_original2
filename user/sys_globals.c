/*
 * sys_globals.c
 *
 *  Created on: 2026年2月2日
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/user/sys_globals.h"

float PI = 3.14159265;


/*  --------------------------- external parameter --------------------------- */
float external_r  = 147.1946;
float external_Lo = 599;;
float external_d  = 617;
float external_theta = 1.328649;
float external_encoder_offset = 2.48186111;
float motor_encoder_offset = 0;
float arm_angle = 0;
float x_l = 0;
float x_m = 0;
/*  --------------------------- motor --------------------------- */
motor_parameter PMSM = PMSM_data_750W;


/*  --------------------------- Encoder --------------------------- */
POSSPEED Pos_Speed = POSSPEED_DEFAULTS;


/*  --------------------------- Communication --------------------------- */
float mesg[9]={0};
char *mes_p = (char *)&mesg;
volatile int tx_ready = 0; // 发送允许标志


/*  --------------------------- Operating Mode --------------------------- */
ACR_mode_ALL mode_ACR = ACR_open;// ACR_cls;
ASR_mode_ALL mode_ASR = ASR_cls;//ASR_cls;
POS_mode_ALL mode_POS = POS_cls;
ACR_Z_mode_ALL mode_ACR_Z = ACR_Z_cls;
float I_dref   = 0;
float I_qref   = 0;
float n_ref    = 0;
float pos_ref  = 0;
float I_xref   = 0;
float I_yref   = 0;
float u_d      = 0;
float u_q      = 0;
float omega_ek = 0;
float pos_offset   = 0;
float pos_feedback = 0;
bool pos_flag      = false;


float I_abc1[3], I_abc2[3];
float u_alpha,u_beta,u_A1,u_B1,u_A2,u_B2,u_x,u_y = 0;


/*  --------------------------- Times --------------------------- */
Run_time Run_time1 = Run_time_DEFAULTS;
