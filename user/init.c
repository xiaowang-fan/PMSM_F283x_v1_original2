/*
 * init.c
 *
 *  Created on: 15/4/2024
 *      Author: Haoyi Mu
 */

#include "init.h"


float SQRT_R_3 = 0.57735;   //  sqrt(1/3)
int CurrentOffsetA = 2004;
int CurrentOffsetB = 2004;
int CurrentOffsetC = 1999;
int CurrentOffsetA1 = 2133;
int CurrentOffsetB1 = 2210;
int CurrentOffsetC1 = 2093;
int CurrentOffsetA2 = 2106;
int CurrentOffsetB2 = 2095;
int CurrentOffsetC2 = 2110;
float CurrentCoef = 200 , CurrentCoef_p = 0;
float CurrentCoef_A = 1.0, CurrentCoef_B = 1.0, CurrentCoef_C = 1.0;

int Current_regA = 0, Current_regB = 0, Current_regC = 0;
int Current_regA1 = 0, Current_regB1 = 0, Current_regC1 = 0;
int Current_regA2 = 0, Current_regB2 = 0, Current_regC2 = 0;
float Ia = 0, Ib = 0, Ic = 0;

float Udc = 100.0, Imax = 2.0, U_svMax = 6.0, SpeedMax = 100.0, Uz_Max = 0;
char SPEED_LOOP = 10;
char POSITION_LOOP = 100;
char SV_protect = 0;

//int encoder_offset = 370; //直接在结构体里面修改即可



PIREG3 ASR = PIREG3_DEFAULTS, ACR_d = PIREG3_DEFAULTS, ACR_q = PIREG3_DEFAULTS,ACR_x = PIREG3_DEFAULTS,ACR_y = PIREG3_DEFAULTS, POS = PIREG3_DEFAULTS;

SVPWM_2L SVpwm1 = SVPWM_2L_DEFAULTS;
SVPWM_2L SVpwm2 = SVPWM_2L_DEFAULTS;

//ESO
ESO ASR_ESO = ESO_DEFAULTS, ACR_ESO_d = ESO_DEFAULTS, ACR_ESO_q = ESO_DEFAULTS;
float ESO_lambda_c = -1000.0F, ESO_lambda_w = -80.0F;

//QR
QR ASR_QR = QR_DEFAULTS;
QR ACR_d_QR = QR_DEFAULTS;
QR ACR_q_QR = QR_DEFAULTS;

//Dual_O test
Dual_O Dual_obs_id = Dual_O_id_DEFAULTS;
Dual_O Dual_obs_iq = Dual_O_iq_DEFAULTS;
Dual_O Dual_obs_wm = Dual_O_wm_DEFAULTS;

LPF ACR_id_ref_LPF = LPF_DEFAULTS;


void init_control_para(){

    CurrentCoef_p = 1.0F/CurrentCoef;
    U_svMax = Udc * SQRT_R_3;


    Pos_Speed.k1 = 0.995; //0.998
    Pos_Speed.k2 = 0.005;

    POS.Ki = 0.0;
    POS.Kp = 3.0;
    POS.OutMax = SpeedMax;
    POS.OutMin = - SpeedMax;

    ACR_d.Kp = 2.5;   //2
    ACR_d.Ki = 0.08; //0.4
    ACR_d.OutMax = U_svMax;
    ACR_d.OutMin = - ACR_d.OutMax;

    ACR_q.syn(&ACR_q, &ACR_d);

    ACR_x.Kp = 1;//0.05;//0.1;//0.9;//0.9;//5;//10      //0.9
    ACR_x.Ki = 0.1;//.02;//0.001;//.09;
    ACR_x.OutMax = U_svMax;
    ACR_x.OutMin = - ACR_d.OutMax;

    ACR_y.syn(&ACR_y, &ACR_x);


    ASR.Kp = 0.012;   //0.046
    ASR.Ki = 0.003;   //0.0022
    ASR.OutMax = Imax;
    ASR.OutMin = - Imax;

    ASR_ESO.Ts = 1e-4;
    ACR_ESO_d.Ts = 1e-4;
    ACR_ESO_q.Ts = 1e-4;

    ACR_id_ref_LPF.k1 = 0.92;//0.97;

    ASR_QR.zeta1 = 0.15;
    ASR_QR.zeta2 = 0.01;
    ASR_QR.Ts = 1e-4;
    ASR_QR.w = 80.0 *2*PI;// 480rpm *5 *2 /60  , 2倍波动
    ASR_QR.para(&ASR_QR);

    ACR_d_QR.zeta1 = 10.0;
    ACR_d_QR.zeta2 = 0.01;
    ACR_d_QR.Ts = 1e-4;
    ACR_d_QR.w = 80.0 *6*PI;
    ACR_d_QR.para(&ACR_d_QR);

    ACR_q_QR.zeta1 = 550.0;
    ACR_q_QR.zeta2 = 0.0001;
    ACR_q_QR.Ts = 1e-4;
    ACR_q_QR.w = 80.0 *6*PI;
    ACR_q_QR.para(&ACR_q_QR);



    Dual_obs_id.l = 10.0;
    Dual_obs_iq.l = 10.0;
    Dual_obs_wm.l = 10.0;
    Dual_obs_id.Ts = 1e-4;
    Dual_obs_iq.Ts = 1e-4;
    Dual_obs_wm.Ts = 1e-4;


//    Pos_Speed.cal_angle = encoder_offset;
    SVpwm1.Udc = Udc;
    SVpwm2.Udc = Udc;

}

void Limiting_amplitude(float *in, float lim){
    if (fabsf(*in) >lim){
        if(*in >0){
            *in = lim;
        }else{
            *in = - lim;
        }
    }
}

void Hysteresis_loop_compare(float *in, float threshold){
    static float pre_value = 0;
    if (fabsf(pre_value - *in) > threshold){
        pre_value = *in;
    } else{
        *in = pre_value;
    }
}



