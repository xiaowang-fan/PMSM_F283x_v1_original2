//
// Created by Mu on 2024/10/10.
//

#include "Control_Dual_Obs.h"

void Dual_O_cal_id(Dual_O *pd, motor_parameter *m, float *id, float *iq,float *ud, float *we){
    pd->z1_p = - pd->l * pd->z1_old - pd->l*(pd->l * *id + 1.0/m->L * *ud - m->R / m->L * *id + *we * *iq);
    pd->z1 = pd->z1_old + pd->z1_p * pd->Ts;
    pd->Out =  pd->z1 + pd->l * *id;
    pd->z1_old = pd->z1;
}

void Dual_O_cal_iq(Dual_O *pq, motor_parameter *m, float *id, float *iq, float *uq,float *we){
    pq->z1_p = - pq->l * pq->z1_old - pq->l*(pq->l * *iq + 1.0/m->L * *uq - m->R / m->L * *iq - *we * *id - m->psi_f * *we/m->L);
    pq->z1 = pq->z1_old + pq->z1_p * pq->Ts;
    pq->Out = pq->z1 + pq->l * *iq;
    pq->z1_old = pq->z1;
}

void Dual_O_cal_wm(Dual_O *pw, motor_parameter *m, float *iq, float *we){
    pw->z1_p = - pw->l * pw->z1_old - pw->l *(pw->l * *we/m->n  + 1.5 * m->n *m->psi_f * *iq / m->J);
    pw->z1 = pw->z1_old + pw->z1_p * pw->Ts;
    pw->Out = pw->z1 + pw->l * *we/m->n;
    pw->z1_old = pw->z1;
}

