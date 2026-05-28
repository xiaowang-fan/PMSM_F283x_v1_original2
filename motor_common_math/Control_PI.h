//
// Created by Haoyi Mu on 2024/4/24.
//

#ifndef CONTROL_PI_H
#define CONTROL_PI_H

#define PIREG3_DEFAULTS {0,0,0,0,0, \
                         0,0,0,     \
                         0,0,0,0,   \
                         (void (*)(long))PIREG3_cal_pa, \
                         (void (*)(long))PIREG3_cal_se, \
                         (void (*)(long))PIREG3_syn}

typedef struct {
    float  Ref;               // Input: Reference input
    float  Fdb;               // Input: Feedback input
    float  Err;               // Variable: Error
    float  Kp;                // Parameter: Proportional gain
    float  Ki;                // Parameter: Integral gain

    float  Up;                // Variable: Proportional output
    float  Ui;                // Variable: Integral output
    char   flag;              // Variable: saturated flag

    float  OutPreSat;         // Variable: Pre-saturated output
    float  OutMax;            // Parameter: Maximum output
    float  OutMin;            // Parameter: Minimum output
    float  Out;               // Output: PID output
    void (*calc_pa)();       // Pointer to  calculation function
    void (*calc_se)();       // Pointer to  calculation function
    void (*syn)();
}PIREG3;
typedef PIREG3 *PIDREG3_handle;
void PIREG3_cal_pa(PIDREG3_handle, float *ref, float  *feb, float  *output); //parallel
void PIREG3_cal_se(PIDREG3_handle, float *ref, float  *feb, float  *output); //series
void PIREG3_syn(PIDREG3_handle, PIDREG3_handle);

#endif //CONTROL_PI_H



