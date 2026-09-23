/*
 * chirp.c
 *
 * 工业级在线SFRA扫频器
 *
 * 特性：
 * 1. 对数分段扫频
 * 2. 固定周期数积分（工业标准）
 * 3. 在线同步检波
 * 4. 自动计算：
 *      - 幅值(dB)
 *      - 相位(deg)
 * 5. 自动跳过前N个周期暂态
 * 6. 自动切换频率
 * 7. 自动输出FRF结果
 *
 * 推荐：
 * cycle_num = 20
 * transient_cycles = 5
 *
 */

#include "chirp.h"
#include <math.h>


#define SWEEP_FREQ_EPS          (1.0e-6f)

/* 防止复数幅值或除数为0 */
#define SWEEP_MAG_EPS           (1.0e-12f)

/* 防止功率计算除以0 */
#define SWEEP_POWER_EPS         (1.0e-20f)

/* 最少有效采样点 */
#define SWEEP_MIN_SAMPLES       (32UL)

/*
 * 单频分量占输出总交流功率的最低比例。
 * 初始建议0.5，可根据实验调整。
 */
#define SWEEP_QUALITY_MIN       (0.50f)

/* 无效FRF频点的幅值标记 */
#define SWEEP_INVALID_MAG_DB    (-300.0f)


#define CHIRP_PI 3.14159265358979f
#define CHIRP_RAD_TO_DEG     (57.29577951308232f)
#define SWEEP_MAG_EPS        (1.0e-12f)


/* =========================================================
 * 全局对象
 * ========================================================= */

LogStepSweep_t g_speed_sweep;
LogStepSweep_t g_current_sweep;

/* =========================================================
 * 初始化扫频器
 * ========================================================= */

static inline void Update_Dwell_Time(LogStepSweep_t *v)
{
    float measure_cycles;
        float min_cycles_by_time;

        if(v->freq_current <= SWEEP_FREQ_EPS)
        {
            v->transient_time = 0.0f;
            v->measure_time = 0.0f;
            v->dwell_time = 0.0f;
            return;
        }

        v->transient_time = v->transient_cycles / v->freq_current;

        measure_cycles = v->cycle_num;

        if(measure_cycles < 1.0f)
        {
            measure_cycles = 1.0f;
        }

        min_cycles_by_time = ceilf(v->dwell_min * v->freq_current);

        if(min_cycles_by_time < 1.0f)
        {
            min_cycles_by_time = 1.0f;
        }

        if(measure_cycles < min_cycles_by_time)
        {
            measure_cycles = min_cycles_by_time;
        }

        v->measure_time = measure_cycles / v->freq_current;

        v->dwell_time = v->transient_time + v->measure_time;
}


void Init_Log_Step_Sweep(LogStepSweep_t *v, float f0, float f1, float A, Uint16 N_total, float cycle_num, float transient_cycles, float dwell_min, float isr_freq)
{
    v->f0 = f0;
    v->f1 = f1;
    v->A = A;

    v->N_total = N_total;
    v->cycle_num = cycle_num;
    v->transient_cycles = transient_cycles;
    v->dwell_min = dwell_min;

    v->ts = 1.0f / isr_freq;

    v->freq_current = f0;
    v->current_step = 0U;

    if((N_total > 1U) && (f0 > 0.0f) && (f1 > 0.0f))
    {
        v->log_factor =  powf(  f1 / f0, 1.0f / (float)(N_total - 1U));
    }
    else
    {
        v->log_factor = 1.0f;
    }

    Update_Dwell_Time(v);

    v->phase = 0.0f;
    v->step_time_count = 0.0f;

    v->measure_started = 0U;
    v->is_running = 1U;

    v->ref_cos_sum = 0.0f;
    v->ref_sin_sum = 0.0f;
    v->fbk_cos_sum = 0.0f;
    v->fbk_sin_sum = 0.0f;
    v->sample_cnt = 0UL;

    v->result_mag_dB = 0.0f;
    v->result_phase_deg = 0.0f;
    v->result_freq = 0.0f;

    v->fbk_sum = 0.0f;
    v->fbk_sq_sum = 0.0f;

    v->result_input_amp = 0.0f;
    v->result_output_amp = 0.0f;
    v->result_quality = 0.0f;

    v->result_valid_flag = 0U;

    v->result_ready_flag = 0U;
    v->finished_flag = 0U;
}

