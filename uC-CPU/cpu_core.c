/*
*********************************************************************************************************
*                                               uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/CPU in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/CPU.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           CORE CPU MODULE
*
* Filename      : cpu_core.c
* Version       : V1.22
* Programmer(s) : SR
*                 ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    CPU_CORE_MODULE
#include  <cpu_core.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  CPU COUNT LEAD ZEROs LOOKUP TABLE
*
* Note(s) : (1) Index into bit pattern table determines the number of leading zeros in an 8-bit value :
*
*                         b07  b06  b05  b04  b03  b02  b01  b00    # Leading Zeros
*                         ---  ---  ---  ---  ---  ---  ---  ---    ---------------
*                          1    x    x    x    x    x    x    x            0
*                          0    1    x    x    x    x    x    x            1
*                          0    0    1    x    x    x    x    x            2
*                          0    0    0    1    x    x    x    x            3
*                          0    0    0    0    1    x    x    x            4
*                          0    0    0    0    0    1    x    x            5
*                          0    0    0    0    0    0    1    x            6
*                          0    0    0    0    0    0    0    1            7
*                          0    0    0    0    0    0    0    0            8
*********************************************************************************************************
*/

#ifndef   CPU_CFG_LEAD_ZEROS_ASM_PRESENT
static  const  CPU_INT08U  CPU_CntLeadZerosTbl[256] = {         /* Data vals :                                          */
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,             /*   0x00 to 0x0F                                       */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,             /*   0x10 to 0x1F                                       */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,             /*   0x20 to 0x2F                                       */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,             /*   0x30 to 0x3F                                       */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             /*   0x40 to 0x4F                                       */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             /*   0x50 to 0x5F                                       */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             /*   0x60 to 0x6F                                       */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             /*   0x70 to 0x7F                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0x80 to 0x8F                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0x90 to 0x9F                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0xA0 to 0xAF                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0xB0 to 0xBF                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0xC0 to 0xCF                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0xD0 to 0xDF                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             /*   0xE0 to 0xEF                                       */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0              /*   0xF0 to 0xFF                                       */
};
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)                            /* ------------------ CPU NAME FNCTS ------------------ */
static  void  CPU_NameInit(void);
#endif


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                             CPU_Init()
*
* Description : (1) Initialize CPU module :
*
*                   (a) Initialize CPU name
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Your Product's Application.
*
*               This function is a CPU initialization function & MAY be called by application/
*               initialization function(s).
*
* Note(s)     : (2) CPU_Init() MUST be called BEFORE product's application calls any core CPU module
*                   function(s).
*********************************************************************************************************
*/

