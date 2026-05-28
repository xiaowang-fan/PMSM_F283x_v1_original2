//
// Created by Haoyi Mu on 2024/4/24.
//

#include "Control_PI.h"

void PIREG3_cal_se(PIREG3 *p, float *ref, float  *feb, float  *output){
    p->Ref = *ref;
    p->Fdb = *feb;
    p->Err = p->Ref - p->Fdb;
    p->Up  = p->Kp *p->Err;
    p->OutPreSat = p->Ui + p->Ki * p->Up;

    if(p->OutPreSat >= p->OutMax)
        p->flag = 0;
    else if(p->OutPreSat <= p->OutMin)
        p->flag = 0;
    else
        p->flag = 1;

    p->Ui = p->Ui + p->Ki * p->Up *p->flag;
    p->OutPreSat = p->Ui + p->Up;

    if(p->OutPreSat >= p->OutMax)
        p->Out = p->OutMax;
    else if(p->OutPreSat <= p->OutMin)
        p->Out = p->OutMin;
    else
        p->Out = p->OutPreSat;

    *output = p->Out;
}

void PIREG3_cal_pa(PIREG3 *p, float *ref, float  *feb, float  *output){
    p->Ref = *ref;
    p->Fdb = *feb;
    p->Err = p->Ref - p->Fdb;
    p->Up  = p->Kp *p->Err;
    p->OutPreSat = p->Ui + p->Ki * p->Err;

    if(p->OutPreSat >= p->OutMax)
        p->flag = 0;
    else if(p->OutPreSat <= p->OutMin)
        p->flag = 0;
    else
        p->flag = 1;

    p->Ui = p->Ui + p->Ki * p->Err *p->flag;
    p->OutPreSat = p->Ui + p->Up;

    if(p->OutPreSat >= p->OutMax)
        p->Out = p->OutMax;
    else if(p->OutPreSat <= p->OutMin)
        p->Out = p->OutMin;
    else
        p->Out = p->OutPreSat;

    *output = p->Out;
}

void PIREG3_syn(PIREG3 *p1, PIREG3 *p2){
    p1->Ki = p2->Ki;
    p1->Kp = p2->Kp;
    p1->OutMax = p2->OutMax;
    p1->OutMin = p2->OutMin;
}

