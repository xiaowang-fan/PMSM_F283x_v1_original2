/*
 * eQEP_position_speed.c
 *
 *  Created on: 2024年4月14日
 *      Author: Haoyi Mu
 */


#include "eQEP_position_speed.h"

//
// POSSPEED_Init - Initialize EQEP1 configuration
//


//void POSSPEED_Init(POSSPEED *p) {
//    int i;
//    EQep2Regs.QDECCTL.bit.QSRC = 0;
//    EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;
//    EQep2Regs.QEPCTL.bit.PCRM = 0;
//    EQep2Regs.QPOSMAX = p->N_line;
//    EQep2Regs.QEPCTL.bit.QPEN = 1;
//    EQep2Regs.QCAPCTL.bit.CEN = 0;
//
//    // 初始化历史缓冲区索引
//    p->history_idx = 0;
//    for(i = 0; i < 10; i++) p->theta_history[i] = 0;
//}
//
//// 该函数在 10kHz 中断里被调用
//void POSSPEED_Calc(POSSPEED *p) {
//    uint32_t newp;
//
//    p->DirectionQep = EQep2Regs.QEPSTS.bit.QDF;
//    newp = EQep2Regs.QPOSCNT;
//    p->theta_raw = newp + p->cal_angle;
//    while(p->theta_raw >= p->N_line) p->theta_raw -= p->N_line;
//    while(p->theta_raw < 0)          p->theta_raw += p->N_line;
//    p->theta_mech = (float)p->theta_raw * p->mech_scaler;
//
//    int32_t delta_pos_100us = (int32_t)p->theta_raw - (int32_t)p->old_theta_raw;
//    if (delta_pos_100us < -((int32_t)p->N_line / 2))      p->rev_cnt++;
//    else if (delta_pos_100us > ((int32_t)p->N_line / 2))  p->rev_cnt--;
//    p->theta_pos = p->theta_mech * 360.0f + (float)p->rev_cnt * 360.0f;
//    p->theta_elec = (float)p->pole_pairs * p->theta_mech;
//    p->theta_elec = p->theta_elec - (int)p->theta_elec;
//
//    // =========================================================================
//    // 🚀 核心：1ms (10个周期) 历史跨度差分算法（修复 ma_idx 报错）
//    // =========================================================================
//    // 1. 取出 1ms (10个周期) 之前的旧角度位置
//    int32_t old_theta_1ms = p->theta_history[p->history_idx];
//
//    // 2. 将当前最新角度存入历史环形缓冲区，供 1ms 后使用
//    p->theta_history[p->history_idx] = (int32_t)p->theta_raw;
//    p->history_idx++;
//    if(p->history_idx >= 10) p->history_idx = 0;
//
//    // 3. 计算 1ms 跨度内的脉冲总增量
//    int32_t delta_pos_1ms = (int32_t)p->theta_raw - old_theta_1ms;
//
//    // 4. 1ms 跨度的过零溢出处理
//    if (delta_pos_1ms > ((int32_t)p->N_line / 2))       delta_pos_1ms -= p->N_line;
//    else if (delta_pos_1ms < -((int32_t)p->N_line / 2)) delta_pos_1ms += p->N_line;
//
//    // 5. 使用 1ms 跨度计算转速 (1000Hz 差分频率)
//    // 60 * 1000 / 10000线 = 6.0f
//    p->SpeedRpm_fr = 6.0f * (float)delta_pos_1ms;
//
//    p->speed_send_rpm = p->SpeedRpm_fr;
//    p->old_theta_raw = p->theta_raw;
//}
//
//void  POSSPEED_Init(POSSPEED *p)
//
//{
//
//    //EQep1Regs.QUPRD = 2000000;            // Unit Timer for 100Hz at 200 MHz
//
//                                          // SYSCLKOU
//
//
//
//
//
//    EQep2Regs.QDECCTL.bit.QSRC = 00;      // QEP quadrature count mode
//
//    EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;
//
//    EQep2Regs.QEPCTL.bit.PCRM = 00;       // PCRM=00 mode - QPOSCNT reset on
//
//                                          // index event
//
//    EQep2Regs.QEPCTL.bit.UTE = 0;         // Unit Timeout disable
//
//    //EQep1Regs.QEPCTL.bit.QCLM = 1;        // Latch on unit time out
//
//    EQep2Regs.QPOSMAX = p->N_line;
//
//    EQep2Regs.QEPCTL.bit.QPEN = 1;        // QEP enable
//
//
//
//    //EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
//
//    //EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
//
//    EQep2Regs.QCAPCTL.bit.CEN = 0;        // QEP Capture disable
//
//}
//
//
//
//
//
//
//
////
//
//// POSSPEED_Calc - Perform the position calculations
//
//// only  (x1-x2) /T
//
////
//
//void POSSPEED_Calc(POSSPEED *p)
//
//{
//        int32_t newp;
//        int32_t oldp;
//        int32_t delta_cnt;
//        int32_t delta_theta_raw;
//
//        float frf_window_time;
//
//        p->DirectionQep = EQep2Regs.QEPSTS.bit.QDF;
//
//        newp = (int32_t)EQep2Regs.QPOSCNT;
//        oldp = (int32_t)p->oldpos;
//
//        p->theta_raw = newp + p->cal_angle;
//
//        while(p->theta_raw >= p->N_line)
//        {
//            p->theta_raw -= p->N_line;
//        }
//
//        while(p->theta_raw < 0)
//        {
//            p->theta_raw += p->N_line;
//        }
//
//        p->theta_mech = (float)p->theta_raw * p->mech_scaler;
//
//        delta_theta_raw = (int32_t)p->theta_raw - (int32_t)p->old_theta_raw;
//
//        if(delta_theta_raw < -((int32_t)p->N_line / 2))
//        {
//            p->rev_cnt++;
//        }
//        else if(delta_theta_raw > ((int32_t)p->N_line / 2))
//        {
//            p->rev_cnt--;
//        }
//
//        p->theta_pos = (p->theta_mech + (float)p->rev_cnt) * 360.0f;
//
//        p->theta_elec = p->pole_pairs * p->theta_mech;
//
//        p->theta_elec = p->theta_elec - (int32_t)p->theta_elec;
//
//        delta_cnt = newp - oldp;
//
//        if(delta_cnt > ((int32_t)p->N_line / 2))
//        {
//            delta_cnt -= (int32_t)p->N_line;
//        }
//        else if(delta_cnt < -((int32_t)p->N_line / 2))
//        {
//            delta_cnt += (int32_t)p->N_line;
//        }
//
//        p->SpeedRpm_raw = (float)delta_cnt * 60.0f * QEP_SAMPLE_FREQ_HZ / (float)p->N_line;
//
//        if(p->k1 < 0.0f)
//        {
//            p->k1 = 0.0f;
//        }
//        else if(p->k1 >= 1.0f)
//        {
//            p->k1 = 0.9999f;
//        }
//
//        p->k2 = 1.0f - p->k1;
//
//        p->SpeedRpm_fr += p->k2 * (p->SpeedRpm_raw - p->SpeedRpm_fr);
//
//        p->frf_delta_cnt_sum += delta_cnt;
//        p->frf_speed_sample_cnt++;
//
//        if(p->frf_speed_sample_cnt >= FRF_SPEED_WINDOW_SAMPLES)
//        {
//            frf_window_time = (float)p->frf_speed_sample_cnt / QEP_SAMPLE_FREQ_HZ;
//
//            if(frf_window_time > 0.0f)
//            {
//                p->SpeedRpm_frf = (float)p->frf_delta_cnt_sum * 60.0f / ((float)p->N_line * frf_window_time);
//            }
//            else
//            {
//                p->SpeedRpm_frf = 0.0f;
//            }
//
//            p->frf_delta_cnt_sum = 0;
//            p->frf_speed_sample_cnt = 0U;
//        }
//
//        /*
//         * 显示或通信速度可根据用途选择。
//         * 如果主要观察低速工作点，建议使用SpeedRpm_frf。
//         */
//        p->speed_send_rpm = p->SpeedRpm_frf;
//
//        /* ========================================================
//         * 保存本次位置
//         * ======================================================== */
//        p->oldpos = newp;
//        p->old_theta_raw = p->theta_raw;
//
//}
void POSSPEED_Init(POSSPEED *p)
{
    //EQep1Regs.QUPRD = 2000000;            // Unit Timer for 100Hz at 200 MHz
                                          // SYSCLKOU


    EQep2Regs.QDECCTL.bit.QSRC = 00;      // QEP quadrature count mode
    EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;
    EQep2Regs.QEPCTL.bit.PCRM = 00;       // PCRM=00 mode - QPOSCNT reset on
                                          // index event
    EQep2Regs.QEPCTL.bit.UTE = 0;         // Unit Timeout disable
    //EQep1Regs.QEPCTL.bit.QCLM = 1;        // Latch on unit time out
    EQep2Regs.QPOSMAX = p->N_line;
    EQep2Regs.QEPCTL.bit.QPEN = 1;        // QEP enable

    //EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    //EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    EQep2Regs.QCAPCTL.bit.CEN = 0;        // QEP Capture disable
}




