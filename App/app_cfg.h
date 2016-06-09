/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*
* Note(s)  (1) The following #define's are used to enable/disable uC/Modules:
*              (a) APP_CFG_PROBE_OS_PLUGIN_EN   Enables/Disables the uC/OS-II plugin in uC/Probe
*
*              (b) APP_CFG_PROBE_COM_EN         Enables/Disables the target communication with uC/Probe
*
*              (c) APP_CFG_PROBE_DEMO_INTRO     Enables/Disables the introductory demo for uC/Probe
*********************************************************************************************************
*/

#define  APP_CFG_PROBE_OS_PLUGIN_EN                DEF_ENABLED           /* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */
#define  APP_CFG_PROBE_COM_EN                      DEF_ENABLED
#define  APP_CFG_PROBE_DEMO_INTRO_EN               DEF_ENABLED


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                          2

#define  OS_PROBE_TASK_PRIO                               6
#define  OS_PROBE_TASK_ID                                 6

#define  PROBE_DEMO_INTRO_CFG_TASK_LED_PRIO               7

#define  OS_TASK_TMR_PRIO                         (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE                    128

#define  OS_PROBE_TASK_STK_SIZE                         128

#define  PROBE_DEMO_INTRO_CFG_TASK_LED_STK_SIZE         128

/*
*********************************************************************************************************
*                                               uC/CPU CONFIGURATION
*
* Note(s)  (1) CPU_CFG_LEAD_ZEROS_ASM_PRESENT Counting leading zeros is optimized in assembly (bsp_a.s)
*
*********************************************************************************************************
*/

#define  CPU_CFG_LEAD_ZEROS_ASM_PRESENT

/*
*********************************************************************************************************
*                                           uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                    DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                         DEF_ENABLED


/*
*********************************************************************************************************
*                                            BSP CONFIGURATION
*
* Note(s)  (1) BSP_CFG_ARG_CHK_EN  Enables/Disables arguments checking.
*
*********************************************************************************************************
*/

#define  BSP_CFG_ARG_CHK_EN                        DEF_ENABLED

/*
*********************************************************************************************************
*                               uC/Probe plug-in for uC/OS-II CONFIGURATION
*********************************************************************************************************
*/

#define  OS_PROBE_TASK                                    1     /* Task will be created for uC/Probe OS Plug-In             */
#define  OS_PROBE_TMR_32_BITS                             1     /* uC/Probe OS Plug-In timer is a 32-bit timer              */
#define  OS_PROBE_HOOKS_EN                                1     /* Hooks to update OS_TCB profiling members will be included*/

/*
*********************************************************************************************************
*                                       uC/OS-II DCC CONFIGURATION
*********************************************************************************************************
*/

#define  OS_CPU_ARM_DCC_EN                                0 // nhan modify 1->0

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                                 0
#define  TRACE_LEVEL_INFO                                1
#define  TRACE_LEVEL_DBG                                 2

#define  APP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF
#define  BSP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF

#define  APP_CFG_TRACE                   printf
#define  BSP_CFG_TRACE                   printf

#define  APP_TRACE_INFO(x)               ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#define  BSP_TRACE_INFO(x)               ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_INFO) ? (void)(BSP_CFG_TRACE x) : (void)0)
#define  BSP_TRACE_DBG(x)                ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_DBG)  ? (void)(BSP_CFG_TRACE x) : (void)0)

#endif