float Calc_Step_Log_Sweep(LogStepSweep_t *v, float input_val, float fbk_val)
{
    float out_disturbance;
    float sin_now;
    float cos_now;
    float inv_N;

    float X_real;
    float X_imag;
    float Y_real;
    float Y_imag;

    float X_mag;
    float Y_mag;

    float denominator;
    float H_real;
    float H_imag;
    float H_mag;

    float fbk_mean;
    float fbk_total_power;
    float fbk_tone_power;
    float quality;

    if(v->result_ready_flag == 1U)
    {
        return 0.0f;
    }

    if(v->is_running == 0U)
    {
        return 0.0f;
    }

    sin_now = sinf(v->phase);
    cos_now = cosf(v->phase);

    out_disturbance = v->A * sin_now;

    if(v->measure_started == 0U)
    {

        v->step_time_count += v->ts;

        if(v->step_time_count >= v->transient_time)
        {
            v->measure_started = 1U;

            v->step_time_count = v->transient_time;

            v->ref_cos_sum = 0.0f;
            v->ref_sin_sum = 0.0f;

            v->fbk_cos_sum = 0.0f;
            v->fbk_sin_sum = 0.0f;

            v->fbk_sum = 0.0f;
            v->fbk_sq_sum = 0.0f;

            v->sample_cnt = 0UL;
        }
    }
    else
    {

        v->ref_cos_sum += input_val * cos_now;
        v->ref_sin_sum += input_val * sin_now;

        v->fbk_cos_sum += fbk_val * cos_now;
        v->fbk_sin_sum += fbk_val * sin_now;

        v->fbk_sum += fbk_val;
        v->fbk_sq_sum += fbk_val * fbk_val;

        v->sample_cnt++;
        v->step_time_count += v->ts;
    }

    v->phase += 2.0f * CHIRP_PI * v->freq_current * v->ts;

    if(v->phase >= 2.0f * CHIRP_PI)
    {
        v->phase -= 2.0f * CHIRP_PI;
    }

    if((v->measure_started == 0U) || (v->step_time_count < v->dwell_time))
    {
        return out_disturbance;
    }

    v->result_freq = v->freq_current;

    v->result_mag_dB = SWEEP_INVALID_MAG_DB;
    v->result_phase_deg = 0.0f;

    v->result_input_amp = 0.0f;
    v->result_output_amp = 0.0f;
    v->result_quality = 0.0f;

    v->result_valid_flag = 0U;

    if(v->sample_cnt >= SWEEP_MIN_SAMPLES)
    {
        inv_N = 1.0f / (float)v->sample_cnt;

        X_real = v->ref_cos_sum * inv_N;
        X_imag = -v->ref_sin_sum * inv_N;

        Y_real = v->fbk_cos_sum * inv_N;
        Y_imag = -v->fbk_sin_sum * inv_N;

        X_mag = sqrtf( X_real * X_real + X_imag * X_imag);

        Y_mag = sqrtf( Y_real * Y_real + Y_imag * Y_imag);

        v->result_input_amp = 2.0f * X_mag;
        v->result_output_amp = 2.0f * Y_mag;

        fbk_mean = v->fbk_sum * inv_N;

        fbk_total_power = v->fbk_sq_sum * inv_N - fbk_mean * fbk_mean;

        if(fbk_total_power < 0.0f)
        {
            fbk_total_power = 0.0f;
        }

        fbk_tone_power = 2.0f * Y_mag * Y_mag;

        quality = fbk_tone_power / (fbk_total_power + SWEEP_POWER_EPS);

        if(quality > 1.0f)
        {
            quality = 1.0f;
        }
        else if(quality < 0.0f)
        {
            quality = 0.0f;
        }

        v->result_quality = quality;

        denominator = X_real * X_real + X_imag * X_imag;

        if((X_mag > SWEEP_MAG_EPS) && (denominator > SWEEP_MAG_EPS * SWEEP_MAG_EPS))
        {
            H_real = (Y_real * X_real + Y_imag * X_imag) / denominator;

            H_imag = (Y_imag * X_real - Y_real * X_imag) / denominator;

            H_mag = sqrtf( H_real * H_real + H_imag * H_imag);

            if(H_mag < SWEEP_MAG_EPS)
            {
                H_mag = SWEEP_MAG_EPS;
            }

            v->result_mag_dB = 20.0f * log10f(H_mag);

            v->result_phase_deg = atan2f(H_imag, H_real) * CHIRP_RAD_TO_DEG;

            if((Y_mag > SWEEP_MAG_EPS) && (quality >= SWEEP_QUALITY_MIN))
            {
                v->result_valid_flag = 1U;
            }
        }
    }

    v->result_ready_flag = 1U;

    v->ref_cos_sum = 0.0f;
    v->ref_sin_sum = 0.0f;

    v->fbk_cos_sum = 0.0f;
    v->fbk_sin_sum = 0.0f;

    v->fbk_sum = 0.0f;
    v->fbk_sq_sum = 0.0f;

    v->sample_cnt = 0UL;

    v->step_time_count = 0.0f;
    v->measure_started = 0U;

    v->current_step++;

    if(v->current_step < v->N_total)
    {
        if(v->current_step == (v->N_total - 1U))
        {
            v->freq_current = v->f1;
        }
        else
        {
            v->freq_current *= v->log_factor;
        }
        v->phase = 0.0f;

        Update_Dwell_Time(v);
    }
    else
    {
        v->is_running = 0U;
        v->finished_flag = 1U;
    }

    return 0.0f;
}

