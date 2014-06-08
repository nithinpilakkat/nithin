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
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : flash_data.c 
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RDK+RX62N
* Description  : Defines flash and ADC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Provides standard string function definitions used in this file */
#include <string.h>
/* Defines flash API functions used in this file */
#include "Flash_API_RX600.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */

/* Defines LCD functions used in this file */

/* Declares prototypes for functions defined in this file */
#include "flash_data.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Imported global variable, list of block addresses array */
extern const uint32_t block_addresses[54];
/* Flash write buffer array */
uint8_t gFlashWriteBuffer[16];
/* Flash write address location global variable */
uint32_t gFlashWriteAddr;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* ADC initialisation function prototype declaration */
void InitADC_FlashData(void);
/* Switch callback function prototype delcaration */
void CB_Switch(void);
/* 16bit integer to character string function prototype delcaration */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* 32bit integer to character string function prototype declaration */
void uint32_ToString(uint8_t *, uint8_t, uint32_t);

/*******************************************************************************
* Outline      : Init_FlashData
* Description  : This function initialises the MCU flash area, allowing it to be
*         read and written to by user code. The function then calls the
*         InitADC_FlashData function to configure the ADC unit used in
*         the project. Finally the function erases the contents of the
*         data flash.
* Argument     : none
* Return value : none
*******************************************************************************/
 unsigned long  *fptr1;
 const char st[]= "Hello World";
void Init_FlashData(void)
{
//   unsigned char buf[4];
  /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
  
  /* Initialise the ADC unit */
  //--InitADC_FlashData();
   fptr1 =(unsigned long*) block_addresses[BLOCK_DB0]; 
  /* Erase flash memory before writing to it */
  //--Erase_FlashData();

  //--R_FlashWrite(block_addresses[BLOCK_DB0],(uint32_t)st,256);
                                   
}
/******************************************************************************
End of function Init_FlashData
******************************************************************************/

/*******************************************************************************
* Outline      : Erase_FlashData
* Description  : This function enters a for loop, and erases a block of data
*         flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
void Erase_FlashData(void)
{
  /* Declare flash API error flag */
  uint8_t ret = 0;
  
  /* Declare loop count and block count variables */
  uint8_t loop_counter = 0, block_count = BLOCK_DB0;
  
  uint32_t address = 0x00000000;
  
  /* Initialise a for loop to erase each of the data flash blocks */
  for(loop_counter = 0; loop_counter < DF_NUM_BLOCKS; loop_counter++)
    {
        /* Fetch beginning address of DF block */  
        address = block_addresses[BLOCK_DB0 + loop_counter];
        
    /* Erase data flash block */
        ret |=   R_FlashErase
        (
          block_count
        );  
    
    /* Check if data flash area is blank */
        ret |=  R_FlashDataAreaBlankCheck
        (
          address,
          BLANK_CHECK_ENTIRE_BLOCK
        );
                
    /* Increment block counter */
    block_count++;
  }
   
   /* Check Blank Checking */
     ret |= R_FlashDataAreaBlankCheck
       (
        address,
        BLANK_CHECK_ENTIRE_BLOCK
      );
   
   /* Halt in while loop when flash API errors detected */
   //--while(ret);
}  


/*******************************************************************************
* Outline      : Erase_FlashData
* Description  : This function enters a for loop, and erases a block of data
*         flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
  uint8_t loop_counter = 0, block_count = BLOCK_DB0;
  uint32_t address = 0x00000000;
int ret;
void Erase_FlashData1(uint8_t nb)
{
  /* Declare flash API error flag */
  /* Declare loop count and block count variables */
  
  /* Initialise a for loop to erase each of the data flash blocks */
 // for(loop_counter = 0; loop_counter < nblock; loop_counter++)
    {
        /* Fetch beginning address of DF block */  
        address = block_addresses[BLOCK_DB0 + nb];
        block_count = BLOCK_DB0 +nb;
    /* Erase data flash block */
        ret |=   R_FlashErase
        (
          block_count
        );  
    
    /* Check if data flash area is blank */
        
		ret |=  R_FlashDataAreaBlankCheck
        (
          address,
          BLANK_CHECK_ENTIRE_BLOCK
        );
                
    /* Increment block counter */
    block_count++;
  }
   
   /* Check Blank Checking */
     /*
	 ret |= R_FlashDataAreaBlankCheck
       (
        address,
        BLANK_CHECK_ENTIRE_BLOCK
      );
     */
   /* Halt in while loop when flash API errors detected */
   //--while(ret);
}  

/*******************************************************************************
* End of function Erase_FlashData
*******************************************************************************/

/******************************************************************************
* End of function CB_Switch
******************************************************************************/

/*******************************************************************************
* Outline      : InitADC_FlashData
* Description  : This function configures the ADC unit for single shot operation
*         and sets the switch callback function to CB_Switch.
* Argument     : none
* Return value : none
*******************************************************************************/
void InitADC_FlashData(void)
{
  /* Declare error flag */
  bool err = true;
  
 
  /* Set the initial flash write address to the start of block DB0 */
  gFlashWriteAddr = block_addresses[BLOCK_DB0];
  
  /* Configure switch callback function */
  //SetSwitchReleaseCallback(CB_Switch);  
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function InitADC_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 16 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
void uint16_ToString(uint8_t * output_string, uint8_t pos,
           uint16_t input_number)
{
  /* Declare temporary charcter storage variable, and bit_shift variable */  
  uint8_t a = 0x00, bit_shift = 12u;
  
  /* Declare 16bit mask variable */
  uint16_t mask = 0xF000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while(bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a = (input_number & mask) >> bit_shift;
    
    /* Convert the hex digit into an ASCII character, and store in output
       string */
    output_string[pos] = (a < 0x0A) ? (a + 0x30) : (a + 0x37);
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = mask >> 4u;
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint16_ToString
*******************************************************************************/

/*******************************************************************************
* Outline      : uint32_ToString
* Description  : Function converts a 32 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 32 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
void uint32_ToString(uint8_t * output_string, uint8_t pos,
           uint32_t input_number)
{
  /* Declare temporary charcter storage variable, and bit_shift variable */  
  uint8_t a = 0x00, bit_shift = 28u;
  
  /* Declare 32bit mask variable */
  uint32_t mask = 0xF0000000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while(bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a = (input_number & mask) >> bit_shift;
    
    /* Convert the hex digit into an ASCII character, and store in output
       string */
    output_string[pos] = (a < 0x0A) ? (a + 0x30) : (a + 0x37);
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = mask >> 4u;
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint32_ToString
*******************************************************************************/
