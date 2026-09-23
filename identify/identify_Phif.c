/*
 * identify_Phif.c
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#include "motor.h"
#include "Identify_Phif.h"

RLS_Phif identify_Phif = RLS_Phif_DEFAULTS;

void Phif_Macro(RLS_Phif *v,MOTOR *w)
{
    v->y = w->uq-w->We*w->L*w->id-w->r*w->iq;
    v->psi = w->We;
    v->k = (v->p*v->psi)/(v->lamda+v->psi*v->p*v->psi);
    v->p = (1-v->k*v->psi)*v->p/v->lamda;
    v->x = v->x+v->k*(v->y-v->psi*v->x);
}

void Phif_Identify(void)
{
    identify_motor.We = Pos_Speed.SpeedRpm_fr / 60.0f *2.0f * PI * PMSM.n;   //!!!!!这里后面把电机的参数都给集成到一个结构体里
    identify_motor.id = VSD_output.I_d;
    identify_motor.iq = VSD_output.I_q;
    identify_motor.ud = u_d;
    identify_motor.uq = u_q;

    if(fabsf(identify_motor.We) > 65.0f)
    {
    Phif_Macro(&identify_Phif, &identify_motor);
    }
    else
    {
        identify_Phif.p = 1e4f;
        identify_Phif.k = 0.0f;
    }

}


