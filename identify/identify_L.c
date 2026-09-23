/*
 * identify_L.c
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#include "motor.h"
#include "Identify_L.h"

INJ_L identify_L = INJ_L_DEFAULTS;


void L_MACRO(INJ_L *v, MOTOR *w)
{
    v->count++;

    float angle_pu = (float)((v->count - 1) % v->N) / 16.0f;
    v->ud_ref = v->amp * __sinpuf32(angle_pu);

    v->sum_udcos = v->sum_udcos + w->ud * __cospuf32(angle_pu);
    v->sum_idcos = v->sum_idcos + w->id * __cospuf32(angle_pu);
    v->sum_udsin = v->sum_udsin + w->ud * __sinpuf32(angle_pu);
    v->sum_idsin = v->sum_idsin + w->id * __sinpuf32(angle_pu);

    if(v->count % v->N == 0)
    {
        v->mag_ud = __sqrt(v->sum_udcos*v->sum_udcos + v->sum_udsin*v->sum_udsin);
        v->mag_id = __sqrt(v->sum_idcos*v->sum_idcos + v->sum_idsin*v->sum_idsin);

        if(v->sum_udcos != 0.0f && v->sum_udsin/(v->sum_udcos + 1e-6f) <= 1.0f && v->sum_udsin/(v->sum_udcos + 1e-6f) >= -1.0f)
            v->ang_ud = __atanpuf32(v->sum_udsin / (v->sum_udcos + 1e-6f));
        else
            v->ang_ud = 0.25f - __atanpuf32(v->sum_udcos / (v->sum_udsin + 1e-6f));

        if(v->sum_idcos != 0.0f && v->sum_idsin/(v->sum_idcos + 1e-6f) <= 1.0f && v->sum_idsin/(v->sum_idcos + 1e-6f) >= -1.0f)
            v->ang_id = __atanpuf32(v->sum_idsin / (v->sum_idcos + 1e-6f));
        else
            v->ang_id = 0.25f - __atanpuf32(v->sum_idcos / (v->sum_idsin + 1e-6f));

        v->theta = v->ang_ud - v->ang_id;

        v->L = v->mag_ud / (v->mag_id * v->fre * 2.0f * 3.1415926f / (float)v->N) * __sinpuf32(v->theta);

        v->L_mean += v->L / 500.0f;

        v->sum_udcos = 0.0f;
        v->sum_idcos = 0.0f;
        v->sum_udsin = 0.0f;
        v->sum_idsin = 0.0f;
        v->DFTCount++;

        if(v->DFTCount % 500 == 0)
        {
           v->x = v->L_mean;
           v->L_mean = 0.0f;
           v->DFTCount = 0;
           v->count = 0;
        }
    }
}


void L_Identify(void)
{
    identify_motor.We = Pos_Speed.SpeedRpm_fr / 60.0f *2.0f * PI * PMSM.n;   //!!!!!这里后面把电机的参数都给集成到一个结构体里
    identify_motor.id = VSD_output.I_d;
    identify_motor.iq = VSD_output.I_q;
    identify_motor.ud = u_d;
    identify_motor.uq = u_q;

    L_MACRO(&identify_L, &identify_motor);

    u_d = identify_L.ud_ref;  // d轴吃高频正弦波
    u_q = 0.0f;               // q轴保持静默
    u_x = 0.0f;               // x轴清零，防止六相非线性发热
    u_y = 0.0f;               // y轴清零
}


//
//void L_MACRO(INJ_L *v,MOTOR *w)
//{
//    v->count++;
//    float angle_pu = (float)((v->count - 1) % v->N) / (float)v->N;
//    v->ud_ref = v->amp * __sinpuf32(angle_pu);
//
//    v->sum_udcos = v->sum_udcos+w->ud*__cospuf32((v->count-1)/v->N);
//    v->sum_idcos = v->sum_idcos+w->id*__cospuf32((v->count-1)/v->N);
//    v->sum_udsin = v->sum_udsin+w->ud*__sinpuf32((v->count-1)/v->N);
//    v->sum_idsin = v->sum_idsin+w->id*__sinpuf32((v->count-1)/v->N);
//
//    v->mag_ud = __sqrt(v->sum_udcos*v->sum_udcos+v->sum_udsin*v->sum_udsin);
//    v->mag_id = __sqrt(v->sum_idcos*v->sum_idcos+v->sum_idsin*v->sum_idsin);
//
//    if(v->sum_udsin/v->sum_udcos<=1&&v->sum_udsin/v->sum_udcos>=-1)
//        v->ang_ud = __atanpuf32(v->sum_udsin/v->sum_udcos);
//    else
//        v->ang_ud = 0.25-__atanpuf32(v->sum_udcos/v->sum_udsin);
//    if(v->sum_idsin/v->sum_idcos<=1&&v->sum_idsin/v->sum_idcos>=-1)
//        v->ang_id = __atanpuf32(v->sum_idsin/v->sum_idcos);
//    else
//        v->ang_id = 0.25-__atanpuf32(v->sum_idcos/v->sum_idsin);
//
//    v->theta = v->ang_ud-v->ang_id;
//
//    if(v->count%v->N==0)
//    {
//        v->L = v->mag_ud/(v->mag_id*v->fre*2*PI/v->N)*__sinpuf32(v->theta);
//        v->L_mean = v->x+v->L/50;
//        v->sum_udcos = 0;
//        v->sum_idcos = 0;
//        v->sum_udsin = 0;
//        v->sum_idsin = 0;
//        v->DFTCount++;
//        if(v->DFTCount%50==0)
//        {
//           v->x = v->L_mean;
//           v->L_mean = 0;
//           v->DFTCount = 0;
//           v->count = 0;
//        }
//    }
//}


