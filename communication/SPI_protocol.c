/*
 * SPI_protocol.c
 *
 *  Created on: 2026年1月30日
 *      Author: 86135
 */

#include "SPI_protocol.h"

int16 motor_position;      // 电机位置
Uint16 angle_offset = 0; //1400是张哥老电机旋变对0位置偏置,2041是55kW老电机零位，2068是杨老师的新电机，3375是黄师兄的电机,0是12kW出厂电机
float electrical_position; // 电角度
float Angle_pu;            // 电机电角度标幺

void Motor_Position_1210() // 1210芯片读取旋变位置
{
    // 1210初始化
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO12 = 1; // 拉低数据转移控制信号
    DELAY_US(3);
    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1; // 拉低SAMPLE
    DELAY_US(3);
    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1; // 拉低SPI片选信号
    EDIS;

    SpiaRegs.SPITXBUF = 0x0552; // SPI发数
    while (SpiaRegs.SPIFFRX.bit.RXFFST != 1)
    {
    }
    motor_position = ((SpiaRegs.SPIRXBUF) & 0xffc0) >> 6;        // SPI读取位置信号  motor_position旋变电角度
    electrical_position = (motor_position + angle_offset) % 1024; // electrical_position电机电角度
    Angle_pu = electrical_position / (float)(1024.0);            // 电机电角度标幺到0-1
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO19 = 1; // 拉高SPI片选信号
    GpioDataRegs.GPASET.bit.GPIO12 = 1; // 拉高SAMPLE
    EDIS;
    DELAY_US(6);
}
