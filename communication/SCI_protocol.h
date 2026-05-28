/*
 * Sci_protocol.h
 *
 *  Created on: 2026Äê1ÔÂ27ÈÕ
 *      Author: 86135
 */

#ifndef MOTOR_COMMON_MATH_SCI_PROTOCOL_H_
#define MOTOR_COMMON_MATH_SCI_PROTOCOL_H_

#include <sys_globals.h>

#include "F28x_Project.h"


void scia_msg1(char *msg,char lenth);
void scia_xmit(int a);
void scib_msg1(char *msg,char lenth);
void scib_xmit(int a);
void scic_msg1(char *msg,char lenth);
void scic_xmit(int a);
void scic_msg(char * msg);



#endif /* MOTOR_COMMON_MATH_SCI_PROTOCOL_H_ */
