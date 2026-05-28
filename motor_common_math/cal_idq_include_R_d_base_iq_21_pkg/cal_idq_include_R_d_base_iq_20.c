/*
 * File: cal_idq_include_R_d_base_iq_20.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 18-Mar-2024 08:49:39
 */

/* Include Files */
#include "cal_idq_include_R_d_base_iq_20.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static float FW_cal_CL(float d_d, float d_q, float omega_e, float R_s,
                       float L_s, float U_max, float psi_f, float X_old[2]);

static float FW_cal_id(float i_q, float d_d, float d_q, float omega_e,
                       float R_s, float L_s, float U_max, float psi_f,
                       float *b_index);

/* Function Definitions */
/*
 * Arguments    : float d_d
 *                float d_q
 *                float omega_e
 *                float R_s
 *                float L_s
 *                float U_max
 *                float psi_f
 *                float X_old[2]
 * Return Type  : float
 */
static float FW_cal_CL(float d_d, float d_q, float omega_e, float R_s,
                       float L_s, float U_max, float psi_f, float X_old[2])
{
  float F_new_idx_0;
  float F_new_idx_0_tmp;
  float F_new_idx_1;
  float a;
  float a_tmp;
  float b_a_tmp;
  float b_index;
  float c_a_tmp;
  float e_new;
  float e_old;
  float lambda;
  int c_index;
  bool exitg1;
  lambda = 1.0F;
  /*  R_s = 0.74; */
  /*  L_s = 0.002; */
  /*  U_max = 50/sqrt(3); */
  /*  i_max = 8; */
  a_tmp = L_s * d_d;
  e_new = (a_tmp - R_s * -4.0F) + L_s * 4.0F * omega_e;
  b_a_tmp = L_s * d_q;
  c_a_tmp = omega_e * psi_f;
  a = ((R_s * 4.0F - b_a_tmp) + c_a_tmp) + L_s * -4.0F * omega_e;
  F_new_idx_0_tmp = U_max * U_max;
  F_new_idx_0 = (e_new * e_new + a * a) - F_new_idx_0_tmp;
  F_new_idx_1 = -32.0F;
  X_old[0] = -4.0F;
  X_old[1] = 4.0F;
  e_old = F_new_idx_0 * F_new_idx_0 + 1024.0F;
  b_index = 1.0F;
  c_index = 0;
  exitg1 = false;
  while ((!exitg1) && (c_index < 10)) {
    float b_lambda_tmp;
    float c_lambda_tmp;
    float d_lambda_tmp;
    float e_lambda_tmp;
    float lambda_idx_1;
    float lambda_tmp;
    b_index = (float)c_index + 1.0F;
    /*  R_s = 0.74; */
    /*  L_s = 0.002; */
    a = L_s * R_s;
    lambda_tmp = L_s * L_s;
    b_lambda_tmp = omega_e * omega_e;
    c_lambda_tmp = lambda_tmp * d_d;
    e_new = lambda_tmp * d_q;
    d_lambda_tmp =
        2.0F *
        (((((c_lambda_tmp * X_old[0] * omega_e + e_new * X_old[1] * omega_e) -
            L_s * X_old[1] * b_lambda_tmp * psi_f) +
           a * d_d * X_old[1]) -
          a * d_q * X_old[0]) +
         R_s * X_old[0] * omega_e * psi_f);
    e_lambda_tmp = R_s * R_s;
    lambda_idx_1 = lambda * (X_old[0] / d_lambda_tmp);
    a = lambda * (-((((X_old[0] * lambda_tmp * b_lambda_tmp - e_new * omega_e) -
                      a_tmp * R_s) +
                     psi_f * L_s * b_lambda_tmp) +
                    X_old[0] * e_lambda_tmp) /
                  d_lambda_tmp);
    X_old[0] -= lambda * (-X_old[1] / d_lambda_tmp) * F_new_idx_0 +
                lambda *
                    (((((X_old[1] * lambda_tmp * b_lambda_tmp +
                         c_lambda_tmp * omega_e) -
                        b_a_tmp * R_s) +
                       X_old[1] * e_lambda_tmp) +
                      psi_f * R_s * omega_e) /
                     d_lambda_tmp) *
                    F_new_idx_1;
    X_old[1] -= lambda_idx_1 * F_new_idx_0 + a * F_new_idx_1;
    /*  R_s = 0.74; */
    /*  L_s = 0.002; */
    /*  U_max = 50/sqrt(3); */
    /*  i_max = 8; */
    e_new = (a_tmp - R_s * X_old[0]) + L_s * X_old[1] * omega_e;
    a = ((R_s * X_old[1] - b_a_tmp) + c_a_tmp) + L_s * X_old[0] * omega_e;
    F_new_idx_0 = (e_new * e_new + a * a) - F_new_idx_0_tmp;
    F_new_idx_1 = (X_old[0] * X_old[0] - 64.0F) + X_old[1] * X_old[1];
    e_new = F_new_idx_0 * F_new_idx_0 + F_new_idx_1 * F_new_idx_1;
    if (e_new > e_old) {
      lambda /= 2.0F;
    }
    e_old = e_new;
    if (e_new < 0.1F) {
      exitg1 = true;
    } else {
      c_index++;
    }
  }
  return b_index;
}

