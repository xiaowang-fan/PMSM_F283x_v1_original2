/*
 * spi_init.h
 *
 *  Created on: 2026年1月30日
 *      Author: 86135
 */

#ifndef BSP_SPI_INIT_H_
#define BSP_SPI_INIT_H_

#include "F28x_Project.h"

void Init_spi();
void Init_spia_GPIO(void);
void spia_configuration();
void Init_AD2S1210_GPIO(void);

#define SPI_BRR        0x0012//((150000000 / 4) / 400000) - 1    //波特率 400 000

#endif /* BSP_SPI_INIT_H_ */
