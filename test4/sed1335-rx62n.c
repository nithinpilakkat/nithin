//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
#include "machine.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
#include "sed1335.h"
//-#define SED1335_DATA_PORT		P2OUT
//-#define SED1335_DATA_DIR		P2DIR
//-#define SED1335_DATA_PIN		P2IN

//-#define SED1335_CONTROL_PORT	        P1OUT
//-#define SED1335_CONTROL_DIR		P1DIR
//-#define	SED1335_CONTROL_PIN		P1IN


#define SED1335_A0			0x40    // P1.6
#define SED1335_WR		 	0x10    // P1.4
#define SED1335_RD			0x08    // P1.3
#define SED1335_RES			0x80    // P1.7
#define SED1335_CS			0x20    // P1.5

#define RST_1                           R_IO_PORT_Write(PDL_IO_PORT_4_2,1)
#define RST_0                           R_IO_PORT_Write(PDL_IO_PORT_4_2,1)

#define SED1335_CS_1			R_IO_PORT_Write(PDL_IO_PORT_4_3,1)
#define SED1335_CS_0			R_IO_PORT_Write(PDL_IO_PORT_4_3,0)

#define A0_1			        R_IO_PORT_Write(PDL_IO_PORT_4_7,1)
#define A0_0			        R_IO_PORT_Write(PDL_IO_PORT_4_7,0)

#define RD_1                            R_IO_PORT_Write(PDL_IO_PORT_4_5,1)
#define RD_0                            R_IO_PORT_Write(PDL_IO_PORT_4_5,0)

#define WR_1                            R_IO_PORT_Write(PDL_IO_PORT_4_6,1)
#define WR_0                            R_IO_PORT_Write(PDL_IO_PORT_4_6,0)





void delay_4us()
{
    unsigned int w = 80;
    while(w--);

}

//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char dataToWrite)
{

//--GLCD_ReadStatus();

SED1335_CS_0;
A0_0;
RD_1;

//SED1335_CONTROL_PORT &= ~(SED1335_WR);
WR_0;
R_IO_PORT_Write(PDL_IO_PORT_E,dataToWrite);//SED1335_DATA_PORT = dataToWrite;

//SED1335_CONTROL_PORT |= (SED1335_WR);
WR_1;

}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char commandToWrite)
{
//GLCD_ReadStatus();
  
SED1335_CS_0;
A0_1;
RD_1;

//SED1335_CONTROL_PORT &= ~(SED1335_WR);
WR_0;

R_IO_PORT_Write(PDL_IO_PORT_E,commandToWrite);

//SED1335_CONTROL_PORT |= ( SED1335_WR);
WR_1;

}


