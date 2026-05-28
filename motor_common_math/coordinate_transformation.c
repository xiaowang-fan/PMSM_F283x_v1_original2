//
// Created by Haoyi Mu on 2024/4/24.
//

#include "coordinate_transformation.h"

#define SQRT3_2 0.8660254f  // sqrt(3)/2

CT_3R2S I_CT3R2S = CT_3R2S_DEFAULTS ;
CT_2R2S I_CT2R2S = CT_2R2S_DEFAULTS ;
CT_2S2R U_CT2S2R1 = CT_2S2R_DEFAULTS ;
CT_2S2R U_CT2S2R2 = CT_2S2R_DEFAULTS ;


void CT_3R2S_Calc(CT_3R2S *p){
    p->Sine = sinf(p->Angle *2.0F *PI);
    p->Cosine = cosf(p->Angle *2.0F *PI);

    p->Alpha = p->AS;
    p->Beta = p->AS * 0.57735026918963F + p->Bs * 2 * 0.57735026918963F;  // 1/sqrt(3) = 0.57735026918963

    p->Ds = p->Alpha * p->Cosine  + p->Beta *p->Sine;
    p->Qs = p->Beta * p->Cosine   - p->Alpha *p->Sine;

}

void CT_2R2S_Calc(CT_2R2S *p){
    p->Sine = sinf(p->Angle *2.0F *PI);
    p->Cosine = cosf(p->Angle *2.0F *PI);

    p->Ds = p->Alpha * p->Cosine  + p->Beta *p->Sine;
    p->Qs = p->Beta * p->Cosine   - p->Alpha *p->Sine;

}

void CT_2S2R_Calc(CT_2S2R *p){
    p->Alpha = p->Ds * p->Cosine - p->Qs *p->Sine;
    p->Beta  = p->Qs * p->Cosine + p->Ds *p->Sine;
}

void CT_2S2R_angle_syn(CT_2S2R *p1,CT_3R2S *p2){
    p1->Sine = p2->Sine;
    p1->Cosine = p2->Cosine;
}

void VSD_Transform(float i_abc1[3], float i_abc2[3], float angle, VSD* out)
{
//    float i_alpha = (i_abc1[0] - 0.5f * i_abc1[1] - 0.5f * i_abc1[2] +\
//                     SQRT3_2 * i_abc2[0] - SQRT3_2 * i_abc2[1]) / 3 ;
//    float i_beta  = (SQRT3_2 * i_abc1[1] - SQRT3_2 * i_abc1[2] +\
//                     0.5f * i_abc2[0] + 0.5f * i_abc2[1] - i_abc2[2]) / 3 ;
//    float s = sinf(angle);
//    float c = cosf(angle);
//
//    out->I_d = i_alpha * c + i_beta * s;
//    out->I_q =-i_alpha * s + i_beta * c;

    out->I_d = (cosf(angle)*i_abc1[0] +\
           cosf(angle-2*PI/3)*i_abc1[1] +\
           cosf(angle+2*PI/3)*i_abc1[2] +\
           cosf(angle-PI/6)*i_abc2[0] +\
           cosf(angle-5*PI/6)*i_abc2[1] +\
           cosf(angle+PI/2)*i_abc2[2])/3;
    out->I_q = -1*(sinf(angle)*i_abc1[0] +\
            sinf(angle-2*PI/3)*i_abc1[1] +\
            sinf(angle+2*PI/3)*i_abc1[2] +\
            sinf(angle-PI/6)*i_abc2[0] +\
            sinf(angle-5*PI/6)*i_abc2[1]+\
            sinf(angle*+PI/2)*i_abc2[2])/3;

    out->I_x = (i_abc1[0] - 0.5f * i_abc1[1] - 0.5f * i_abc1[2] -\
                SQRT3_2 * i_abc2[0] + SQRT3_2 * i_abc2[1]) / 3 ;
    out->I_y = (-SQRT3_2 * i_abc1[1] + SQRT3_2 * i_abc1[2] +\
                0.5f * i_abc2[0] + 0.5f * i_abc2[1] - i_abc2[2]) /3 ;
}

