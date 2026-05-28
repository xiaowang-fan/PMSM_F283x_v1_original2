//
// Created by Haoyi Mu on 2024/4/24.
//

#ifndef SVPWM_H
#define SVPWM_H

#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "coordinate_transformation.h"

#define SVPWM_2L_DEFAULTS {0,0, 0, 1.0,\
                           0.5, 0.5, 0.5, \
                           1e4, 5e3, 5e3, 5e3,       \
                           (void (*)(long))SVPWM_2L_calc, \
                           (void (*)(long))SVPWM_PWM_GEN1, \
                           (void (*)(long))SVPWM_PWM_GEN2, \
                           (void (*)(long))SVPWM_PWM_U_syn }

typedef struct  {
    float  Ualpha;  // Input: reference alpha-axis phase voltage
    float  Ubeta;   // Input: reference beta-axis phase voltage
    float  Udc;     // Input: DC voltage
    float  T;       // Input: PWM Modulation Period
    float  Tcmpa; // Output: reference phase-a switching function   Half of the duty cycle
    float  Tcmpb; // Output: reference phase-b switching function
    float  Tcmpc; // Output: reference phase-c switching function

    Uint16 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles
    Uint16 PWM1out;
    Uint16 PWM2out;
    Uint16 PWM3out;
    void (*calc)();       // Pointer to duty cycle calculation function
    void (*gen1)();          // Pointer to gen function
    void (*gen2)();
    void (*U_syn)();          // Pointer to gen function
} SVPWM_2L;


typedef SVPWM_2L *SVPWM_2L_handle;


void SVPWM_2L_calc(SVPWM_2L_handle);
void SVPWM_PWM_GEN1(SVPWM_2L_handle);
void SVPWM_PWM_GEN2(SVPWM_2L_handle);
void SVPWM_PWM_U_syn(SVPWM_2L_handle, CT_2S2R_handle);




#endif //SVPWM_H
