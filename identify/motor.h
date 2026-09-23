/*
 * motor.h
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#ifndef IDENTIFY_MOTOR_H_
#define IDENTIFY_MOTOR_H_


typedef struct{float r;  //电阻
               float L;  //电感
               float phif;//磁链
               float Ts; //电流环采样周期
               float We; //电机电角速度
               float ud; //d轴电压
               float id; //d轴电流
               float uq; //q轴电压
               float iq; //q轴电流
              }MOTOR;
typedef MOTOR *MOTOR_handle;

extern MOTOR identify_motor;

#define MOTOR_DEFAULTS { 1.1,\
                         0.0031,\
                         0.0,\
                         1e-4,\
                         0.0,\
                         0.0,\
                         0.0,\
                         0.0,\
                         0.0}
//
//#define MOTOR_DEFAULTS { 0.9,\
//                         4.2e-3,\
//                         0.0594,\
//                         1e-4,\
//                         0.0,\
//                         0.0,\
//                         0.0,\
//                         0.0,\
//                         0.0}

#endif /* IDENTIFY_MOTOR_H_ */
