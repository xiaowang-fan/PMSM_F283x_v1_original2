/*
 * sci_init.c
 *
 *  Created on: 2026Äê1ÔÂ24ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/sci_init.h"

void Init_sci()
{
    /* init sci_gpio */

//    Init_scia_GPIO();
//    Init_scia_fifo();
//    scia_configuration();
//
//    Init_scib_GPIO();
//    Init_scib_fifo();
//    scib_configuration();

    Init_scic_GPIO();
    Init_scic_fifo();
    scic_configuration();
}


void Init_scia_GPIO()
{

    /* SCIA_RX */

    GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);

    /* SCIA_TX */

    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_ASYNC);

}


void Init_scia_fifo()
{
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;
}

void scia_configuration()
{

    /* 1 stop bit,  No loopback,  No parity,8 char bits,  async mode, idle-line protocol */

    SciaRegs.SCICCR.all             = 0x0007;

    /* enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE */

    SciaRegs.SCICTL1.all            = 0x0003;
    SciaRegs.SCICTL2.all            = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA   = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    /* baud */

    SciaRegs.SCIHBAUD.all           = 0;//0x0002;
    SciaRegs.SCILBAUD.all           = 1;//0x008B;

    SciaRegs.SCICTL1.all            = 0x0023;  // Relinquish SCI from Reset

}


void Init_scib_GPIO()
{

    /* SCIB_RX */

    GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 2);
    GPIO_SetupPinOptions(19, GPIO_INPUT, GPIO_PUSHPULL);

    /* SCIB_TX */

    GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 2);
    GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC);

}


void Init_scib_fifo()
{
    ScibRegs.SCIFFTX.all = 0xE040;
    ScibRegs.SCIFFRX.all = 0x2044;
    ScibRegs.SCIFFCT.all = 0x0;
}

void scib_configuration()
{

    /* 1 stop bit,  No loopback,  No parity,8 char bits,  async mode, idle-line protocol */

    ScibRegs.SCICCR.all             = 0x0007;

    /* enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE */

    ScibRegs.SCICTL1.all            = 0x0003;
    ScibRegs.SCICTL2.all            = 0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA   = 1;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;

    /* baud */

    ScibRegs.SCIHBAUD.all           = 0;//0x0002;
    ScibRegs.SCILBAUD.all           = 1;//0x008B;

    ScibRegs.SCICTL1.all            = 0x0023;  // Relinquish SCI from Reset

}


void Init_scic_GPIO()
{

    GPIO_SetupPinMux(139, GPIO_MUX_CPU1, 6); //R
    GPIO_SetupPinOptions(139, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 6); //T
    GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);
//    GPIO_SetupPinMux(57, GPIO_MUX_CPU1, 6);
//    GPIO_SetupPinOptions(57, GPIO_INPUT, GPIO_PUSHPULL);
//    GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 6);
//    GPIO_SetupPinOptions(56, GPIO_OUTPUT, GPIO_ASYNC);

}




void Init_scic_fifo()
{
    ScicRegs.SCIFFTX.all = 0xE040;
    ScicRegs.SCIFFRX.all = 0x2044;
    ScicRegs.SCIFFCT.all = 0x0;
}

void scic_configuration()
{
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //

    ScicRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
    ScicRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                    // Disable RX ERR, SLEEP, TXWAKE
    ScicRegs.SCICTL2.all = 0x0003;
    ScicRegs.SCICTL2.bit.TXINTENA = 1;
    ScicRegs.SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //115200
    ScicRegs.SCIHBAUD.all = 0;//0x0002;
    ScicRegs.SCILBAUD.all = 1;//0x008B;

    ScicRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset
}