void  CPU_Init (void)
{
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameInit();
#endif
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                            CPU_NameClr()
*
* Description : Clear CPU Name.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_NameInit(),
*               Application.
*
*               This function is a CPU module application interface (API) function & MAY be called by
*               application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
void  CPU_NameClr (void)
{
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    Mem_Clr((void     *)&CPU_Name[0],
            (CPU_SIZE_T) CPU_CFG_NAME_SIZE);
    CPU_CRITICAL_EXIT();
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                            CPU_NameGet()
*
* Description : Get CPU name.
*
* Argument(s) : pname       Pointer to an ASCII character array that will receive the return CPU name
*                               ASCII string from this function (see Note #1).
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               CPU_ERR_NONE                CPU name successfully returned.
*                               CPU_ERR_NULL_PTR            Argument 'pname' passed a NULL pointer.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a CPU module application interface (API) function & MAY be called by
*               application function(s).
*
* Note(s)     : (1) The size of the ASCII character array that will receive the return CPU name ASCII
*                   string :
*
*                   (a) MUST   be greater than or equal to the current CPU name's ASCII string size
*                           including the terminating NULL character;
*                   (b) SHOULD be greater than or equal to CPU_CFG_NAME_SIZE
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
void  CPU_NameGet (CPU_CHAR  *pname,
                   CPU_ERR   *perr)
{
    CPU_SR_ALLOC();


    if (pname == (CPU_CHAR *)0) {
       *perr = CPU_ERR_NULL_PTR;
        return;
    }

    CPU_CRITICAL_ENTER();
    Str_Copy((CPU_CHAR *) pname,
             (CPU_CHAR *)&CPU_Name[0]);
    CPU_CRITICAL_EXIT();

   *perr = CPU_ERR_NONE;
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                            CPU_NameSet()
*
* Description : Set CPU name.
*
* Argument(s) : pname       Pointer to CPU name to set.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               CPU_ERR_NONE                CPU name successfully set.
*                               CPU_ERR_NULL_PTR            Argument 'pname' passed a NULL pointer.
*                               CPU_ERR_NAME_SIZE           Invalid CPU name size (see Note #1).
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a CPU module application interface (API) function & MAY be called by
*               application function(s).
*
* Note(s)     : (1) 'pname' ASCII string size, including the terminating NULL character, MUST be less
*                    than or equal to CPU_CFG_NAME_SIZE.
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
void  CPU_NameSet (CPU_CHAR  *pname,
                   CPU_ERR   *perr)
{
    CPU_SIZE_T  len;
    CPU_SR_ALLOC();


    if (pname == (CPU_CHAR *)0) {
       *perr = CPU_ERR_NULL_PTR;
        return;
    }

    len  = Str_Len(pname);
    len += sizeof((CPU_CHAR)'\0');                              /* Adj name len to include NULL char (see Note #1).     */
    if (len <= CPU_CFG_NAME_SIZE) {                             /* If       cfg name len <= max cfg name size, ...      */
        CPU_CRITICAL_ENTER();
        Str_Copy((CPU_CHAR *)&CPU_Name[0],                      /* ... copy cfg name to CPU name.                       */
                 (CPU_CHAR *) pname);
        CPU_CRITICAL_EXIT();
       *perr = CPU_ERR_NONE;

    } else {
       *perr = CPU_ERR_NAME_SIZE;
    }
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           CPU_NameInit()
*
* Description : Initialize CPU Name.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
static  void  CPU_NameInit (void)
{
    CPU_NameClr();
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_CntLeadZeros()
*
* Description : Determine the number of contiguous, most-significant, leading zero bits in a data value.
*
* Argument(s) : val         Data value to count leading zero bits.
*
* Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val', if NO errors.
*
*               0,                                                                  otherwise.
*
* Caller(s)   : Application.
*
*               This function is an INTERNAL CPU module function but MAY be called by application function(s).
*
* Note(s)     : (1) (a) Supports the following data value sizes :
*
*                       (1)  8-bits
*                       (2) 16-bits
*                       (3) 32-bits
*
*                       See also 'cpu_def.h  CPU WORD CONFIGURATION  Note #1'.
*
*                   (b) (1) For  8-bit values :
*
*                                  b07  b06  b05  b04  b03  b02  b01  b00    # Leading Zeros
*                                  ---  ---  ---  ---  ---  ---  ---  ---    ---------------
*                                   1    x    x    x    x    x    x    x            0
*                                   0    1    x    x    x    x    x    x            1
*                                   0    0    1    x    x    x    x    x            2
*                                   0    0    0    1    x    x    x    x            3
*                                   0    0    0    0    1    x    x    x            4
*                                   0    0    0    0    0    1    x    x            5
*                                   0    0    0    0    0    0    1    x            6
*                                   0    0    0    0    0    0    0    1            7
*                                   0    0    0    0    0    0    0    0            8
*
*
*                       (2) For 16-bit values :
*
*                             b15  b14  b13  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           11
*                              0    0    0         0    1    x    x    x           12
*                              0    0    0         0    0    1    x    x           13
*                              0    0    0         0    0    0    1    x           14
*                              0    0    0         0    0    0    0    1           15
*                              0    0    0         0    0    0    0    0           16
*
*
*                       (3) For 32-bit values :
*
*                             b31  b30  b29  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           27
*                              0    0    0         0    1    x    x    x           28
*                              0    0    0         0    0    1    x    x           29
*                              0    0    0         0    0    0    1    x           30
*                              0    0    0         0    0    0    0    1           31
*                              0    0    0         0    0    0    0    0           32
*
*
*                       See also 'CPU COUNT LEAD ZEROs LOOKUP TABLE  Note #1'.
*********************************************************************************************************
*/
/*$PAGE*/
#ifndef   CPU_CFG_LEAD_ZEROS_ASM_PRESENT
CPU_DATA  CPU_CntLeadZeros (CPU_DATA  val)
{
    CPU_DATA    nbr_lead_zeros_msb;
    CPU_DATA    nbr_lead_zeros_tbl;
    CPU_DATA    nbr_lead_zeros_tot;
    CPU_INT08U  nbr_shift;
    CPU_INT08U  ix;


#if   (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_08)                       /* ---------------  8-BIT DATA VAL ---------------- */
                                                                    /* Chk bits [07:00] :            ...                */
    nbr_shift          =  0;                                        /* ... right-shift 'val'  0 bits ...                */
    nbr_lead_zeros_msb =  0;                                        /* ... & nbr msb lead zeros =  0.                   */


#elif (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_16)                       /* --------------- 16-BIT DATA VAL ---------------- */
    if (val > 0x00FFL) {                                            /* Chk bits [15:08] :            ...                */
        nbr_shift          =  8;                                    /* ... right-shift 'val'  8 bits ...                */
        nbr_lead_zeros_msb =  0;                                    /* ... & nbr msb lead zeros =  0.                   */

    } else {                                                        /* Chk bits [07:00] :            ...                */
        nbr_shift          =  0;                                    /* ... right-shift 'val'  0 bits ...                */
        nbr_lead_zeros_msb =  8;                                    /* ... & nbr msb lead zeros =  0.                   */
    }


#elif (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_32)                       /* --------------- 32-BIT DATA VAL ---------------- */
    if (val > 0x0000FFFFL) {
        if (val > 0x00FFFFFFL) {                                    /* Chk bits [31:24] :            ...                */
            nbr_shift          = 24;                                /* ... right-shift 'val' 24 bits ...                */
            nbr_lead_zeros_msb =  0;                                /* ... & nbr msb lead zeros =  0.                   */

        } else {                                                    /* Chk bits [23:16] :            ...                */
            nbr_shift          = 16;                                /* ... right-shift 'val' 16 bits ...                */
            nbr_lead_zeros_msb =  8;                                /* ... & nbr msb lead zeros =  8.                   */
        }

    } else {
        if (val > 0x000000FFL) {                                    /* Chk bits [15:08] :            ...                */
            nbr_shift          =  8;                                /* ... right-shift 'val'  8 bits ...                */
            nbr_lead_zeros_msb = 16;                                /* ... & nbr msb lead zeros = 16.                   */

        } else {                                                    /* Chk bits [07:00] :            ...                */
            nbr_shift          =  0;                                /* ... right-shift 'val'  0 bits ...                */
            nbr_lead_zeros_msb = 24;                                /* ... & nbr msb lead zeros = 24.                   */
        }
    }


#else                                                               /* See Note #1a.                                    */
    return (0);
#endif


                                                                    /* ------------- CALC NBR LEAD ZEROS -------------- */
    ix                 = (CPU_INT08U)(val >> nbr_shift) & 0xFF;     /* Calc lookup tbl ix.                              */
    nbr_lead_zeros_tbl = (CPU_DATA  ) CPU_CntLeadZerosTbl[ix];      /* Get  lookup tbl nbr lead zeros.                  */
    nbr_lead_zeros_tot = (CPU_DATA  ) nbr_lead_zeros_msb +          /* Calc        tot nbr lead zeros.                  */
                                      nbr_lead_zeros_tbl;


    return (nbr_lead_zeros_tot);
}
#endif

