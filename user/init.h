/*
 * init.h
 *
 *  Created on: 2024Äê4ÔÂ14ÈÕ
 *      Author: Haoyi Mu
 */
#ifndef USER_INIT_H_
#define USER_INIT_H_

#include "F28x_Project.h"
#include "hw_memmap.h"
#include "hw_asysctl.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include "driverlib.h"
//#include "device.h"


#include "eQEP_position_speed.h"
#include "../../PMSM_F283x_v1_original2/BSP/ADC_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/epwm_init.h"
#include "../../PMSM_F283x_v1_original2/user/interrupts.h"






void init_epwm_ch1_3();
void init_control_para();
void Limiting_amplitude(float *in, float lim);
void Hysteresis_loop_compare(float *in, float threshold);

#endif /* USER_INIT_H_ */
