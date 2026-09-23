/*
 * motor_control_algorithm.c
 *
 *  Created on: 2026Äê6ÔÂ8ÈÕ
 *      Author: 86135
 */

#include "motor_control_algorithm.h"


SpeedRamp_t backlash_speed_ramp;

void SpeedRamp_Init(SpeedRamp_t *ramp, float initial_speed, float accel, float decel, float sample_time)
{
    if (ramp == NULL)
    {
        return;
    }

    ramp->output = initial_speed;
    ramp->accel = fabsf(accel);
    ramp->decel = fabsf(decel);
    ramp->sample_time = sample_time;
}



float SpeedRamp_Update(SpeedRamp_t *ramp, float target_speed)
{
    float step;
    float delta;

    if (ramp == NULL || ramp->sample_time <= 0.0f)
    {
        return 0.0f;
    }

    if (ramp->output * target_speed < 0.0f)
    {
        step = ramp->decel * ramp->sample_time;

        if (ramp->output > step)
        {
            ramp->output -= step;
        }
        else if (ramp->output < -step)
        {
            ramp->output += step;
        }
        else
        {
            ramp->output = 0.0f;
        }

        return ramp->output;
    }

    if (fabsf(target_speed) > fabsf(ramp->output))
    {
        step = ramp->accel * ramp->sample_time;
    }
    else
    {
        step = ramp->decel * ramp->sample_time;
    }

    delta = target_speed - ramp->output;

    if (delta > step)
    {
        ramp->output += step;
    }
    else if (delta < -step)
    {
        ramp->output -= step;
    }
    else
    {
        ramp->output = target_speed;
    }

    return ramp->output;
}


