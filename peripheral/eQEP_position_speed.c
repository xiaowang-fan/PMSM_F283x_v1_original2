/*
 * eQEP_position_speed.c
 *
 *  Created on: 2024Äê4ÔÂ14ÈÕ
 *      Author: Haoyi Mu
 */


#include "F28x_Project.h"
#include "eQEP_position_speed.h"

//
// POSSPEED_Init - Initialize EQEP1 configuration
//
void  POSSPEED_Init(POSSPEED *p)
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
// only  (x1-x2) /T
//
void POSSPEED_Calc(POSSPEED *p)
{
    int Tmp1,newp,oldp;

    //
    // Position calculation - mechanical and electrical motor angle
    //
    p->DirectionQep = EQep2Regs.QEPSTS.bit.QDF;  // Motor direction:
                                                 // 0=CCW/reverse, 1=CW/forward

    newp = (unsigned int)EQep2Regs.QPOSCNT; // capture position once
                                                 // per QA/QB period
    oldp = p->oldpos;


    p->theta_raw = newp + p->cal_angle;      // raw theta = current pos. +
                                                 // ang. offset from QA
    while(p->theta_raw >= p->N_line)
    {
        p->theta_raw -= p->N_line;
    }

    while(p->theta_raw < 0)
    {
        p->theta_raw += p->N_line;
    }

    //
    // The following lines calculate
    // p->theta_mech ~= QPOSCNT/mech_scaler [current cnt/(total cnt in 1 rev.)]
    // where mech_scaler = 4000 cnts/revolution
    //
    p->theta_mech = p->theta_raw * p->mech_scaler;


    //The new angle for position
    if(p->DirectionQep==1)
    {
        if(newp < oldp)
            p->rev_cnt++;
    }
    else
    {
        if(newp > oldp)
            p->rev_cnt--;
    }

    p->theta_pos = p->theta_mech * 360.0 + p->rev_cnt * 360.0;

    //
    // The following lines calculate p->elec_mecha
    //
    p->theta_elec = p->pole_pairs * p->theta_mech;
    p->theta_elec = p->theta_elec - (int)p->theta_elec;  // convert to Unit

    //
    // High Speed Calculation using QEP Position counter

    //
    // Differentiator
    //



    if(p->DirectionQep==0)                     // POSCNT is counting down
    {
        if(newp>oldp)
        {
            Tmp1 = - (p->N_line - newp + oldp);    // x2-x1 should be negative
        }
        else
        {
            Tmp1 = newp -oldp;
        }
    }
    else if(p->DirectionQep == 1)              // POSCNT is counting up
    {
        if(newp<oldp)
        {
            Tmp1 = p->N_line + newp - oldp;
        }
        else
        {
            Tmp1 = newp - oldp;                 // x2-x1 should be positive
        }
    }

    //
    // Update the electrical angle
    //
    p->oldpos = newp;

    p->k2 = 1.0 - p->k1;

    // difference Line / total Line  / T *60 --->RPM
    p->SpeedRpm_fr = (Tmp1 * 60)*p->k2 + p->SpeedRpm_fr * p->k1  ; //10k  period calculation
    p->speed_send_rpm = p->SpeedRpm_fr;
}

//
// End of file
//


