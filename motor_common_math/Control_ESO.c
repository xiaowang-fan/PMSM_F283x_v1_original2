//
// Created by Haoyi Mu on 2024/4/30.
//

#include "Control_ESO.h"

void ESO_cal_id(ESO *pd, ESO *pq, motor_parameter *m,float *ud, float *we, float *feb){
    pd->Fdb = *feb;
    pd->Err = pd->Fdb - pd->z1_old;
    pd->z1 = pd->z1_old *(1.0F - pd->Ts *  m->R / m->L) + pd->Ts * (*ud /m->L + \
             *we * pq->z1_old + pd->z2_old + pd->p1 *pd->Err);
    pd->z2 = pd->z2_old + pd->Ts *pd->p2 *pd->Err;
}

void ESO_cal_iq(ESO *pd, ESO *pq, motor_parameter *m, float *uq, float *we, float *feb){
    pq->Fdb = *feb;
    pq->Err = pq->Fdb - pq->z1_old;
    pq->z1 = pq->z1_old *(1.0F - pq->Ts *  m->R / m->L) + pq->Ts * (*uq /m->L - \
             *we *pd->z1_old + pq->z2_old + pq->p1 *pq->Err - \
             m->psi_f * *we /m->L);
    pq->z2 = pq->z2_old + pq->Ts *pq->p2 *pq->Err;
}

void ESO_cal_we(ESO *p, motor_parameter *m, float *iq, float *feb){
    p->Fdb = *feb;
    p->Err = p->Fdb - p->z1_old;
    p->z1 = p->z1_old + p->Ts *(1.5F *m->n *m->n *m->psi_f * *iq /m->J  + \
            p->z2_old + p->p1 *p->Err);
    p->z2 = p->z2_old + p->Ts *p->p2 *p->Err;
}

void ESO_cal_updata(ESO *p){
    p->z1_old = p->z1;
    p->z2_old = p->z2;
}

void ESO_cal_syn_p(ESO *p1, ESO *p2){
    p1->p1 = p2->p1;
    p1->p2 = p2->p2;
}

void ESO_cal_P1(ESO *pd, ESO *pq, motor_parameter *m, float *lambda, float *we){
    float abs_we = fabsf(*we);
    pd->p1 = - m->R/m->L - 2.0F * *lambda - abs_we;
    pq->p1 = pd->p1 + 2.0F *abs_we;
    pd->p2 = *lambda * *lambda;
    pq->p2 = pd->p2;
}

void ESO_cal_P2(ESO *p, float *lambda){
    p->p1 = -2.0F * *lambda;
    p->p2 = *lambda * *lambda;
}

void ESO_cal_i_ULM(ESO *p, motor_parameter *m, float *u, float *feb){
    p->Fdb = *feb;
    p->Err = p->Fdb - p->z1_old;
    p->z1 = p->z1_old + p->Ts *(*u / m->L + p->z2_old + p->p1 *p->Err);
    p->z2 = p->z2_old + p->Ts *p->p2 *p->Err;
}

