//
// Created by Haoyi Mu on 2024/4/24.
//

#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include <math.h>
extern float PI;

#define CT_3R2S_DEFAULTS  {0,0, 0,0, 0,0,0, 0,0, (void (*)(long))CT_3R2S_Calc }

typedef struct {
    float  AS;          // Input: phase-a stator variable
    float  Bs;          // Input: phase-b stator variable
    float  Alpha;  		// variable: alpha stator variable
    float  Beta;	 	// variable: beta  stator variable

    float  Angle;		// Input: rotating angle (pu)
    float  Sine;        // variable
    float  Cosine;      // variable

    float  Ds;			// Output: rotating d-axis stator variable
    float  Qs;			// Output: rotating q-axis stator variable
    void  (*calc)();         // Pointer to the calc funtion
}CT_3R2S;

typedef CT_3R2S *CT_3R2S_handle;
void CT_3R2S_Calc(CT_3R2S_handle);

#define CT_2S2R_DEFAULTS    {0,0, 0,0, 0,0, \
                                (void (*)(long))CT_2S2R_Calc, \
                                (void (*)(long))CT_2S2R_angle_syn   }

typedef struct {
    float Alpha;        // Output: alpha stator variable
    float  Beta;        // Output: beta stator variable
    float  Sine;        // Input:
    float  Cosine;      // Input:
    float  Ds;          // Input: stationary d-axis stator variable
    float  Qs;          // Input: stationary q-axis stator variable
    void  (*calc)();         // Pointer to the calc funtion
    void  (*angle_syn)();    // Pointer to the angle syn funtion
}CT_2S2R;
typedef CT_2S2R *CT_2S2R_handle;



void CT_2S2R_Calc(CT_2S2R_handle);
void CT_2S2R_angle_syn(CT_2S2R_handle,CT_3R2S_handle);

#define CT_2R2S_DEFAULTS  {0,0, 0,0,0, 0,0, (void (*)(long))CT_2R2S_Calc }

typedef struct
{
    float  Alpha;         // variable: alpha stator variable
    float  Beta;        // variable: beta  stator variable

    float  Angle;       // Input: rotating angle (pu)
    float  Sine;        // variable
    float  Cosine;      // variable

    float  Ds;          // Output: rotating d-axis stator variable
    float  Qs;          // Output: rotating q-axis stator variable

    void  (*calc)();         // Pointer to the calc funtion
}CT_2R2S;

typedef CT_2R2S *CT_2R2S_handle;
void CT_2R2S_Calc(CT_2R2S_handle);

extern CT_3R2S I_CT3R2S;
extern CT_2R2S I_CT2R2S;
extern CT_2S2R U_CT2S2R1;
extern CT_2S2R U_CT2S2R2;

typedef struct
{
    float I_d, I_q;
    float I_x, I_y;
}VSD;

extern VSD VSD_output;

void VSD_Transform(float i_abc1[3], float i_abc2[3], float angle, VSD* out);


#endif //COORDINATE_TRANSFORMATION_H
