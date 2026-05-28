//
// Created by Haoyi Mu on 2024/5/13.
//

#ifndef CONTROL_QPR_H
#define CONTROL_QPR_H

typedef struct {
    float  zeta1;                 // Parameter:
    float  zeta2;                 // Parameter:
    float  w;                 // Parameter:
    float  Ts;                 // Parameter:

    float  in1;                // input:
    float  in2;                // Variable:
    float  in3;                // Variable:
    float  out1;                // output:
    float  out2;                // Variable:
    float  out3;                // Variable:

    float a1, a2, a3, b1, b2, b3;
    void (*para)();
    void (*cal)();
} QR;
typedef QR *QR_handle;
#define QR_DEFAULTS {0, 0, 0, 0,   0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0,\
                      (void (*)(long))QR_cal_para, (void (*)(long))QR_cal_out}

void QR_cal_out(QR_handle);
void QR_cal_para(QR_handle);

#endif //CONTROL_QPR_H
