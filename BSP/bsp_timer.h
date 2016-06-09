/**
 * \file  bsp_timer.h
 *
 * \brief header file of bsp_timer.
 *
 * \Copyright (C) 2012 NHAN
 */
 
#include "uart.h"
#include "hw_uart.h"
#include "interrupt.h"
#include "soc_OMAPL138.h"
#include "hw_syscfg0_OMAPL138.h"
#include "timer.h"
#include "lcdkOMAPL138.h"
#include "uartStdio.h"

#ifndef _TMS320C6X
#include "cpu.h"
#endif

//gpio
#include "gpio.h"
#include "psc.h"

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define STR_LEN                        (13)
#define TMR_PERIOD_LSB32               (0x03FFFFFF)
#define TMR_PERIOD_MSB32               (0x0)

#define TEST_TIME                      1

#if 0
/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void TimerIsr(void);
static void TimerSetUp64Bit(void);
static void TimerIntrSetUp(void);


/******************************************************************************
**                             FUNCTION
*******************************************************************************/
void InitTimeTick (void);

#endif
