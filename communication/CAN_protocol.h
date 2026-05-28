/*
 * CAN_protocol.h
 *
 *  Created on: 2026Äê1ÔÂ29ÈÕ
 *      Author: 86135
 */

#ifndef MOTOR_COMMON_MATH_CAN_PROTOCOL_H_
#define MOTOR_COMMON_MATH_CAN_PROTOCOL_H_

#include <sys_globals.h>

#include "F28x_Project.h"

union CAN_data
{
    struct
    {
        float data1;
        float data2;
    } data;
    struct
    {
        uint32_t L;    //CAN_IF1DATA
        uint32_t H;    //CAN_IF1DATB
    } reg;
};

extern volatile union CAN_data CANA_Send_data_Mail1;
extern volatile union CAN_data CANA_Receive_data_Mail2;

extern volatile union CAN_data CANB_Send_data_Mail3;
extern volatile union CAN_data CANB_Receive_data_Mail4;


void CANA_Send_mesg(float data1, float data2);
void CANB_Send_mesg(float data1, float data2);
void CAN_Send(uint32_t mailbox, uint32_t dataA, uint32_t dataB);



#endif /* MOTOR_COMMON_MATH_CAN_PROTOCOL_H_ */
