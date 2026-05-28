//
// Created by Haoyi Mu on 2024/4/30.
//

#include "Control_DPC.h"

void controller_DPCC_without_Observer(CT_3R2S *I, motor_parameter *m, \
           float *we, float *idref, float *iqref, float Tc, float *ud, float *uq){
    float i_dp =0, i_qp =0;
    i_dp = (1- m->R/m->L *Tc) *I->Ds  +Tc *( *we *I->Qs + *ud/m->L );
    i_qp = (1- m->R/m->L *Tc) *I->Qs  -Tc *( *we *I->Ds - *uq/m->L +m->psi_f * *we /m->L);

    *ud = m->L* *idref /Tc + (m->R - m->L/Tc) *i_dp - *we *i_qp *m->L;
    *uq = m->L* *iqref /Tc + (m->R - m->L/Tc) *i_qp + *we *i_dp *m->L +m->psi_f * *we;

}

void controller_DPCC_ESO(ESO *pd, ESO *pq, motor_parameter *m, \
        float *we, float *idref, float *iqref, float Tc, float *ud, float *uq){
    *ud = m->L* *idref /Tc + (m->R - m->L/Tc) * pd->z1 - *we *pq->z1 *m->L  - m->L *pd->z2;
    *uq = m->L* *iqref /Tc + (m->R - m->L/Tc) * pq->z1 + *we *pd->z1 *m->L  - m->L *pq->z2 + m->psi_f * *we;

    pd->updata(pd);
    pq->updata(pq);

}

void controller_DP_S_ESO(ESO *p, motor_parameter *m, float *n_ref, float *iqref ,float *Imax, float Tp){
    float iq_ref_temp = 0;
    iq_ref_temp = m->J / (1.5F *m->n *m->n *m->psi_f *Tp) * (*n_ref * 0.5236F - p->z1 - p->z2 *Tp );// 0.5236==>2pi*5/60

    Limiting_amplitude(&iq_ref_temp, *Imax);
    *iqref = iq_ref_temp;

//    p->updata(p);
}

void controller_DP_S_ESO_FW(ESO *pd, ESO *pq,ESO *pw, motor_parameter *m, LPF *F,\
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp){

    static float id_set_old = 0, iq_set_old = 0;
    static float flag1 = 0, index1 = 0, index2 = 0;
    float iq_ref_temp = 0;
    iq_ref_temp = m->J / (1.5F *m->n *m->n *m->psi_f *Tp) * (*n_ref * 0.5236F - pw->z1 - pw->z2 *Tp );// 0.5236==>2pi*5/60

    cal_idq_include_R_d_base_iq_20(iq_ref_temp, pw->z1, pd->z2, pq->z2,
                                   m->R, m->L, m->psi_f, *Umax, idref, iqref,
                                   &flag1, &index1, &index2);

    if(*idref > -*Imax && *idref <1.0F && fabsf(*iqref)<= *Imax){
        id_set_old = *idref;
        iq_set_old = *iqref;
    } else{
        *idref = id_set_old;
        *iqref = iq_set_old;
    }

    F->calc(F,idref);

}

void controller_DPCC_ULM_ESO(ESO *pd, ESO *pq, motor_parameter *m, \
                             float *idref, float *iqref, float Tc, float *ud, float *uq){

    *ud = ((*idref - pd->z1) / Tc - pd->z2) *m->L;
    *uq = ((*iqref - pq->z1) / Tc - pq->z2) *m->L;

    pd->updata(pd);
    pq->updata(pq);
}

void controller_DP_S_ESO_QR(ESO *p, QR *Q, motor_parameter *m, \
            float *n_ref, float *iqref ,float *Imax, float Tp){

    float iq_ref_temp = 0;
    iq_ref_temp = m->J / (1.5F *m->n *m->n *m->psi_f *Tp) * (*n_ref * 0.5236F - p->z1 - p->z2 *Tp );// 0.5236==>2pi*5/60
    iq_ref_temp += Q->out1;

    Limiting_amplitude(&iq_ref_temp, *Imax);
    *iqref = iq_ref_temp;

}

void controller_DP_S_ESO_QR_FW(ESO *pd, ESO *pq,ESO *pw, QR *Q, motor_parameter *m, LPF *F, \
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp){

    static float id_set_old = 0, iq_set_old = 0;
    static float flag1 = 0, index1 = 0, index2 = 0;
    float iq_ref_temp = 0;
    iq_ref_temp = m->J / (1.5F *m->n *m->n *m->psi_f *Tp) * (*n_ref * 0.5236F - pw->z1 - pw->z2 *Tp );// 0.5236==>2pi*5/60
//    iq_ref_temp +=  Q->out1; //m->J / (1.5F *m->n *m->n *m->psi_f *Tp) *

    cal_idq_include_R_d_base_iq_20(iq_ref_temp, pw->z1, pd->z2, pq->z2,
                                   m->R, m->L, m->psi_f, *Umax, idref, iqref,
                                   &flag1, &index1, &index2);

    (*iqref) += Q->out1;

    if(*idref > -*Imax && *idref <1.0F && fabsf(*iqref)<= *Imax){
        id_set_old = *idref;
        iq_set_old = *iqref;
    } else{
        *idref = id_set_old;
        *iqref = iq_set_old;
    }

    F->calc(F,idref);
//    Limiting_amplitude(&iq_ref_temp, *Imax);
//    *iqref = iq_ref_temp;

}

void controller_DPCC_ESO_QR(ESO *pd, ESO *pq, QR *Qd, QR *Qq, motor_parameter *m, \
         float *we, float *idref, float *iqref, float Tc, float *ud, float *uq){

    *ud = m->L* *idref /Tc + (m->R - m->L/Tc) * pd->z1 - *we *pq->z1 *m->L  - m->L *pd->z2 + m->L * Qd->out1;
    *uq = m->L* *iqref /Tc + (m->R - m->L/Tc) * pq->z1 + *we *pd->z1 *m->L  - m->L *pq->z2 + m->psi_f * *we + m->L * Qq->out1;

    pd->updata(pd);
    pq->updata(pq);
}

void controller_DP_S_ESO_QR_FW_2(ESO *pd, ESO *pq,ESO *pw, QR *Qd, QR *Qq, motor_parameter *m, LPF *F, \
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp){

    static float id_set_old = 0, iq_set_old = 0;
    static float flag1 = 0, index1 = 0, index2 = 0;
    float iq_ref_temp = 0;
    iq_ref_temp = m->J / (1.5F *m->n *m->n *m->psi_f *Tp) * (*n_ref * 0.5236F - pw->z1 - pw->z2 *Tp );// 0.5236==>2pi*5/60

    cal_idq_include_R_d_base_iq_20(iq_ref_temp, pw->z1, pd->z2, pq->z2,
                                   m->R, m->L, m->psi_f, *Umax, idref, iqref,
                                   &flag1, &index1, &index2);

    if(*idref > -*Imax && *idref <1.0F && fabsf(*iqref)<= *Imax){
//        *idref -= 0.1;
//        Hysteresis_loop_compare(idref,0.1);
        id_set_old = *idref;
        iq_set_old = *iqref;

    } else{
        *idref = id_set_old;
        *iqref = iq_set_old;
    }
    F->calc(F,idref);
}

