/*
 * sys_globals.c
 *
 *  Created on: 2026Äê2ÔÂ2ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/user/sys_globals.h"

float PI = 3.14159265;

/*  --------------------------- motor --------------------------- */
motor_parameter PMSM = PMSM_data_750W;
/*  --------------------------- Encoder --------------------------- */
POSSPEED Pos_Speed = POSSPEED_DEFAULTS;
/*  --------------------------- Communication --------------------------- */
float mesg[9]={0};
char *mes_p = (char *)&mesg;
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

/*  --------------------------- Transforms --------------------------- */
//CT_3R2S I_CT3R2S = CT_3R2S_DEFAULTS ;
//CT_2S2R U_CT2S2R = CT_2S2R_DEFAULTS ;
/*  --------------------------- Times --------------------------- */
Run_time Run_time1 = Run_time_DEFAULTS;
