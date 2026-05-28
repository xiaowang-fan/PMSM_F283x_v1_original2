//
// Created by Haoyi Mu on 2024/4/30.
//

#ifndef CONTROL_LPF_H
#define CONTROL_LPF_H

typedef struct {
    float k1;
    float k2;
    float in;
    float out;
    float out_old;
    void (*calc)();       // Pointer to  calculation function
}LPF;
typedef LPF *LPF_handle;
#define LPF_DEFAULTS {0.9, 0, 0, 0, 0,(void (*)(long))LPF_cal}

void LPF_cal(LPF_handle, float *in);

#endif //CONTROL_LPF_H
