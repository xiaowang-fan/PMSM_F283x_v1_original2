//
// Created by Haoyi Mu on 2024/4/30.
//

#ifndef CONTROL_ESO_H
#define CONTROL_ESO_H

#include <math.h>
#include "sys_globals.h"

typedef struct {
    float  Fdb;               // Input: Feedback input
    float  Err;               // Variable: Error
    float  p1;                 // Parameter:
    float  p2;                 // Parameter:
    float  Ts;                 // Parameter:

    float  z1;                // Variable:
    float  z2;                // Variable:
    float  z1_old;                // Variable:
    float  z2_old;                // Variable:
    void (*calc_id)();       // Pointer to  calculation function
    void (*calc_iq)();       // Pointer to  calculation function
    void (*calc_we)();       // Pointer to  calculation function
    void (*cal_p1)();
    void (*cal_p2)();
    void (*updata)();
    void (*cal_ULM)();
} ESO;
typedef ESO *ESO_handle;
#define ESO_DEFAULTS {0, 0, 0, 0, 0,   0, 0, 0, 0, \
                      (void (*)(long))ESO_cal_id, (void (*)(long))ESO_cal_iq, \
                      (void (*)(long))ESO_cal_we, (void (*)(long))ESO_cal_P1, \
                      (void (*)(long))ESO_cal_P2, (void (*)(long))ESO_cal_updata, \
                      (void (*)(long))ESO_cal_i_ULM}

void ESO_cal_id(ESO_handle, ESO_handle, motor_parameter_handle, float *ud, float *we, float *feb);
void ESO_cal_iq(ESO_handle, ESO_handle, motor_parameter_handle, float *uq, float *we, float *feb);
void ESO_cal_we(ESO_handle, motor_parameter_handle, float *iq, float *feb);
void ESO_cal_updata(ESO_handle);
void ESO_cal_syn_p(ESO_handle, ESO_handle);
void ESO_cal_P1(ESO_handle, ESO_handle, motor_parameter_handle, float *lambda, float *we);
void ESO_cal_P2(ESO_handle, float *lambda);
void ESO_cal_i_ULM(ESO_handle, motor_parameter_handle, float *u, float *feb);

#endif //CONTROL_ESO_H
