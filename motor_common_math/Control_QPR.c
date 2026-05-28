//
// Created by Haoyi Mu on 2024/5/13.
//

#include "Control_QPR.h"

void QR_cal_para(QR *p){
    float temp_1 = p->Ts * p->w;
    float temp_2 = temp_1 *temp_1;
    float temp_3 = 2.0 *temp_1 *p->zeta1;
    float temp_4 = 2.0 *temp_1 *p->zeta2;

    p->a1 = 4.0 + temp_2 + temp_3;
    p->a2 = 2.0 * temp_2 - 8.0;
    p->a3 = 4.0 + temp_2 - temp_3;

    p->b1 = 4.0 + temp_2 + temp_4;
    p->b2 = p->a2;
    p->b3 = 4.0 + temp_2 - temp_4;
}

void QR_cal_out(QR *p){
    p->out1 = (-p->b2 *p->out2 - p->b3 *p->out3 + p->a1 *p->in1 +p->a2 *p->in2 +p->a3* p->in3) / p->b1;

    p->in3 = p->in2;
    p->in2 = p->in1;
    p->out3 = p->out2;
    p->out2 = p->out1;
}
