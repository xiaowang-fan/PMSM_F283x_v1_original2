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
#include "../../PMSM_F283x_v1_original2/BSP/ADC_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/DAC_init.h"
#include "../../PMSM_F283x_v1_original2/communication/CAN_protocol.h"
#include "main.h"
#include "sys_globals.h"
#include "chirp.h"


extern int CurrentOffsetA, CurrentOffsetB;
extern int CurrentOffsetC;
extern float CurrentCoef, CurrentCoef_p;
extern float CurrentCoef_A, CurrentCoef_B, CurrentCoef_C;
extern int CurrentOffsetA1, CurrentOffsetB1, CurrentOffsetC1;
extern int CurrentOffsetA2, CurrentOffsetB2, CurrentOffsetC2;
extern int CurrentOffsetC;
extern int Current_regA ,Current_regB , Current_regC;
extern int Current_regA1 ,Current_regB1 , Current_regC1;
extern int Current_regA2 ,Current_regB2 , Current_regC2;
extern float Ia, Ib, Ic;
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
__interrupt void cana_rx_isr(void);
__interrupt void canb_rx_isr(void);
__interrupt void fault_isr(void);
__interrupt void temp_isr(void);
__interrupt void ready_isr(void);
extern Uint32 Emergency_Flag;
extern Uint16 SysEn;// 0 means malfunction

extern float Id,Iq,Ix,Iy;
extern float u_alpha,u_beta,u_A1,u_B1,u_A2,u_B2,u_x,u_y;



void FOC_controller(ASR_mode_ALL s_mode, ACR_mode_ALL c_mode, POS_mode_ALL p_mode);

void Proportional_limiting();



void speed_test_step(float start, float end, float step, Uint16 step_time,float *n, Uint16 *cont, char *flag);

#endif /* USER_INT_FUN_H_ */
