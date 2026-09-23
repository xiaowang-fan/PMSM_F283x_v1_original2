/*
 * motor_control_algorithm.h
 *
 *  Created on: 2026年6月8日
 *      Author: 86135
 */

#ifndef USER_MOTOR_CONTROL_ALGORITHM_H_
#define USER_MOTOR_CONTROL_ALGORITHM_H_

#include <math.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    float output;       // 当前斜坡输出，即实际使用的 n_ref
    float accel;        // 加速度，单位：(速度单位)/s
    float decel;        // 减速度，单位：(速度单位)/s
    float sample_time;  // 调用周期，单位：s
} SpeedRamp_t;

extern SpeedRamp_t backlash_speed_ramp;

void SpeedRamp_Init(SpeedRamp_t *ramp, float initial_speed, float accel, float decel, float sample_time);
float SpeedRamp_Update(SpeedRamp_t *ramp, float target_speed);

#endif /* USER_MOTOR_CONTROL_ALGORITHM_H_ */
