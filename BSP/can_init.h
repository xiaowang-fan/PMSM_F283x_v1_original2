/*
 * can_init.h
 *
 *  Created on: 2026Äê1ÔÂ27ÈÕ
 *      Author: 86135
 */

#ifndef BSP_CAN_INIT_H_
#define BSP_CAN_INIT_H_

#include "F28x_Project.h"
#include "hw_can.h"

void Init_can();
void Init_canA_GPIO();
void Init_canB_GPIO();
void canA_configuration(void);
void canB_configuration(void);


#endif /* BSP_CAN_INIT_H_ */





