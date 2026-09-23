/*
 * identify_L.h
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#ifndef IDENTIFY_IDENTIFY_L_H_
#define IDENTIFY_IDENTIFY_L_H_


#include "sys_globals.h"
#include "motor.h"

typedef struct{float fre;//注入正弦电流频率
               float amp;//开环d轴注入幅值
               int   N;//注入正弦电压周期数
               float ud_ref;//注入d轴正弦电压流给定
               float sum_udcos;//d轴电压高频cos分量
               float sum_udsin;//d轴电压高频sin分量
               float sum_idcos;//d轴电流压高频cos分量
               float sum_idsin;//d轴电流压高频sin分量
               float mag_ud;//d轴电压高频幅值
               float mag_id;//d轴电流高频幅值
               float ang_ud;//d轴电压高频相位
               float ang_id;//d轴电流压高频相位
               float theta;//电压电流相角差
               int   count;//计数
               int   DFTCount;//DFT计算次数
               float L;//DFT计算电感参数
               float L_mean;//DFT计算电感平均值
               float x;//辨识电感
               }INJ_L;
typedef INJ_L *INJ_L_handle;

extern INJ_L identify_L;

#define INJ_L_DEFAULTS\
        {1e4,\
         3,\
         16,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0,\
         0}

void L_MACRO(INJ_L *v,MOTOR *w);


void L_Identify(void);


#endif /* IDENTIFY_IDENTIFY_L_H_ */
