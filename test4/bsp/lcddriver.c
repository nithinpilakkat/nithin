#include "machine.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
#include "lcddriver.h"
//#include <intrinsics.h>
#define RST_1                           R_IO_PORT_Write(PDL_IO_PORT_4_3,0)
#define RST_0                           R_IO_PORT_Write(PDL_IO_PORT_4_3,1)

#define SED1335_CS_1					R_IO_PORT_Write(PDL_IO_PORT_4_4,1)
#define SED1335_CS_0					R_IO_PORT_Write(PDL_IO_PORT_4_4,0)

#define A0_1			        		R_IO_PORT_Write(PDL_IO_PORT_4_5,1)//4.7
#define A0_0			        		R_IO_PORT_Write(PDL_IO_PORT_4_5,0)

#define RD_1                            R_IO_PORT_Write(PDL_IO_PORT_4_7,1)//4.5
#define RD_0                            R_IO_PORT_Write(PDL_IO_PORT_4_7,0)

#define WR_1                            R_IO_PORT_Write(PDL_IO_PORT_4_6,1)
#define WR_0                            R_IO_PORT_Write(PDL_IO_PORT_4_6,0)

const unsigned char BitReverseTable[256] =
{
0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

void LcdDelay(unsigned int ms)
{
    
    while(ms--)
    {
       __delay_cycles(35000);
    }
}
unsigned int TempLcd_data=0;
unsigned int rcv_data=0;

void GLCD_WriteData(unsigned char dataToWrite)
{
/*
  char i;
  
rcv_data=dataToWrite;
for( i=0;i<8;i++)
{
  rcv_data=rcv_data<<1;
  TempLcd_data|=rcv_data&0x0100;
  TempLcd_data=TempLcd_data>>1;
}
*/
//dataToWrite=REVERSEBITS[dataToWrite];//TempLcd_data;
//--GLCD_ReadStatus();
A0_0;
SED1335_CS_0;


RD_1;
//SED1335_CONTROL_PORT &= ~(SED1335_WR);
WR_0;
R_IO_PORT_Write(PDL_IO_PORT_D,BitReverseTable[dataToWrite]);//SED1335_DATA_PORT = dataToWrite;
WR_1;


}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char commandToWrite)
{
//GLCD_ReadStatus();
/*
char i;
rcv_data=commandToWrite;
for(i=0;i<8;i++)
{
  rcv_data=rcv_data<<1;
  TempLcd_data|=rcv_data&0x0100;
  TempLcd_data=TempLcd_data>>1;
}
commandToWrite=TempLcd_data;
*/

A0_1;  
SED1335_CS_0;

RD_1;

WR_0;
R_IO_PORT_Write(PDL_IO_PORT_D,BitReverseTable[commandToWrite]);


WR_1;

}


void LcdInit(void)
{
    int i;
    __delay_cycles(990000);
    __delay_cycles(990000);    
    __delay_cycles(990000);
    __delay_cycles(990000);    
    __delay_cycles(990000);
    __delay_cycles(990000);    

    A0_1;
    RD_1;
    WR_1;
    SED1335_CS_0 ;
    
    RST_0;
    __delay_cycles(990000);
    __delay_cycles(990000);
    RST_1;
    __delay_cycles(990000);
    __delay_cycles(990000);
    RST_0;
    __delay_cycles(990000);
    __delay_cycles(990000);    
    __delay_cycles(990000);
    __delay_cycles(990000);    
    __delay_cycles(990000);
    __delay_cycles(990000);    
     
    GLCD_WriteCommand(SED1335_SYSTEM_SET);
    GLCD_WriteData(SED1335_SYS_P1);
    GLCD_WriteData(SED1335_SYS_P2);
    GLCD_WriteData(SED1335_FY);
    GLCD_WriteData(SED1335_CR);
    GLCD_WriteData(SED1335_TCR);
    GLCD_WriteData(SED1335_LF);
    GLCD_WriteData(SED1335_APL);
    GLCD_WriteData(SED1335_APH);
    
    
    
    GLCD_WriteCommand(SED1335_SCROLL);
    GLCD_WriteData(0);
    GLCD_WriteData(0);
    GLCD_WriteData(0xef);
    GLCD_WriteData(0xb0);
    GLCD_WriteData(0x04);
    GLCD_WriteData(0xef);
    GLCD_WriteData(0);
    GLCD_WriteData(0);
    GLCD_WriteData(0);
    GLCD_WriteData(0);
    
    
    GLCD_WriteCommand(SED1335_CSRFORM);
    GLCD_WriteData(0x04);
    GLCD_WriteData(0x07);
    
    GLCD_WriteCommand(SED1335_CGRAM_ADR);
    GLCD_WriteData(0);
    GLCD_WriteData(0);
    
    GLCD_WriteCommand(SED1335_CSRDIR_R);
    
    GLCD_WriteCommand(SED1335_HDOT_SCR);
    GLCD_WriteData(SED1335_SCRD);
    
    GLCD_WriteCommand(SED1335_OVLAY);
    GLCD_WriteData(0x01);
    
    GLCD_WriteCommand(SED1335_DISP_ON);
    GLCD_WriteData(SED1335_FLASH);
    
    //CLEAR TEXT MODE
    GLCD_WriteCommand(SED1335_CSRW);
    GLCD_WriteData(0);
    GLCD_WriteData(0);

    GLCD_WriteCommand(SED1335_MWRITE);
    for(i = 0; i < 1200; i++)
            GLCD_WriteData(' ');    
    
    
}
