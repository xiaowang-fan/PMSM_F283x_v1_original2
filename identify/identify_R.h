/*
 * identify_R.h
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#ifndef IDENTIFY_IDENTIFY_R_H_
#define IDENTIFY_IDENTIFY_R_H_

#include "motor.h"
#include "sys_globals.h"

typedef struct{float mode;//模式(0为开环，1为电流闭环）
               float inj_ud;//开环d轴注入电压
               float inj_id;//闭环d轴注入电流
               float ud_ref;//开环d轴电压给定
               float id_ref;//闭环q轴电流给定
               float mean_id1;//注入前电流平均值
               float mean_ud1;//注入前电压平均值
               float mean_id2;//注入后电流平均值
               float mean_ud2;//注入后电压平均值
               int   count;//计数
               float x;//辨识电阻
               }INJ_R;
typedef INJ_R *INJ_R_handle;

extern INJ_R identify_R;

#define INJ_R_DEFAULTS\
        {0,\
         3,\
         1,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0}

void R_MACRO(INJ_R *v,MOTOR *w);

void R_Identify(void);




#endif /* IDENTIFY_IDENTIFY_R_H_ */
