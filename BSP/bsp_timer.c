#if 0
/**
 * \file  bsp_timer.c
 *
 * \brief create tick time for ucOS.
 */

/*
* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/ 
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "bsp_timer.h"

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int secCnt = 0;
static volatile unsigned int flagIsrCnt = 0;

/******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/
#if 0
int main(void)
{
    /* Set up the UART2 peripheral */
    UARTStdioInit();

    /* Set up the Timer2 peripheral */
    TimerSetUp64Bit();

    /* Set up the AINTC to generate Timer2 interrupts */
    TimerIntrSetUp();

    /* Enable the timer interrupt */
    TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

#ifndef _TMS320C6X
    /* Switch to non privileged mode; This is done for demonstration purpose */
    CPUSwitchToUserMode();
#endif

    /* Send the first String */
    UARTPuts("Tencounter: 9", -1);

    /* Start the timer. Characters from cntArr will be sent from the ISR */
    TimerEnable(SOC_TMR_2_REGS, TMR_TIMER12, TMR_ENABLE_CONT);

    /* make sure all the characters from cntArray from ISR */
    while(secCnt < 9)
    {
        /* Replace previous number each time the timer interrupt occurs */
        if(flagIsrCnt)
        {
            UARTPutc('\b');
            UARTPutc(cntArr[secCnt]);
            secCnt++;
            flagIsrCnt = 0;
        }
    }

    /* Disable the timer. No more timer interrupts */
    TimerDisable(SOC_TMR_2_REGS, TMR_TIMER12);

    /* Halt the program */
    while(1);
}
#endif


/*
 * @function: InitTimeTick
 * @brief   : initial tick clock for ucOS II
 * @param   : none
 * @return  : none
 * @note    : it write base on timer functions of startware, TI
 */
void InitTimeTick (void)
{

	/* Set up the Timer2 peripheral */
    TimerSetUp64Bit();

    /* Set up the AINTC to generate Timer2 interrupts */
    TimerIntrSetUp();

    /* Enable the timer interrupt */
    TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
	
	/* Start the timer. Characters from cntArr will be sent from the ISR */
    TimerEnable(SOC_TMR_2_REGS, TMR_TIMER12, TMR_ENABLE_CONT);
}


/*
** Timer Interrupt Service Routine
*/
static void TimerIsr(void)
{
    /* Disable the timer interrupt */
    TimerIntDisable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

#ifdef _TMS320C6X
    /* Clear interrupt status in DSPINTC */
    IntEventClear(SYS_INT_T64P2_TINTALL);
#else
    /* Clear the interrupt status in AINTC */
    IntSystemStatusClear(SYS_INT_TIMR2_ALL);
#endif
    TimerIntStatusClear(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

#if TEST_TIME >0
    if (flagIsrCnt == 1)
    {
    	flagIsrCnt = 0;
    	GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_LOW);
    }
    else
    {
    	flagIsrCnt = 1;
    	GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_HIGH);
    }
#endif
    /* Enable the timer interrupt */
    TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}


/*
** Setup the timer for 64 bit mode
*/
static void TimerSetUp64Bit(void)
{
    /* Configuration of Timer */
    TimerConfigure(SOC_TMR_2_REGS, TMR_CFG_64BIT_CLK_INT);

    /* Set the 64 bit timer period */
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER12, TMR_PERIOD_LSB32);
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER34, TMR_PERIOD_MSB32);
}

/*
** Set up the ARM Interrupt Controller for generating timer interrupt
*/
static void TimerIntrSetUp(void)
{
#ifdef _TMS320C6X
    /* Initialize the DSPINTC */
    IntDSPINTCInit();

    /* Register the Timer ISR */
    IntRegister(C674X_MASK_INT4, TimerIsr);

    /* Map Timer interrupts to DSP maskable interrupt */
    IntEventMap(C674X_MASK_INT4, SYS_INT_T64P2_TINTALL);

    /* Enable DSP interrupt in DSPINTC */
    IntEnable(C674X_MASK_INT4);

    /* Enable DSP interrupts */
    IntGlobalEnable();
#else
    /* Initialize AINTC and register timer interrupt */
    IntAINTCInit();
    
    /* Register the Timer ISR */
    IntRegister(SYS_INT_TIMR2_ALL, TimerIsr);
  
    /* Set the channel number for Timer interrupt, it will map to IRQ */
    IntChannelSet(SYS_INT_TIMR2_ALL, 2);
   
    /* Enable IRQ for ARM (in CPSR)*/
    IntMasterIRQEnable();
  
    /* Enable AINTC interrupts in GER */
    IntGlobalEnable();

    /* Enable IRQ in AINTC */
    IntIRQEnable();

    /* Enable timer interrupts in AINTC */
    IntSystemEnable(SYS_INT_TIMR2_ALL);
#endif
}
/***************************** End Of File ***********************************/
#endif
