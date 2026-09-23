/*
 * identify_R.c
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#include "motor.h"
#include "Identify_R.h"

#define U_DEAD_COMP  0.96f  //U_deadtime

INJ_R identify_R = INJ_R_DEFAULTS;

void R_MACRO(INJ_R *v,MOTOR *w)
{
    v->count++;

    if(v->mode==0)//开环d轴电压方波注入
    {
        if(v->count<=1e3)
            {
                v->ud_ref = 0;
            }
        if(v->count<=2e3&&v->count>1e3)
            {
                v->ud_ref = -v->inj_ud*(v->count-1e3)/1e3;
            }
        if(v->count<=3e3&&v->count>2e3)
            {
                v->ud_ref = -v->inj_ud;
            }
        if(v->count<=4e3&&v->count>3e3)
            {
                v->ud_ref = -v->inj_ud*(1-(v->count-3e3)/1e3);
            }
    }
    else //闭环d轴电流方波注入
    {
        if(v->count<=1e3)
                {
                    v->id_ref = 0;
                }
                if(v->count<=2e3&&v->count>1e3)
                {
                    v->id_ref = -v->inj_id*(v->count-1e3)/1e3;
                }
                if(v->count<=3e3&&v->count>2e3)
                {
                    v->id_ref = -v->inj_id;
                }
                if(v->count<=4e3&&v->count>3e3)
                {
                    v->id_ref = -v->inj_id*(1-(v->count-3e3)/1e3);
                }
    }

    if(v->count<=1e3&&v->count>5e2)
        {
            float ud_actual = w->ud;

            // 根据电流方向动态修正指令电压，还原为电机两端的真实物理电压
            if(w->id > 0.05f)        ud_actual = w->ud - U_DEAD_COMP; // 电流为正，实际电压降低
            else if(w->id < -0.05f)  ud_actual = w->ud + U_DEAD_COMP; // 电流为负，实际电压升高

            v->mean_id1 = v->mean_id1+w->id*0.02f;
            v->mean_ud1 = v->mean_ud1+ud_actual*0.02f; // 使用修正后的真实电压
        }

    if(v->count<=3e3&&v->count>2.5e3)
        {
            float ud_actual = w->ud;

            // 根据电流方向动态修正指令电压
            if(w->id > 0.05f)        ud_actual = w->ud - U_DEAD_COMP;
            else if(w->id < -0.05f)  ud_actual = w->ud + U_DEAD_COMP; // 负注入时通常触发这一条

            v->mean_id2 = v->mean_id2+w->id*0.02f;
            v->mean_ud2 = v->mean_ud2+ud_actual*0.02f; // 使用修正后的真实电压
        }

    if(v->count==4e3)
    {

        v->x = (v->mean_ud2-v->mean_ud1)/(v->mean_id2-v->mean_id1);
        v->mean_ud1 = 0;
        v->mean_ud2 = 0;
        v->mean_id1 = 0;
        v->mean_id2 = 0;
        v->count = 0;
    }
}

void R_Identify(void)
{
    identify_motor.We = Pos_Speed.SpeedRpm_fr / 60.0f *2.0f * PI * PMSM.n;   //!!!!!这里后面把电机的参数都给集成到一个结构体里
    identify_motor.id = VSD_output.I_d;
    identify_motor.iq = VSD_output.I_q;
    identify_motor.ud = u_d;
    identify_motor.uq = u_q;

    R_MACRO(&identify_R,&identify_motor);

    u_d = identify_R.ud_ref; // 把辨识方波强行赋给底层使用的 u_d
    u_q = 0.0f;              // q轴保持为0
    u_x = 0.0f;
    u_y = 0.0f;
}
