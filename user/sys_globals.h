/*
 * sys_globals.h
 *
 *  Created on: 2026年2月2日
 *      Author: 86135
 */

#ifndef USER_SYS_GLOBALS_H_
#define USER_SYS_GLOBALS_H_

#include "F28x_Project.h"
#include "eQEP_position_speed.h"
#include "coordinate_transformation.h"
#include "Control_PI.h"
#include "Control_QPR.h"
#include <math.h>
#include <stdio.h>
#include "ADC_init.h"
#include "DAC_init.h"
#include "CAN_protocol.h"

extern float PI;
/*  --------------------------- external parameter --------------------------- */
extern float external_r;
extern float external_Lo;
extern float external_d;
extern float external_theta;
extern float external_encoder_offset;
extern float motor_encoder_offset;
extern float arm_angle;
extern float x_l;
extern float x_m;

/*  --------------------------- motor --------------------------- */
typedef struct {
    float L;
    float psi_f;
    float R;
    float J;
    int   n;
}motor_parameter;

typedef motor_parameter *motor_parameter_handle;

#define PMSM_data_750W  {0.00311403F , 0.03535F, 1.109F, 2.39F, 5}//{0.004F , 0.061F, 0.74F, 0.00016F, 5}
//1.33 new test plant
extern motor_parameter PMSM;

/*  --------------------------- Encoder --------------------------- */

extern POSSPEED Pos_Speed;

/*  --------------------------- Communication  --------------------------- */

extern char *mes_p;
extern float mesg[9];
extern volatile int tx_ready; // 发送允许标志


/*  --------------------------- Operating Mode  --------------------------- */

typedef enum {
    ASR_cls = 0, ASR_open, ASR_PI, ASR_DP_without_observer, ASR_DP_ESO, ASR_DP_ESO_FW, ASR_DP_ESO_QR, ASR_DP_ESO_QR_FW, \
    ASR_DP_ESO_QR_FW_2, ASR_Obs_test
}ASR_mode_ALL;

typedef enum {
    ACR_cls = 0, ACR_open, ACR_PI, ACR_DP_without_observer, ACR_DP_ESO, ACR_DP_SMDO, ACR_ULM_DP_ESO, ACR_DP_ESO_QR, \
    ACR_Obs_test
}ACR_mode_ALL;

typedef enum {
    POS_cls = 0 ,POS_open, POS_PI
}POS_mode_ALL;

typedef enum {
    ACR_Z_cls = 0, ACR_Z_open, ACR_Z_PI
}ACR_Z_mode_ALL;

extern ACR_mode_ALL mode_ACR;// ACR_cls;
extern ASR_mode_ALL mode_ASR;//ASR_cls;
extern POS_mode_ALL mode_POS;
extern ACR_Z_mode_ALL mode_ACR_Z;

extern float I_dref;
extern float I_qref;
extern float n_ref;
extern float pos_ref;
extern float I_xref;
extern float I_yref;
extern float u_d;
extern float u_q;
extern float omega_ek;
extern float pos_offset;
extern float pos_feedback;
extern bool pos_flag;

extern float u_alpha,u_beta,u_A1,u_B1,u_A2,u_B2,u_x,u_y;

/*  --------------------------- Times --------------------------- */
#define Run_time_DEFAULTS {0, 0, 0, 0, 0,(void (*)(long))Run_time_cal}

typedef struct {
    Uint16 tick;
    char s;
    char m;
    char h;
    char d;
    void (*calc)();         // Pointer to the calc funtion
}Run_time;
typedef Run_time *Run_time_handle;
void Run_time_cal(Run_time_handle);

extern Run_time Run_time1;

#endif /* USER_SYS_GLOBALS_H_ */
