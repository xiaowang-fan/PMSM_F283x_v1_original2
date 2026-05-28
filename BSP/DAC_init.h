/*
 * DAC_init.h
 *
 *  Created on: 2026Äê1ÔÂ26ÈÕ
 *      Author: 86135
 */

#ifndef BSP_DAC_INIT_H_
#define BSP_DAC_INIT_H_

#include "F28x_Project.h"

extern volatile struct DAC_REGS* DAC_PTR[4];

void configureDAC(Uint16 dac_num);



#endif /* BSP_DAC_INIT_H_ */