static inline void Clear_Frf_Accumulator(LogStepSweep_t *v)
{
    v->ref_cos_sum = 0.0f;
    v->ref_sin_sum = 0.0f;

    v->fbk_cos_sum = 0.0f;
    v->fbk_sin_sum = 0.0f;

    v->fbk_sum = 0.0f;
    v->fbk_sq_sum = 0.0f;

    /* 新增 */
    v->speed_sum = 0.0f;
    v->angle_sum = 0.0f;

    v->sample_cnt = 0UL;
}

float Calc_Fixed_Freq_Repeat( LogStepSweep_t *v, float input_val, float fbk_val, float speed_rpm, float angle_deg)
{
    float out_disturbance;
    float sin_now;
    float cos_now;
    float inv_N;

    float X_real;
    float X_imag;
    float Y_real;
    float Y_imag;

    float X_mag;
    float Y_mag;

    float denominator;
    float H_real;
    float H_imag;
    float H_mag;

    float fbk_mean;
    float fbk_total_power;
    float fbk_tone_power;
    float quality;

    if(v->is_running == 0U)
    {
        return 0.0f;
    }

    sin_now = sinf(v->phase);
    cos_now = cosf(v->phase);

    out_disturbance = v->A * sin_now;

    v->phase += 2.0f * CHIRP_PI * v->freq_current * v->ts;

    if(v->phase >= 2.0f * CHIRP_PI)
    {
        v->phase -= 2.0f * CHIRP_PI;
    }

    if(v->result_ready_flag == 1U)
    {
        return out_disturbance;
    }

    if(v->measure_started == 0U)
    {
        v->step_time_count += v->ts;

        if(v->step_time_count >= v->transient_time)
        {
            v->measure_started = 1U;

            v->step_time_count = 0.0f;

            Clear_Frf_Accumulator(v);
        }

        return out_disturbance;
    }

    v->ref_cos_sum += input_val * cos_now;
    v->ref_sin_sum += input_val * sin_now;

    v->fbk_cos_sum += fbk_val * cos_now;
    v->fbk_sin_sum += fbk_val * sin_now;

    v->fbk_sum += fbk_val;
    v->fbk_sq_sum += fbk_val * fbk_val;

    v->speed_sum += speed_rpm;
    v->angle_sum += angle_deg;

    v->sample_cnt++;
    v->step_time_count += v->ts;

    if(v->step_time_count < v->measure_time)
    {
        return out_disturbance;
    }

    v->result_freq = v->freq_current;

    v->result_mag_dB = -300.0f;
    v->result_phase_deg = 0.0f;

    v->result_input_amp = 0.0f;
    v->result_output_amp = 0.0f;
    v->result_quality = 0.0f;

    v->result_speed_mean_rpm = 0.0f;
    v->result_angle_mean_deg = 0.0f;

    if(v->sample_cnt > 0UL)
    {
        inv_N = 1.0f / (float)v->sample_cnt;

        v->result_speed_mean_rpm = v->speed_sum * inv_N;

        v->result_angle_mean_deg = v->angle_sum * inv_N;

        X_real = v->ref_cos_sum * inv_N;
        X_imag = -v->ref_sin_sum * inv_N;

        Y_real = v->fbk_cos_sum * inv_N;
        Y_imag = -v->fbk_sin_sum * inv_N;

        X_mag = sqrtf( X_real * X_real + X_imag * X_imag);

        Y_mag = sqrtf( Y_real * Y_real + Y_imag * Y_imag);

        v->result_input_amp = 2.0f * X_mag;
        v->result_output_amp = 2.0f * Y_mag;

        denominator = X_real * X_real + X_imag * X_imag;

        if(denominator > 1.0e-20f)
        {

            H_real = (Y_real * X_real + Y_imag * X_imag) / denominator;

            H_imag = (Y_imag * X_real - Y_real * X_imag) / denominator;

            H_mag = sqrtf( H_real * H_real + H_imag * H_imag);

            if(H_mag < 1.0e-12f)
            {
                H_mag = 1.0e-12f;
            }

            v->result_mag_dB = 20.0f * log10f(H_mag);

            v->result_phase_deg = atan2f(H_imag, H_real) * CHIRP_RAD_TO_DEG;
        }

        fbk_mean = v->fbk_sum * inv_N;

        fbk_total_power = v->fbk_sq_sum * inv_N - fbk_mean * fbk_mean;

        if(fbk_total_power < 0.0f)
        {
            fbk_total_power = 0.0f;
        }

        fbk_tone_power = 2.0f * Y_mag * Y_mag;

        quality = fbk_tone_power / (fbk_total_power + 1.0e-20f);

        if(quality > 1.0f)
        {
            quality = 1.0f;
        }
        else if(quality < 0.0f)
        {
            quality = 0.0f;
        }

        v->result_quality = quality;
    }

    v->result_ready_flag = 1U;

    v->current_step++;

    Clear_Frf_Accumulator(v);

    v->step_time_count = 0.0f;

    if(v->current_step >= v->N_total)
    {

        v->finished_flag = 1U;
        v->is_running = 0U;

        return 0.0f;
    }

    return out_disturbance;
}








