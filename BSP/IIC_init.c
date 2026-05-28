/*
 * IIC_init.c
 *
 *  Created on: 2026年1月30日
 *      Author: 86135
 */



#include "../../PMSM_F283x_v1_original2/BSP/IIC_init.h"


void IIC_Init(void)
{
    EALLOW;
//    CpuSysRegs.PCLKCR3.bit.TBCLKSYNC = 0;


    /* SDA */

    GPIO_SetupPinMux(32, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(32, GPIO_INPUT, GPIO_ASYNC | GPIO_PULLUP);

    /* SCL */

    GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(33, GPIO_INPUT, GPIO_ASYNC | GPIO_PULLUP);

    EDIS;
}

void SDA_OUT(void)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    EDIS;
}

void SDA_IN(void)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;
    EDIS;
}
//SCL为时钟信号，低通俗来讲是关闭总线
//高若SDA不变，是数据传输；若SDA电平变化，是起始和停止。
void IIC_Start(void)
{
    EALLOW;

    SDA_OUT();
    IIC_SCL_SETH;
    IIC_SDA_SETH;
    DELAY_US(5);
    IIC_SDA_SETL;
    DELAY_US(6);
    IIC_SCL_SETL;               //钳住IIC总线，准备发送或接收数据

    EDIS;
}

void IIC_Stop(void)
{
    EALLOW;

    SDA_OUT();
    IIC_SCL_SETL;
    IIC_SDA_SETL;
    IIC_SCL_SETH;
    DELAY_US(5);
    IIC_SDA_SETH;
    DELAY_US(6);

    EDIS;
}

unsigned char IIC_Wait_Ack(void)
{
    unsigned char tempTime = 0;

    EALLOW;

    IIC_SDA_SETH;               //线与，接收SDA为低电平时可以判断
    DELAY_US(1);
    SDA_IN();
    IIC_SCL_SETH;
    DELAY_US(1);

    EDIS;
    while(READ_SDA)             //SDA为低电平是应答
    {
        tempTime++;
        if(tempTime > 250)
        {
            IIC_Stop();
            return 1;           //接收应答失败
        }
    }
    IIC_SCL_SETL;               //钳住IIC总线，方便下次传输
    return 0;                   //接收应答成功
}

void IIC_Ack(void)              //主机向从机发送应答信号
{
    EALLOW;

    IIC_SCL_SETL;
    SDA_OUT();
    IIC_SDA_SETL;
    IIC_SCL_SETH;
    DELAY_US(2);
    IIC_SCL_SETL;

    EDIS;
}

void IIC_NAck(void)             //主机向从机发送不要应答信号
{
    EALLOW;

   IIC_SCL_SETL;
   SDA_OUT();
   IIC_SDA_SETH;
   IIC_SCL_SETH;                //第9个时钟
   DELAY_US(2);
   IIC_SCL_SETL;

   EDIS;
}

void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;
    SDA_OUT();
    IIC_SCL_SETL;                   //SCL拉低，此时SDA数据可以变化
    for(t = 0;t < 8;t++)
    {
        if(txd & 0x80)            //0x80  10000000
        {
            IIC_SDA_SETH;
        }
        else
        {
            IIC_SDA_SETL;
        }
        txd <<= 1;
        DELAY_US(2);
        IIC_SCL_SETH;
        DELAY_US(2);
        IIC_SCL_SETL;
        DELAY_US(2);
    }
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
   unsigned char i,receive = 0;
   SDA_IN();
   for(i = 0;i < 8;i++)
   {
       IIC_SCL_SETL;
       DELAY_US(2);
       IIC_SCL_SETH;
       receive <<= 1;
       if(READ_SDA)
       {
           receive++;
           DELAY_US(1);
       }
   }
   if (!ack)
          IIC_NAck();
      else
          IIC_Ack();
      return receive;
}



