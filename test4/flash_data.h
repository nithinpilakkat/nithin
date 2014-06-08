/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name    : flash_data.h
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RDK+RX62N
* Description   : Provides declarations for functions defined in flash_data.c  
*******************************************************************************/
/*******************************************************************************
* History     : 27.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef FLASH_DATA_H
#define FLASH_DATA_H
/* Define PROG_SIZE (see Flash API Application note for further details) */
#define PROG_SIZE       256
/* Define BUFF_SIZE (see Flash API Application note for further details) */
#define BUFF_SIZE       256

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Initialise flash sample function prototype declaration */
void Init_FlashData(void);
/* Flash write function prototype declaration */
void Write_FlashData(void);
/* Flash erase function prototype declaration */
void Erase_FlashData(void);


/* End of multiple inclusion prevention macro */
#endif
