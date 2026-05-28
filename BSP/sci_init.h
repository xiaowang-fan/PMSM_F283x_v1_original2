/*
 * sci_init.h
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#ifndef BSP_SCI_INIT_H_
#define BSP_SCI_INIT_H_

#include "F28x_Project.h"

void Init_sci();
void Init_scia_GPIO(void);
void Init_scia_fifo(void);
void scia_configuration(void);
void Init_scib_GPIO(void);
void Init_scib_fifo(void);
void scib_configuration(void);
void Init_scic_GPIO(void);
void Init_scic_fifo(void);
void scic_configuration(void);




#endif /* BSP_SCI_INIT_H_ */
