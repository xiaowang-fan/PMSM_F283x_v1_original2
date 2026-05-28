/*
 * ADC_init.c
 *
 *  Created on: 2024��4��18��
 *      Author: Haoyi Mu
 */
#ifndef PERIPHERAL_ADC_INIT_C_
#define PERIPHERAL_ADC_INIT_C_


#include "F28x_Project.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_adc.h"
#include "hw_sysctl.h"
#include "hw_memmap.h"
#include "hw_asysctl.h"


#define EPWM1_SOCA 5;
#define EPWM1_SOCB 6;
#define EPWM2_SOCA 7;
#define EPWM2_SOCB 8;
#define EPWM3_SOCA 9;
#define EPWM3_SOCB 10;



void ConfigureADC(void);
void Configure_single_ADC(volatile struct ADC_REGS *ADCR, Uint16 adc, Uint16 resolution, Uint16 signalmode);
void SetupADCSoftware(void);



#endif /* PERIPHERAL_ADC_INIT_C_ */
