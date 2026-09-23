/*
 * identify_Phif.h
 *
 *  Created on: 2026年6月3日
 *      Author: 86135
 */

#ifndef IDENTIFY_IDENTIFY_PHIF_H_
#define IDENTIFY_IDENTIFY_PHIF_H_

#include "motor.h"
#include "sys_globals.h"

typedef struct{float y;//输出量
               float psi;//输入量
               float x;//辨识磁链
               float p;//协方差
               float k;//自适应增益
               float lamda;//遗忘因子
               }RLS_Phif;
typedef RLS_Phif *RLS_Phif_handle;

extern RLS_Phif identify_Phif;

#define RLS_Phif_DEFAULTS\
        {0,\
         0,\
         0,\
         1e4,\
         0,\
         0.99}

void Phif_Macro(RLS_Phif *v,MOTOR *w);


void Phif_Identify(void);


#endif /* IDENTIFY_IDENTIFY_PHIF_H_ */