/*
 * Arguments    : float i_q
 *                float d_d
 *                float d_q
 *                float omega_e
 *                float R_s
 *                float L_s
 *                float U_max
 *                float psi_f
 *                float *b_index
 * Return Type  : float
 */
static float FW_cal_id(float i_q, float d_d, float d_q, float omega_e,
                       float R_s, float L_s, float U_max, float psi_f,
                       float *b_index)
{
  float F_new;
  float F_old_tmp;
  float X_old;
  float a_tmp;
  float b_a_tmp;
  float c_a_tmp;
  float e_new;
  float e_old;
  float lambda;
  int c_index;
  bool exitg1;
  X_old = -4.0F;
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  lambda = 1.0F;
  /*  R_s = 0.74; */
  /*  L_s = 0.002; */
  /*  U_max = 50/sqrt(3); */
  a_tmp = L_s * d_d;
  b_a_tmp = L_s * i_q * omega_e;
  F_new = (a_tmp - R_s * -4.0F) + b_a_tmp;
  c_a_tmp = (R_s * i_q - L_s * d_q) + omega_e * psi_f;
  e_new = c_a_tmp + L_s * -4.0F * omega_e;
  F_old_tmp = U_max * U_max;
  F_new = (F_new * F_new + e_new * e_new) - F_old_tmp;
  e_old = F_new * F_new;
  *b_index = 1.0F;
  c_index = 0;
  exitg1 = false;
  while ((!exitg1) && (c_index < 10)) {
    *b_index = (float)c_index + 1.0F;
    /*  R_s = 0.74; */
    /*  L_s = 0.002; */
    X_old -=
        lambda *
        (-1.0F / (2.0F * R_s * ((a_tmp - R_s * X_old) + b_a_tmp) -
                  2.0F * L_s * omega_e * (c_a_tmp + L_s * X_old * omega_e))) *
        F_new;
    /*  R_s = 0.74; */
    /*  L_s = 0.002; */
    /*  U_max = 50/sqrt(3); */
    F_new = (a_tmp - R_s * X_old) + b_a_tmp;
    e_new = c_a_tmp + L_s * X_old * omega_e;
    F_new = (F_new * F_new + e_new * e_new) - F_old_tmp;
    e_new = F_new * F_new;
    if (e_new > e_old) {
      lambda /= 2.0F;
    }
    e_old = e_new;
    if (e_new < 0.1F) {
      exitg1 = true;
    } else {
      c_index++;
    }
  }
  return X_old;
}

/*
 * R_s = 0.74;
 *  L_s = 0.002;
 *  psi_f = 0.0622;
 *  U_max = 50/sqrt(3);
 *
 * Arguments    : float iq_set
 *                float omega_e
 *                float d_d
 *                float d_q
 *                float R_s
 *                float L_s
 *                float psi_f
 *                float U_max
 *                float *i_d
 *                float *i_q
 *                float *flag
 *                float *index1
 *                float *index2
 * Return Type  : void
 */
