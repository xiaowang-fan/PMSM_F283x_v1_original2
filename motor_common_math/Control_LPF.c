//
// Created by Haoyi Mu on 2024/4/30.
//

#include "Control_LPF.h"

void LPF_cal(LPF *F, float *in){
    F->in = *in;
    F->k2 = 1.0F - F->k1;
    F->out = F->k1 * F->out_old  + F->k2 *F->in;
    F->out_old = F->out;
    *in = F->out;
}

