/*
 * eQEP_position_speed.h
 *
 *  Created on: 2024Äê4ÔÂ14ÈÕ
 *      Author: Haoyi Mu
 */

#ifndef PERIPHERAL_EQEP_POSITION_SPEED_H_
#define PERIPHERAL_EQEP_POSITION_SPEED_H_

#include <math.h>

//
// Defines
//
#define POSSPEED_DEFAULTS {0, 0, 0, 0, 1e-4, \
                           0, 0, 10000, 5, 750,\
                           0.975, 0.025, \
                           0,0,0,\
                           (void (*)(long))POSSPEED_Init,\
                           (void (*)(long))POSSPEED_Calc }   //1860

//370
// Globals
//
typedef struct {float theta_elec;         // Output: Motor Electrical angle
                float theta_mech;         // Output: Motor Mechanical Angle
                int DirectionQep;       // Output: Motor rotation direction
                int theta_raw;          // Variable: Raw angle
                float mech_scaler;        // Parameter: 1/total count, total
                                        // count = 1e4
                long rev_cnt;
                float theta_pos;

                int N_line;
                int pole_pairs;         // Parameter: Number of pole pairs
                int cal_angle;          // Parameter: Raw angular offset
                                        // between encoder and phase A
                float k1;
                float k2;

                int oldpos;            // Input: Electrical angle (pu)
                float SpeedRpm_fr;      // Output : Speed in rpm  (Q0) -
                                        // independently with global Q
                float speed_send_rpm;
                void (*init)();         // Pointer to the init funcion
                void (*calc)();         // Pointer to the calc funtion
                }POSSPEED;

typedef POSSPEED *POSSPEED_handle;

//
// Function Prototypes
//
void POSSPEED_Init(POSSPEED_handle);
void POSSPEED_Calc(POSSPEED_handle);


#endif /* PERIPHERAL_EQEP_POSITION_SPEED_H_ */
