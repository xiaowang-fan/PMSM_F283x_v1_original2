/*
 * bsp_init.h
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#ifndef USER_BSP_INIT_H_
#define USER_BSP_INIT_H_

#include <BSP/gpio_output_init.h>

#include "F28x_Project.h"

#include "../../PMSM_F283x_v1_original2/BSP/ADC_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/can_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/clocks_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/DAC_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/epwm_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/EQep_Gpio_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/piectrl_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/protect_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/sci_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/spi_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/led_init.h"
#include "../../PMSM_F283x_v1_original2/BSP/gpio_output_init.h"

void BSP_Init();


#endif /* USER_BSP_INIT_H_ */
