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
* File Name    : hwsetup.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : YRDKRX62N
* Description   : Defines the initialisation routines used each time the MCU
*          is restarted. 
******************************************************************************/
/******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
******************************************************************************/
/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
#include "machine.h"
#include "r_pdl_intc.h"
#include "r_pdl_cmt.h"
#include "r_pdl_sci.h"

/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"

/* Switch handler function definitions */
//#include "switch.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"
//extern void CaptureSpeed(void);
//extern void RemoteIsr(void);


/******************************************************************************
* Outline     : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
******************************************************************************/
void HardwareSetup(void)
{
  ConfigureOperatingFrequency();
  ConfigureOutputPorts();
 //ConfigureInterrupts();
  //EnablePeripheralModules();
}

#pragma inline_asm __disable_interrupt
void __disable_interrupt(){
          CLRPSW I; Assembly-language description
}


#pragma inline_asm __enable_interrupt
void __enable_interrupt(){
          SETPSW I; Assembly-language description
}

/******************************************************************************
* End of function HardwareSetup
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument      : none
* Return value  : none
******************************************************************************/
void ConfigureOperatingFrequency(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Modify the MCU clocks */

  err=R_CGC_Set
      (
        12E6,
        96E6,
        48E6,
        24E6,
        PDL_NO_DATA
      );
     
	  
	  

  /*
    R_CGC_Set
      (
        12E6,
        12E6,
        6E6,
        3E6,
        PDL_NO_DATA
      );*/

    /* 
  Clock Description              Frequency
  ----------------------------------------
  Input Clock Frequency..............12MHz
  Internal Clock Frequency...........96MHz  
  Peripheral Clock Frequency.........48MHz
  External Bus Clock Frequency.......24MHz */
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}

/******************************************************************************
* End of function ConfigureOperatingFrequency
******************************************************************************/

/******************************************************************************
* Outline     : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings, and sets the
*          pin outputs to a safe level.
* Argument    : none
* Return value  : none
******************************************************************************/

void ConfigureOutputPorts(void)
{
   unsigned char err;
   
   Lcd_PinConfig();
   Touch_PinConfig();
   Gsm_pinConfig();
   Gps_PinConfig();




  R_IO_PORT_Set
      (
         PDL_IO_PORT_A_2|PDL_IO_PORT_A_3 |PDL_IO_PORT_A_4 ,  PDL_IO_PORT_OUTPUT 
      );

  
  R_IO_PORT_Set
      (
        PDL_IO_PORT_A_1 | PDL_IO_PORT_A_5 |PDL_IO_PORT_A_6 ,  PDL_IO_PORT_INPUT 
      );



	 
	 PORTA.DDR.BYTE = 0xFF;   
	 PORTB.DDR.BYTE = 0x1C;
	 
   
     
   R_IO_PORT_Set(PDL_IO_PORT_0_5, PDL_IO_PORT_INPUT);
 


   R_IO_PORT_Write(PDL_IO_PORT_4,0xff);
   
   R_IO_PORT_Write(PDL_IO_PORT_D ,0xff);
   

	  //buzzer output config.
	  Buzzer_Dir = 1;
	  ROOF_LIGHT_Dir = 1;
	  
   
 
     R_INTC_ControlExtInterrupt
        (
          PDL_INTC_IRQ13,
          PDL_INTC_ENABLE|PDL_INTC_FALLING
        ); 
		
	R_INTC_ControlExtInterrupt(			//  September 07, 2013  10:33:52  Nithin.p
			PDL_INTC_IRQ5,				//	penreq interrupt
			PDL_INTC_ENABLE | PDL_INTC_CLEAR_IR_FLAG
	);
			


  //R_IO_PORT_Set(PDL_IO_PORT_0_7, PDL_IO_PORT_INPUT|PDL_IO_PORT_PULL_UP_ON);
   

 err = R_INTC_ControlExtInterrupt
    (
      PDL_INTC_IRQ15,
      PDL_INTC_ENABLE|PDL_INTC_FALLING
    );  
	
 
    
	
	  //buzzer output config.
	  Buzzer = 0;
	  ROOF_LIGHT = 1;
	  
   
     PORTC.DDR.BIT.B4 = 1;  
     PORTC.DDR.BIT.B5 = 1;
     PORTC.DDR.BIT.B6 = 1;  
     PORTC.DDR.BIT.B7 = 1;
	 PORTB.DDR.BIT.B1 = 1; // PKEY
	 

   
  /* Halt in while loop when RPDL errors detected */  
  //while(!err);
}


/******************************************************************************
* End of function ConfigureOutputPorts
**
****************************************************************************/

/******************************************************************************
* Outline     : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument    : none
* Return value  : none
******************************************************************************/
void EnablePeripheralModules(void)
{

}

void Lcd_PinConfig()
{
	
/* PortD -  LCD DATA PIN */
  R_IO_PORT_Set
      (
        PDL_IO_PORT_D_0 | PDL_IO_PORT_D_1 | PDL_IO_PORT_D_2 | PDL_IO_PORT_D_3 | 
        PDL_IO_PORT_D_4 | PDL_IO_PORT_D_5 | PDL_IO_PORT_D_6 | PDL_IO_PORT_D_7 ,
        PDL_IO_PORT_OUTPUT 
      );

  /* Port4 - LCD CONTROL PIN */

 R_IO_PORT_Set
      (
        PDL_IO_PORT_4_0 | PDL_IO_PORT_4_1 | PDL_IO_PORT_4_2 | PDL_IO_PORT_4_3 | 
        PDL_IO_PORT_4_4 | PDL_IO_PORT_4_5 | PDL_IO_PORT_4_6 |PDL_IO_PORT_4_7 ,
        PDL_IO_PORT_OUTPUT
      );
   
 
}

void Touch_PinConfig()
{
  //------------ TOUCH SCREEN-----//
    
	R_IO_PORT_Set
      (
        PDL_IO_PORT_E_0 | PDL_IO_PORT_E_1 | PDL_IO_PORT_E_2,  PDL_IO_PORT_OUTPUT 
      );
	  CS	=	1;
	  DCLK	=	1;
	  DIN	=	0;
	  
   R_IO_PORT_Set
      (
        PDL_IO_PORT_E_4 | PDL_IO_PORT_E_3|| PDL_IO_PORT_E_5 ,  PDL_IO_PORT_INPUT
      );
/*  TOUCH SCREEN CONTROLLER  Update_1:May 07, 2014,TIME->11:19:29                */
	
}

void Gsm_pinConfig()
{
	R_IO_PORT_Set(PDL_IO_PORT_3_2,PDL_IO_PORT_OUTPUT);//txd6
	   
	R_SCI_Set(PDL_SCI_PIN_SCI6_B);  

}


void Gps_PinConfig(void)
{
	   R_IO_PORT_Set
      (
        PDL_IO_PORT_C_2 ,  PDL_IO_PORT_INPUT
      );

}