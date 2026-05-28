/*
 * IIC_init.h
 *
 *  Created on: 2026Äê1ÔÂ30ÈÕ
 *      Author: 86135
 */

#ifndef BSP_IIC_INIT_H_
#define BSP_IIC_INIT_H_



#include "F28x_Project.h"

#define  IIC_SCL_SETL  (GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1)
#define  IIC_SCL_SETH  (GpioDataRegs.GPBSET.bit.GPIO33 = 1)

#define  IIC_SDA_SETL  (GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1)
#define  IIC_SDA_SETH  (GpioDataRegs.GPBSET.bit.GPIO32 = 1)

#define  READ_SDA      (GpioDataRegs.GPBDAT.bit.GPIO32)

void IIC_Init(void);
void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);




#endif /* BSP_IIC_INIT_H_ */
