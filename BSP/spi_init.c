/*
 * spi_init.c
 *
 *  Created on: 2026Äê1ÔÂ30ÈÕ
 *      Author: 86135
 */

#include "../../PMSM_F283x_v1_original2/BSP/spi_init.h"




void Init_spi()
{
    Init_spia_GPIO();
    Init_AD2S1210_GPIO();
    spia_configuration();


    /* init---1210 */
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO13  = 1; //Reset SI1210 sensor
    GpioDataRegs.GPASET.bit.GPIO12    = 1; //Pull up data transfer control signal
    DELAY_US(15);
    GpioDataRegs.GPASET.bit.GPIO13    = 1; //Enable SI1210 sensor
    DELAY_US(15);
    GpioDataRegs.GPACLEAR.bit.GPIO12  = 1; //Pull down data transfer control signal
    DELAY_US(15);
    GpioDataRegs.GPASET.bit.GPIO12    = 1; //Pull up data transfer control signal
    EDIS;
}

void Init_spia_GPIO()
{

    /* SPIA_MOSI */

    GPIO_SetupPinMux(16, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(16, GPIO_OUTPUT, GPIO_ASYNC | GPIO_PULLUP);

    /* SPIA_MISO */

    GPIO_SetupPinMux(17, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(17, GPIO_INPUT, GPIO_ASYNC | GPIO_PULLUP);

    /* SPIA_CLK */

    GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_ASYNC | GPIO_PULLUP);

    /* SPIA_CS */

    GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(19, GPIO_OUTPUT, GPIO_ASYNC | GPIO_PULLUP);

}

void spia_configuration()
{
    SpiaRegs.SPICCR.bit.SPISWRESET     = 0;

    SpiaRegs.SPIFFTX.bit.SPIRST        = 1;
    SpiaRegs.SPIFFTX.bit.SPIFFENA      = 1;
    SpiaRegs.SPIFFTX.bit.TXFIFO        = 1;
    SpiaRegs.SPIFFTX.bit.TXFFINTCLR    = 1;       //clear flag
    SpiaRegs.SPIFFTX.bit.TXFFIENA      = 0;
    SpiaRegs.SPIFFTX.bit.TXFFIL        = 0;

    SpiaRegs.SPIFFRX.bit.RXFFOVFCLR    = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET   = 1;
    SpiaRegs.SPIFFRX.bit.RXFFINTCLR    = 1;       //clear flag
    SpiaRegs.SPIFFRX.bit.RXFFIENA      = 0;       //disable interrupt
    SpiaRegs.SPIFFRX.bit.RXFFIL        = 10;
    SpiaRegs.SPIFFCT.all               = 0x0;     //close FIFO delay

    SpiaRegs.SPICTL.bit.OVERRUNINTENA  = 0;
    SpiaRegs.SPICTL.bit.CLK_PHASE      = 0;       //SPICLK    UP ---- send     DOWN ---- receive
    SpiaRegs.SPICTL.bit.MASTER_SLAVE   = 1;       //master
    SpiaRegs.SPICTL.bit.TALK           = 1;
    SpiaRegs.SPICTL.bit.SPIINTENA      = 0;       //no SPI interrupt
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE   = 9;       //100MHz LSPCLK ---- 10MHz

    SpiaRegs.SPICCR.bit.CLKPOLARITY    = 0;       //SPI clock polarity   UP ---- send     DOWN ---- receive
    SpiaRegs.SPICCR.bit.SPILBK         = 0;       //Disable SPI loopback test mode
    SpiaRegs.SPICCR.bit.SPICHAR        = 15;      //

    SpiaRegs.SPIPRI.bit.FREE           = 1;       //Set so breakpoints don't disturb xmission

    SpiaRegs.SPICCR.bit.SPISWRESET     = 1;       //Take SPI out of software reset state
}

void Init_AD2S1210_GPIO(void)
{
    /* AD1210SAM */

    GPIO_SetupPinMux(12, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(12, GPIO_OUTPUT, GPIO_ASYNC);

    /* AD1210RST */

    GPIO_SetupPinMux(13, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(13, GPIO_OUTPUT, GPIO_ASYNC);

    /* AD2S1210A1 */

    GPIO_SetupPinMux(14, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(14, GPIO_OUTPUT, GPIO_ASYNC);

    /* AD2S1210A0 */

    GPIO_SetupPinMux(15, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(15, GPIO_OUTPUT, GPIO_ASYNC);

    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1; // Ensure A1 address pin is pulled low (GPIO14 = A1)
    GpioDataRegs.GPACLEAR.bit.GPIO15 = 1; // Ensure A0 address pin is pulled low (GPIO15 = A0)
}


