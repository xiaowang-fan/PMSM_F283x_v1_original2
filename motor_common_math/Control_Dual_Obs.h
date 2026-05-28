//
// Created by Mu on 2024/10/10.
//

#ifndef CONTROL_DUAL_Obs_H
#define CONTROL_DUAL_Obs_H

#include <math.h>
#include "sys_globals.h"

typedef struct {
    float l;    // Parameter should be Postive
    float Ts;                 // Parameter:

    float z1;                // Variable:
    float z1_p;
    float z1_old;                // Variable:

    float Out;
    void (*calc)();       // Pointer to  calculation function
} Dual_O;

typedef Dual_O *Dual_O_handle;

#define Dual_O_id_DEFAULTS {0, 0,  0, 0, 0,  0, (void (*)(long))Dual_O_cal_id}
#define Dual_O_iq_DEFAULTS {0, 0,  0, 0, 0,  0, (void (*)(long))Dual_O_cal_iq}
#define Dual_O_wm_DEFAULTS {0, 0,  0, 0, 0,  0, (void (*)(long))Dual_O_cal_wm}

void Dual_O_cal_id(Dual_O_handle, motor_parameter_handle, float *id, float *iq, float *ud,float *we);
void Dual_O_cal_iq(Dual_O_handle, motor_parameter_handle, float *id, float *iq, float *uq,float *we);
void Dual_O_cal_wm(Dual_O_handle, motor_parameter_handle, float *iq, float *we);

#endif //CONTROL_DUAL_Obs_H
