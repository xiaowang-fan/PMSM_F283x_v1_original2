/*
 * eQEP_position_speed.h
 *
 *  Created on: 2024年4月14日
 *      Author: Haoyi Mu
 */

#ifndef PERIPHERAL_EQEP_POSITION_SPEED_H_
#define PERIPHERAL_EQEP_POSITION_SPEED_H_

#include <math.h>
#include <stdint.h>
#include "F28x_Project.h"

//
#define QEP_SAMPLE_FREQ_HZ          10000.0f
#define FRF_SPEED_WINDOW_SAMPLES    500U
// Defines

#define POSSPEED_DEFAULTS {0, 0, 0, 0, 0, 1e-4, \
                           0, 0, 10000, 5, 700,\
                           0.975, 0.025, \
                           0,0,0,0,0,0,0,\
                           (void (*)(long))POSSPEED_Init,\
                           (void (*)(long))POSSPEED_Calc }   //1860  765

//370
// Globals
//
typedef struct {float theta_elec;         // Output: Motor Electrical angle
                float theta_mech;         // Output: Motor Mechanical Angle
                int DirectionQep;       // Output: Motor rotation direction
                int theta_raw;          // Variable: Raw angle
                int32_t old_theta_raw; // 【新增】：用于记录上一时刻的软件角度脉冲
                float mech_scaler;        // Parameter: 1/total count, total
                                        // count = 1e4
                long rev_cnt;
                float theta_pos;

                int N_line;
                int pole_pairs;         // Parameter: Number of pole pairs
                int cal_angle;          // Parameter: Raw angular offset
                                        // between encoder and phase A
                float k1;
                float k2;

                int oldpos;            // Input: Electrical angle (pu)
                float SpeedRpm_raw;
                float SpeedRpm_frf;
                float SpeedRpm_fr;      // Output : Speed in rpm  (Q0) -
                                        // independently with global Q
                float speed_send_rpm;
                int32_t frf_delta_cnt_sum;
                Uint16 frf_speed_sample_cnt;
                void (*init)();         // Pointer to the init funcion
                void (*calc)();         // Pointer to the calc funtion
                }POSSPEED;

//typedef struct {
//    float theta_elec; float theta_mech; int DirectionQep;
//    int theta_raw; int32_t old_theta_raw; float mech_scaler;
//    long rev_cnt; float theta_pos;
//    int N_line; int pole_pairs; int cal_angle;
//
//    // 差分缓存数组 (用 10 个周期的历史值实现 1ms 变长差分)
//    int32_t theta_history[10];
//    int history_idx;
//
//    float SpeedRpm_fr;      // 最终给 1kHz 速度环的高精度反馈速度
//    float speed_send_rpm;
//
//    void (*init)(); void (*calc)();
//} POSSPEED;
//
//#define POSSPEED_DEFAULTS {0, 0, 0, 0, 0, 1e-4, \
//                           0, 0, 10000, 5, 750, \
//                           {0}, 0, 0, 0, \
//                           (void (*)(long))POSSPEED_Init, \
//                           (void (*)(long))POSSPEED_Calc }
//
//

void POSSPEED_Init(POSSPEED *p);
void POSSPEED_Calc(POSSPEED *p);


#endif /* PERIPHERAL_EQEP_POSITION_SPEED_H_ */
