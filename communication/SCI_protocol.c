/*
 * Sci_protocol.c
 *
 *  Created on: 2026Äê1ÔÂ27ÈÕ
 *      Author: 86135
 */

#include "SCI_protocol.h"


void scia_msg1(char *msg,char lenth)
{
    int i;
    i = 0;

    for(i = 0; i<lenth; i++)
    {
        scia_xmit((msg[i] )&0x00FF);
        scia_xmit((msg[i]>>8 )&0x00FF);
    }

    char tail[4] = {0x00, 0x00, 0x80, 0x7f};

    for(i = 0; i<4; i++)
    {
        scia_xmit(tail[i] );
    }
}


void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST == 0x10) {}
    SciaRegs.SCITXBUF.all =a;
}


void scib_msg1(char *msg,char lenth)
{
    int i;
    i = 0;

    for(i = 0; i<lenth; i++)
    {
        scib_xmit((msg[i] )&0x00FF);
        scib_xmit((msg[i]>>8 )&0x00FF);
    }

    char tail[4] = {0x00, 0x00, 0x80, 0x7f};

    for(i = 0; i<4; i++)
    {
        scib_xmit(tail[i] );
    }
}


void scib_xmit(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST == 0x10) {}
    ScibRegs.SCITXBUF.all =a;
}

void scic_msg1(char *msg,char lenth){
    int i;
    i = 0;

    for(i = 0; i<lenth; i++){
        scic_xmit((msg[i] )&0x00FF);
        scic_xmit((msg[i]>>8 )&0x00FF);
    }

    char tail[4] = {0x00, 0x00, 0x80, 0x7f};

    for(i = 0; i<4; i++){
        scic_xmit(tail[i] );
    }

}
void scic_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scic_xmit(msg[i]);
        i++;
    }
}
void scic_xmit(int a)
{
    while (ScicRegs.SCIFFTX.bit.TXFFST == 0x10) {}
    ScicRegs.SCITXBUF.all =a;
}