//
// POSSPEED_Calc - Perform the position calculations
// only  (x1-x2) /T


void POSSPEED_Calc(POSSPEED *p)
{
    int Tmp1,newp,oldp;
    int32_t delta_pos;


    p->DirectionQep = EQep2Regs.QEPSTS.bit.QDF;
    newp = (unsigned int)EQep2Regs.QPOSCNT;
    oldp = p->oldpos;


    p->theta_raw = newp + p->cal_angle;
    while(p->theta_raw >= p->N_line)
    {
        p->theta_raw -= p->N_line;
    }
    while(p->theta_raw < 0)
    {
        p->theta_raw += p->N_line;
    }

    p->theta_mech = p->theta_raw * p->mech_scaler;


    delta_pos = (int32_t)p->theta_raw - (int32_t)p->old_theta_raw;

    if (delta_pos < -((int32_t)p->N_line / 2))
    {
        p->rev_cnt++;
    }
    else if (delta_pos > ((int32_t)p->N_line / 2))
    {
        p->rev_cnt--;
    }

    p->theta_pos = p->theta_mech * 360.0 + p->rev_cnt * 360.0;

    p->theta_elec = p->pole_pairs * p->theta_mech;
    p->theta_elec = p->theta_elec - (int)p->theta_elec;

    if(p->DirectionQep == 0)
    {
        if(newp > oldp)  Tmp1 = - (p->N_line - newp + oldp);
        else             Tmp1 = newp - oldp;
    }
    else if(p->DirectionQep == 1)
    {
        if(newp < oldp)  Tmp1 = p->N_line + newp - oldp;
        else             Tmp1 = newp - oldp;
    }

    p->oldpos = newp;
    p->old_theta_raw = p->theta_raw;

    p->k2 = 1.0 - p->k1;
    p->SpeedRpm_fr = (Tmp1 * 60) * p->k2 + p->SpeedRpm_fr * p->k1;
    p->speed_send_rpm = p->SpeedRpm_fr;
}


//
// End of file
//
