/*
 * SPI_protocol.h
 *
 *  Created on: 2026年1月30日
 *      Author: 86135
 */

#ifndef MOTOR_COMMON_MATH_SPI_PROTOCOL_H_
#define MOTOR_COMMON_MATH_SPI_PROTOCOL_H_

#include "F28x_Project.h"
#include "sys_globals.h"

extern int16 motor_position;      // 电机位置
extern Uint16 angle_offset; //1400是张哥老电机旋变对0位置偏置,2041是55kW老电机零位，2068是杨老师的新电机，3375是黄师兄的电机,0是12kW出厂电机
extern float electrical_position; // 电角度
extern float Angle_pu;            // 电机电角度标幺

void Motor_Position_1210();


#endif /* MOTOR_COMMON_MATH_SPI_PROTOCOL_H_ */