//void Init_Log_Step_Sweep( LogStepSweep_t *v, float f0, float f1, float A, int N_total, float cycle_num, float transient_cycles, float dwell_min, float isr_freq)
//{
//    /* ----------------------------------------------------- * 用户参数 * ----------------------------------------------------- */
//    v->f0 = f0;
//    v->f1 = f1;
//    v->A = A;
//    v->N_total = N_total;
//    v->cycle_num = cycle_num;
//    v->transient_cycles = transient_cycles;
//    /* ----------------------------------------------------- * 高频最小停留时间 * ----------------------------------------------------- */
//    v->dwell_min = dwell_min;
//    /* ----------------------------------------------------- * ISR周期 * ----------------------------------------------------- */
//    v->ts = 1.0f / isr_freq;
//    /* ----------------------------------------------------- * 初始频率 * ----------------------------------------------------- */
//    v->freq_current = f0;
//    /* ----------------------------------------------------- * 更新停留时间 * ----------------------------------------------------- */
//    Update_Dwell_Time(v);
//    /* ----------------------------------------------------- * 状态变量 * ----------------------------------------------------- */
//    v->phase = 0.0f;
//    v->time_count = 0.0f;
//    v->step_time_count = 0.0f;
//    v->current_step = 0;
//    v->is_running = 1;
//    /* ----------------------------------------------------- * 在线DFT积分器 * ----------------------------------------------------- */
//    v->ref_cos_sum = 0.0f;
//    v->ref_sin_sum = 0.0f;
//    v->fbk_cos_sum = 0.0f;
//    v->fbk_sin_sum = 0.0f;
//    v->sample_cnt = 0;
//    /* ----------------------------------------------------- * 输出结果 * ----------------------------------------------------- */
//    v->result_mag_dB = 0.0f;
//    v->result_phase_deg = 0.0f;
//    v->result_freq = 0.0f;
//    v->result_ready_flag = 0;
//}
/* =========================================================
 * 核心扫频函数
 *
 * 输入：
 *      fbk_val = 当前反馈值
 *
 * 输出：
 *      当前扰动
 * ========================================================= */
