/*
 * CAN_protocol.c
 *
 *  Created on: 2026Äê1ÔÂ29ÈÕ
 *      Author: 86135
 */

#include "CAN_protocol.h"

volatile union CAN_data CANA_Send_data_Mail1;
volatile union CAN_data CANA_Receive_data_Mail2;

volatile union CAN_data CANB_Send_data_Mail3;
volatile union CAN_data CANB_Receive_data_Mail4;


void CANA_Send_mesg(float data1, float data2)
{

    CANA_Send_data_Mail1.data.data1 = data1;
    CANA_Send_data_Mail1.data.data2 = data2;

    while(CanaRegs.CAN_IF1CMD.bit.Busy);

    /* L -- DATA-A£¬H -- DATA-B */

    CanaRegs.CAN_IF1DATA.all = CANA_Send_data_Mail1.reg.L;
    CanaRegs.CAN_IF1DATB.all = CANA_Send_data_Mail1.reg.H;

    CanaRegs.CAN_IF1CMD.all = 0x00870001;

}

void CANB_Send_mesg(float data1, float data2)
{

    CANB_Send_data_Mail3.data.data1 = data1;
    CANB_Send_data_Mail3.data.data2 = data2;

    while(CanbRegs.CAN_IF1CMD.bit.Busy);

    /* L -- DATA-A£¬H -- DATA-B */

    CanbRegs.CAN_IF1DATA.all = CANB_Send_data_Mail3.reg.L;
    CanbRegs.CAN_IF1DATB.all = CANB_Send_data_Mail3.reg.H;

    CanbRegs.CAN_IF1CMD.all = 0x00870003;

    while(CanbRegs.CAN_IF1CMD.bit.Busy);

//    CanbRegs.CAN_TXRQ_X.all = 0x0008;  // 0x0008 = mail3£¨TRS3£©

}
