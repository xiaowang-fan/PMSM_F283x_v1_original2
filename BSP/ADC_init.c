/*
 * ADC_init.c
 *
 *  Created on: 2024锟斤拷4锟斤拷18锟斤拷
 *      Author: Haoyi Mu
 */

#include "../../PMSM_F283x_v1_original2/BSP/ADC_init.h"


void ConfigureADC(void)
{

//    /* ADC  A\B\C\D */
//
//    Configure_single_ADC(&AdcaRegs, ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
//    Configure_single_ADC(&AdcbRegs, ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
//    Configure_single_ADC(&AdccRegs, ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
//    Configure_single_ADC(&AdcdRegs, ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
//
//    /* delay for 1ms to allow ADC time to power up */
//
//    DELAY_US(1000);
//    Device_cal();
//
//    CalAdcINL(ADC_ADCA);
//    CalAdcINL(ADC_ADCB);
//    CalAdcINL(ADC_ADCC);
//    CalAdcINL(ADC_ADCD);
//
//    /* set SOC */
//
//    SetupADCSoftware();
    Configure_single_ADC(&AdcdRegs, ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    DELAY_US(1000);
    Device_cal();
    CalAdcINL(ADC_ADCD);

    Configure_single_ADC(&AdccRegs, ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);//配置ADC模块C，&AdccRegs：指向ADC模块C的寄存器。。
    DELAY_US(1000);//延时1毫秒，确保ADC模块C有足够时间上电和稳定。//ADC_SIGNALMODE_SINGLE：设置信号模式为单端输入。ADC_RESOLUTION_12BIT：设置分辨率为12位
    Device_cal();//调用设备校准函数，校准DSP的全局参数。
    CalAdcINL(ADC_ADCC);//调用ADC模块C的积分非线性（INL）校准函数，提高ADC的精度。


    Configure_single_ADC(&AdcaRegs, ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    DELAY_US(1000);//delay for 1ms to allow ADC time to power up
    Device_cal();
    CalAdcINL(ADC_ADCA);

    Configure_single_ADC(&AdcbRegs, ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    DELAY_US(1000);
    Device_cal();
    CalAdcINL(ADC_ADCB);


    SetupADCSoftware();



}


void Configure_single_ADC(volatile struct ADC_REGS *ADCR, Uint16 adc, Uint16 resolution, Uint16 signalmode){

    EALLOW;

    /* write configurations */

    ADCR->ADCCTL2.bit.PRESCALE    = 6;//6;    // set ADCCLK divider to /4

    AdcSetMode(adc, resolution, signalmode);

    /* set pulse positions to late */

    ADCR->ADCCTL1.bit.INTPULSEPOS = 1;

    /* power up the ADC */

    ADCR->ADCCTL1.bit.ADCPWDNZ    = 1;

    EDIS;

    DELAY_US(1000);
}


void SetupADCSoftware(void)
{
    Uint16 acqps=14;//14;

    //Select the channels to convert and end of conversion flag
    //ADCA
    EALLOW;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL     = 3;       //SOC0 will convert pin A3 ---> current D
    AdcaRegs.ADCSOC0CTL.bit.ACQPS     = acqps;
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;

    AdcaRegs.ADCSOC1CTL.bit.CHSEL     = 5;       //SOC1 will convert pin A5  --->current A
    AdcaRegs.ADCSOC1CTL.bit.ACQPS     = acqps;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;       //end of SOC1 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    EDIS;

    //ADCB
    EALLOW;

    AdcbRegs.ADCSOC0CTL.bit.CHSEL     = 3;       //SOC0 will convert pin A3 ---> current E
    AdcbRegs.ADCSOC0CTL.bit.ACQPS     = acqps;
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;

    AdcbRegs.ADCSOC1CTL.bit.CHSEL     = 5;       //SOC1 will convert pin A5  --->current B
    AdcbRegs.ADCSOC1CTL.bit.ACQPS     = acqps;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;

    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 1;       //end of SOC1 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E   = 0;       //disable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    EDIS;

    //ADCC
    EALLOW;

    AdccRegs.ADCSOC0CTL.bit.CHSEL     = 3;       //SOC0 will convert pin A3 ---> current F
    AdccRegs.ADCSOC0CTL.bit.ACQPS     = acqps;
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;

    AdccRegs.ADCSOC1CTL.bit.CHSEL     = 5;       //SOC1 will convert pin A5  --->current C
    AdccRegs.ADCSOC1CTL.bit.ACQPS     = acqps;
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;

    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 1;       //end of SOC1 will set INT1 flag
    AdccRegs.ADCINTSEL1N2.bit.INT1E   = 0;       //disable INT1 flag
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    EDIS;

}



//void SetupADCSoftware(void)
//{
//    Uint16 acqps=14;
//
//    //
//    //Select the channels to convert and end of conversion flag
//    //ADCA
//    //
//    EALLOW;
//    //ADCD
//    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 5;  //第一套使用ADCINC5
//    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //
//    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //Trigger5对应ePWM1进行触发
//
//    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 5;
//    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;
//    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
//
//    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 3;  // 第二套使用的ADCINC3
//    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps; //
//    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5; //Trigger11对应ePWM4进行触发
//
//    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 3;  //
//    AdcbRegs.ADCSOC2CTL.bit.ACQPS = acqps; //
//    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5; //
//
//
//
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 2; //end of SOC0 will set INT1 flag
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag使能，运行触发中断
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared清除中断标志
//
//    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 2; //end of SOC0 will set INT1 flag
//    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0; //end of SOC0 will set INT1 flag
//    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0; //end of SOC1 will set INT1 flag
//    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//    EDIS;
//}



