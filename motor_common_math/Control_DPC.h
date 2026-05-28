//
// Created by Haoyi Mu on 2024/4/30.
//

#ifndef CONTROL_DPC_H
#define CONTROL_DPC_H

#include "Control_ESO.h"
#include "coordinate_transformation.h"
#include "Control_LPF.h"
#include "Control_QPR.h"
#include "cal_idq_include_R_d_base_iq_21_pkg/cal_idq_include_R_d_base_iq_20.h"
#include "sys_globals.h"

extern void Limiting_amplitude(float *in, float lim);
extern void Hysteresis_loop_compare(float *in, float threshold);

void controller_DPCC_without_Observer(CT_3R2S_handle, motor_parameter_handle, \
           float *we, float *idref, float *iqref, float Tc, float *ud, float *uq);

void controller_DPCC_ESO(ESO_handle, ESO_handle, motor_parameter_handle, \
         float *we, float *idref, float *iqref, float Tc, float *ud, float *uq);

void controller_DP_S_ESO(ESO_handle, motor_parameter_handle, \
            float *n_ref, float *iqref ,float *Imax, float Tp);

void controller_DP_S_ESO_FW(ESO_handle,ESO_handle,ESO_handle, motor_parameter_handle, LPF_handle, \
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp);

void controller_DPCC_ULM_ESO(ESO_handle, ESO_handle, motor_parameter_handle, \
                             float *idref, float *iqref, float Tc, float *ud, float *uq);

void controller_DP_S_ESO_QR(ESO_handle, QR_handle, motor_parameter_handle, \
            float *n_ref, float *iqref ,float *Imax, float Tp);

void controller_DP_S_ESO_QR_FW(ESO_handle,ESO_handle,ESO_handle, QR_handle, motor_parameter_handle, LPF_handle, \
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp);

void controller_DPCC_ESO_QR(ESO_handle, ESO_handle, QR_handle, QR_handle, motor_parameter_handle, \
         float *we, float *idref, float *iqref, float Tc, float *ud, float *uq);

void controller_DP_S_ESO_QR_FW_2(ESO_handle,ESO_handle,ESO_handle, QR_handle, QR_handle, motor_parameter_handle, LPF_handle, \
            float *n_ref, float *idref ,float *iqref ,float *Imax, float *Umax, float Tp);


#endif //CONTROL_DPC_H