//float Calc_Step_Log_Sweep( LogStepSweep_t *v, float fbk_val)
//{
//    float out_disturbance = 0.0f;
//    /* ----------------------------------------------------- * 未运行 * ----------------------------------------------------- */
//    if(!v->is_running)
//    {
//        return 0.0f;
//    }
//    /* ----------------------------------------------------- * 当前相位 * ----------------------------------------------------- */
//    float phase_now = v->phase;
//    /* ----------------------------------------------------- * 扰动输出 * ----------------------------------------------------- */
//    float sin_now = sinf(phase_now);
//    float cos_now = cosf(phase_now);
//    out_disturbance = v->A * sin_now;
//    /* ----------------------------------------------------- * 时间累计 * ----------------------------------------------------- */
//    v->time_count += v->ts;
//    v->step_time_count += v->ts;
//    /* ----------------------------------------------------- * 跳过暂态 * ----------------------------------------------------- */
//    if(v->step_time_count >= v->transient_time)
//    {
//        /* ------------------------------------------------- * 在线同步检波 * ------------------------------------------------- */
//        v->ref_cos_sum += out_disturbance * cos_now;
//        v->ref_sin_sum += out_disturbance * sin_now;
//        v->fbk_cos_sum += fbk_val * cos_now;
//        v->fbk_sin_sum += fbk_val * sin_now;
//        v->sample_cnt++;
//    }
//    /* ----------------------------------------------------- * 相位更新 * ----------------------------------------------------- */
//    v->phase += 2.0f * CHIRP_PI * v->freq_current * v->ts;
//    /* ----------------------------------------------------- * 相位限制 * ----------------------------------------------------- */
//    if(v->phase >= 2.0f * CHIRP_PI)
//    {
//        v->phase -= 2.0f * CHIRP_PI;
//    }
//    /* ----------------------------------------------------- * 当前频点结束 * ----------------------------------------------------- */
//    if(v->step_time_count >= v->dwell_time)
//    {
//        /* ------------------------------------------------- * 最少积分点保护 * * 高频必须有足够DFT点数 * ------------------------------------------------- */
//        if(v->sample_cnt > 200)
//        {
//            /* ============================================= * 输入复数谱 * ============================================= */
//            float X_real = v->ref_cos_sum / (float)v->sample_cnt;
//            float X_imag = -v->ref_sin_sum / (float)v->sample_cnt;
//            /* ============================================= * 输出复数谱 * ============================================= */
//            float Y_real = v->fbk_cos_sum / (float)v->sample_cnt;
//            float Y_imag = -v->fbk_sin_sum / (float)v->sample_cnt;
//            /* ============================================= * 幅值 * ============================================= */
//            float X_mag = sqrtf( X_real * X_real + X_imag * X_imag);
//            float Y_mag = sqrtf( Y_real * Y_real + Y_imag * Y_imag);
//            /* ============================================= * 防止除0 * ============================================= */
//            if(X_mag > 1e-6f)
//            {
//                /* ========================================= * 相位 * ========================================= */
//                float X_phase = atan2f( X_imag, X_real);
//                float Y_phase = atan2f( Y_imag, Y_real);
//                /* ========================================= * 幅值(dB) * ========================================= */
//                v->result_mag_dB = 20.0f * log10f( Y_mag / X_mag);
//                /* ========================================= * 相位差 * ========================================= */
//                float phase_diff = Y_phase - X_phase;
//                /* ========================================= * wrap到[-PI,PI] * ========================================= */
//                while(phase_diff > CHIRP_PI)
//                {
//                    phase_diff -= 2.0f * CHIRP_PI;
//                }
//                while(phase_diff < -CHIRP_PI)
//                {
//                    phase_diff += 2.0f * CHIRP_PI;
//                }
//                /* ========================================= * 转换为角度 * ========================================= */
//                v->result_phase_deg = phase_diff * 180.0f / CHIRP_PI;
//                /* ========================================= * 当前频率 * ========================================= */
//                v->result_freq = v->freq_current;
//                /* ========================================= * 通知上位机 * ========================================= */
//                v->result_ready_flag = 1;
//            }
//        }
//        /* ------------------------------------------------- * 清积分器 * ------------------------------------------------- */
//        v->ref_cos_sum = 0.0f;
//        v->ref_sin_sum = 0.0f;
//        v->fbk_cos_sum = 0.0f;
//        v->fbk_sin_sum = 0.0f;
//        v->sample_cnt = 0;
//        /* ------------------------------------------------- * 时间复位 * ------------------------------------------------- */
//        v->step_time_count = 0.0f;
//        /* ------------------------------------------------- * 下一频点 * ------------------------------------------------- */
//        v->current_step++;
//        if(v->current_step < v->N_total)
//        {
//            float power_exp = (float)v->current_step / (float)(v->N_total - 1);
//            /* ============================================= * 对数频率更新 * ============================================= */
//            v->freq_current = v->f0 * powf( v->f1 / v->f0, power_exp);
//            /* ============================================= * 更新停留时间 * ============================================= */
//            Update_Dwell_Time(v);
//        }
//        else
//        {
//            /* ============================================= * 扫频结束 * ============================================= */
//            v->is_running = 0; v->result_freq = -1.0f;
//        }
//    }
//    return out_disturbance;
//}