void cal_idq_include_R_d_base_iq_20(float iq_set, float omega_e, float d_d,
                                    float d_q, float R_s, float L_s,
                                    float psi_f, float U_max, float *i_d,
                                    float *i_q, float *flag, float *index1,
                                    float *index2)
{
  float a;
  float a_tmp;
  float b_a;
  float b_a_tmp;
  float c_a;
  float c_a_tmp;
  float eqn2;
  float eqn2_tmp;
  /*  把磁链未包含在扰动项中 */
  a_tmp = L_s * d_q;
  b_a_tmp = omega_e * psi_f;
  a = (R_s * 8.0F - a_tmp) + b_a_tmp;
  c_a_tmp = L_s * d_d;
  b_a = c_a_tmp + L_s * 8.0F * omega_e;
  eqn2 = (R_s * 0.8F - a_tmp) + b_a_tmp;
  c_a = c_a_tmp + L_s * 0.8F * omega_e;
  eqn2_tmp = U_max * U_max;
  eqn2 = (eqn2 * eqn2 + c_a * c_a) - eqn2_tmp;
  *i_q = 0.0F;
  *i_d = 0.0F;
  *flag = 0.0F;
  *index1 = 0.0F;
  *index2 = 0.0F;
  if ((a * a + b_a * b_a) - eqn2_tmp < 0.0F) {
    /* 基速以下         */
    if (fabsf(iq_set) > 8.0F) {
      *flag = 1.0F;
      if (rtIsNaNF(iq_set)) {
        c_a = rtNaNF;
      } else if (iq_set < 0.0F) {
        c_a = -1.0F;
      } else {
        c_a = (iq_set > 0.0F);
      }
      *i_q = c_a * 8.0F;
    } else {
      *flag = 2.0F;
      *i_q = iq_set;
    }
  } else if (eqn2 < 0.0F) {
    float X_old[2];
    /* FW 1 */
    *index1 = FW_cal_CL(d_d, d_q, omega_e, R_s, L_s, U_max, psi_f, X_old);
    a = (R_s * iq_set - a_tmp) + b_a_tmp;
    b_a = c_a_tmp + L_s * iq_set * omega_e;
    if (fabsf(iq_set) > X_old[1]) {
      /*  iq_set 过大 */
      *flag = 3.0F;
      *i_d = X_old[0];
      if (rtIsNaNF(iq_set)) {
        c_a = rtNaNF;
      } else if (iq_set < 0.0F) {
        c_a = -1.0F;
      } else {
        c_a = (iq_set > 0.0F);
      }
      *i_q = c_a * X_old[1];
    } else if ((a * a + b_a * b_a) - eqn2_tmp > 0.0F) {
      /*  cal id */
      *flag = 4.0F;
      *i_d =
          FW_cal_id(iq_set, d_d, d_q, omega_e, R_s, L_s, U_max, psi_f, index2);
      *i_q = iq_set;
    } else {
      *flag = 5.0F;
      *i_q = iq_set;
    }
  } else if (eqn2 >= 0.0F) {
    float X_old[2];
    *index1 = FW_cal_CL(d_d, d_q, omega_e, R_s, L_s, U_max, psi_f, X_old);
    if (fabsf(iq_set) > X_old[1]) {
      /*  iq_set 过大 */
      *flag = 6.0F;
      *i_d = X_old[0];
      if (rtIsNaNF(iq_set)) {
        c_a = rtNaNF;
      } else if (iq_set < 0.0F) {
        c_a = -1.0F;
      } else {
        c_a = (iq_set > 0.0F);
      }
      *i_q = c_a * X_old[1];
    } else {
      /*  cal id */
      *flag = 7.0F;
      *i_d =
          FW_cal_id(iq_set, d_d, d_q, omega_e, R_s, L_s, U_max, psi_f, index2);
      *i_q = iq_set;
    }
  }
  /*  flag = d_q1; */
}

/*
 * File trailer for cal_idq_include_R_d_base_iq_20.c
 *
 * [EOF]
 */
