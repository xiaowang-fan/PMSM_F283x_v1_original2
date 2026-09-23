/*
 * chirp.h
 *
 *  Created on: 2026年5月20日
 *      Author: 86135
 */

#ifndef PERIPHERAL_CHIRP_H_
#define PERIPHERAL_CHIRP_H_


#include "math.h"
#include "F28x_Project.h"
#include "sys_globals.h"

#define CHIRP_PI          3.14159265358979f
#define STEADY_START_RATIO 0.250f // 前40%时间用于等系统稳定，丢弃暂态


typedef struct
{

        float f0;
        float f1;
        float A;
        Uint16 N_total;

        float cycle_num;
        float transient_cycles;
        float dwell_min;
        float ts;

        float freq_current;
        Uint16 current_step;

        float log_factor;

        float dwell_time;
        float transient_time;
        float measure_time;

        float phase;
        float step_time_count;

        Uint16 measure_started;
        volatile Uint16 is_running;

        float ref_cos_sum;
        float ref_sin_sum;
        float fbk_cos_sum;
        float fbk_sin_sum;
        Uint16 sample_cnt;

        volatile float result_mag_dB;
        volatile float result_phase_deg;
        volatile float result_freq;

        volatile Uint16 result_ready_flag;
        volatile Uint16 finished_flag;

        float speed_sum;
        float angle_sum;

        float result_speed_mean_rpm;
        float result_angle_mean_deg;

        float fbk_sum;
        float fbk_sq_sum;

        float result_input_amp;
        float result_output_amp;
        float result_quality;

        uint16_t result_valid_flag;

} LogStepSweep_t;

extern LogStepSweep_t g_speed_sweep;
extern LogStepSweep_t g_current_sweep;

void Init_Log_Step_Sweep(LogStepSweep_t *v, float f0, float f1, float A, Uint16 N_total, float cycle_num, float transient_cycles, float dwell_min, float isr_freq);
//void Init_Log_Step_Sweep( LogStepSweep_t *v, float f0, float f1, float A, int N_total, float cycle_num, float transient_cycles, float dwell_min, float isr_freq);
float Calc_Step_Log_Sweep(LogStepSweep_t *v, float input_val, float fbk_val);
static inline void Update_Dwell_Time(LogStepSweep_t *v);
static inline void Clear_Frf_Accumulator(LogStepSweep_t *v);
float Calc_Fixed_Freq_Repeat(LogStepSweep_t *v, float input_val, float fbk_val, float speed_rpm, float angle_deg);





#endif /* PERIPHERAL_CHIRP_H_ */
