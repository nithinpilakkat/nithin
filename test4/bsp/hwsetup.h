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
* File Name    : hwsetup.h
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RDK+RX62N
* Description   : Hardware setup header file.  
*******************************************************************************/
/*******************************************************************************
* History     : 03.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef HWSETUP_H
#define HWSETUP_H



/******************************************************************************
* Global Function Prototypes
******************************************************************************/

#define  Buzzer    	         PORT2.DR.BIT.B3
#define  Buzzer_Dir          PORT2.DDR.BIT.B3

//#define  KEY              !PORTE.PORT.BIT.B5
#define  ROOF_LIGHT     	  PORTB.DR.BIT.B0
#define  ROOF_LIGHT_Dir       PORTB.DDR.BIT.B0

#define  BK_LIGHT         	  PORT2.DR.BIT.B5
#define  BK_LIGHT_Dir      	  PORT2.DDR.BIT.B5


#define  PKEY             	  PORTB.DR.BIT.B1
#define  PKEY_Dir             PORTB.DDR.BIT.B1
#define  Tx_Gsm				  PORT3.DR.BIT.B2		
#define  Rx_Gsm          	  PORT3.DR.BIT.B3 	
#define  Ring           	  PORT3.DR.BIT.B5 	

#define  Tx_Gps				  PORTC.DR.BIT.B3 	
#define  Rx_Gps				  PORTC.DR.BIT.B2  	



#define DCLK          PORTE.DR.BIT.B0  //output WITH REF TO CONTROLLER
#define DIN           PORTE.DR.BIT.B2
#define CS            PORTE.DR.BIT.B1

#define PEN_INT_dir   PORTE.DDR.BIT.B5  //input 
#define PEN_INT       PORTE.PORT.BIT.B5  //input
#define DOUT          PORTE.PORT.BIT.B4
#define AD_BUSY       PORTE.PORT.BIT.B3



/* MCU clock configuration function declaration */
void ConfigureOperatingFrequency(void);
/* MCU I/O port configuration function delcaration */
void ConfigureOutputPorts(void);
/* Interrupt configuration function delcaration */
void ConfigureInterrupts(void);
/* MCU peripheral module configuration function declaration */
void EnablePeripheralModules(void);
/* Hardware setup funtion declaration */
void HardwareSetup(void);             
void Lcd_PinConfig(void);/* Update_1:May 08, 2014,TIME->14:11:58                */

void Touch_PinConfig(void);/* Update_1:May 08, 2014,TIME->14:12:04   */
void Gsm_pinConfig(void);/* Update_1:May 08, 2014,TIME->14:21:48                */
void Gps_PinConfig(void);/* Update_1:May 08, 2014,TIME->14:36:38                */


extern void __delay_cycles(unsigned long ms);
extern void __disable_interrupt();
extern void __enable_interrupt();

/* End of multiple inclusion prevention macro */
#endif
