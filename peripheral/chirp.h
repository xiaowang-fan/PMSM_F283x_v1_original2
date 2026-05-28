/*
 * chirp.h
 *
 *  Created on: 2026年5月20日
 *      Author: 86135
 */

#ifndef PERIPHERAL_CHIRP_H_
#define PERIPHERAL_CHIRP_H_


#include <math.h>
#include "F28x_Project.h"

#define CHIRP_PI          3.14159265358979f
#define STEADY_START_RATIO 0.250f // 前40%时间用于等系统稳定，丢弃暂态


typedef struct
{
    /* =====================================================
     * 用户配置参数
     * ===================================================== */

    float f0;                  // 起始频率
    float f1;                  // 终止频率

    float A;                   // 扰动幅值

    int N_total;               // 总频点数

    float ts;                  // 采样周期

    /* =====================================================
     * 固定周期积分参数（新增）
     * ===================================================== */

    float cycle_num;           // 每个频点积分周期数（推荐20）

    float transient_cycles;    // 跳过的暂态周期数（推荐5）

    /* =====================================================
     * 当前频点动态参数
     * ===================================================== */

    float dwell_time;          // 当前频点停留时间

    float transient_time;      // 当前频点暂态时间

    float freq_current;        // 当前频率

    /* =====================================================
     * 扫频运行状态
     * ===================================================== */

    float phase;               // 当前相位

    float time_count;          // 总运行时间

    float step_time_count;     // 当前频点时间计数

    int current_step;          // 当前频点编号

    int is_running;            // 扫频运行标志

    /* =====================================================
     * 在线同步DFT积分器
     * ===================================================== */

    float ref_sin_sum;         // 输入*sin积分

    float ref_cos_sum;         // 输入*cos积分

    float fbk_sin_sum;         // 输出*sin积分

    float fbk_cos_sum;         // 输出*cos积分

    unsigned long sample_cnt;  // 有效积分点数

    /* =====================================================
     * 单频频响结果
     * ===================================================== */

    float result_freq;         // 当前频点频率

    float result_mag_dB;       // 幅值(dB)

    float result_phase_deg;    // 相位(deg)

    int result_ready_flag;     // 新结果完成标志

} LogStepSweep_t;

extern LogStepSweep_t g_speed_sweep;
extern LogStepSweep_t g_current_sweep;

void Init_Log_Step_Sweep(LogStepSweep_t *v,float f0,float f1,float A,int   N_total,float cycle_num,float transient_cycles,float isr_freq);
float Calc_Step_Log_Sweep(LogStepSweep_t *v, float fbk_val);

#endif /* PERIPHERAL_CHIRP_H_ */
