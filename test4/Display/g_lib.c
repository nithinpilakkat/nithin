#include "machine.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
#include "sed1335.h"
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
/*
#define SED1335_A0			0x40    // P1.6
#define SED1335_WR		 	0x10    // P1.4
#define SED1335_RD			0x08    // P1.3
#define SED1335_RES			0x80    // P1.7
#define SED1335_CS			0x20    // P1.5


#define RST_1                           P1OUT |= SED1335_RES
#define RST_0                           P1OUT &= ~SED1335_RES

#define SED1335_CS_1			P1OUT |= SED1335_CS
#define SED1335_CS_0			P1OUT &= ~SED1335_CS

#define A0_1			        P1OUT |=SED1335_A0
#define A0_0			        P1OUT &=~SED1335_A0

#define RD_1                            P1OUT |=SED1335_RD
#define RD_0                            P1OUT &=~SED1335_RD

#define WR_1                            P1OUT |=SED1335_WR
#define WR_0                            P1OUT &=~SED1335_WR

*/


#define LCD_X_RES                  320
#define LCD_Y_RES                  240
#define LCD_CACHE_SIZE             9600

#define    PIXEL_OFF   0   
#define    PIXEL_ON    1
#define    PIXEL_XOR   2

extern void delay_ms(unsigned int );
__no_init volatile unsigned char LcdCache[LCD_CACHE_SIZE];   // LCD FRAMEBUFFER
int   LcdCacheIdx;                        // CURRENT LCD POINTER
int   LoWaterMark;                        // LCD UPDATE START POINTER
int   HiWaterMark;                        // LCD UPDATE END POINTER 
unsigned char  UpdateLcd;                 // FLAG FOR UPDATING LCD

void LcdPixel ( unsigned int x, unsigned int y, unsigned char mode )
{
	unsigned int  index;
	unsigned int  offset;
	unsigned char  dat;
	if ( x > LCD_X_RES ) return;
	if ( y > LCD_Y_RES ) return;

	index = (y*(LCD_X_RES>>3)) + (x/8);		// select byte from 504
	offset  = 7-(x%8);   //y - ((y / 8) * 8);    // select bit in selected byte

	dat = LcdCache[index];

	if ( mode == PIXEL_OFF ) {
			dat &= (~(0x01 << offset));
		} else if ( mode == PIXEL_ON ) {
			dat |= (0x01 << offset);
		} else if ( mode  == PIXEL_XOR ) {
			dat ^= (0x01 << offset);
		}
	LcdCache[index] = dat;

	if ( index < LoWaterMark ) {
			//  Update low marker.
			LoWaterMark = index;
		}

	if ( index > HiWaterMark ) {
			//  Update high marker.
			HiWaterMark = index;
		}
          UpdateLcd = 1;
}

/*--------------------------------------------------------------------------------------------------

  Name         :  LcdUpdate

  Description  :  Copies the LCD cache into the device RAM.

  Argument(s)  :  None.

  Return value :  .

--------------------------------------------------------------------------------------------------*/
void LcdUpdate (  )
{
	unsigned  int i,xpos,ypos,address;
	if(UpdateLcd)
        {
        if ( LoWaterMark < 0 )
		LoWaterMark = 0;
	else if ( LoWaterMark >= LCD_CACHE_SIZE )
		LoWaterMark = LCD_CACHE_SIZE - 1;

	if ( HiWaterMark < 0 )
		HiWaterMark = 0;
                   else if ( HiWaterMark >= LCD_CACHE_SIZE )
		HiWaterMark = LCD_CACHE_SIZE - 1;

	xpos =  LoWaterMark % LCD_X_RES;
        ypos =  LoWaterMark / LCD_X_RES;
        address = SED1335_GRAPHICSTART + (40 * ypos) + (xpos/8);

        GLCD_SetCursorAddress(address);
        GLCD_WriteCommand(SED1335_MWRITE);


        //-- SET initial X,Y location in lcd ram
	for ( i = LoWaterMark; i <= HiWaterMark; i++ ) {

                        GLCD_WriteData(LcdCache[i]);
		}

	//  Reset watermark pointers.
	//LoWaterMark = LCD_CACHE_SIZE - 1;
	//HiWaterMark = 0;

        LoWaterMark = 0;
	HiWaterMark = LCD_CACHE_SIZE - 1;

	UpdateLcd = 0;
        }
}
void LcdClear ()
{
	int i;

	for ( i = 0; i < LCD_CACHE_SIZE; i++ ) {
                   LcdCache[i] =   0x00;
		}

	//  Reset watermark pointers.
	LoWaterMark = 0;
	HiWaterMark = LCD_CACHE_SIZE - 1;

	UpdateLcd = 1;
}
void LcdLine ( unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char mode )
{
	int dx, dy, stepx, stepy, fraction;

	dy = y2 - y1;
	dx = x2 - x1;

	if ( dy < 0 ) {
			dy    = -dy;
			stepy = -1;
		} else {
			stepy = 1;
		}

	if ( dx < 0 ) {
			dx    = -dx;
			stepx = -1;
		} else {
			stepx = 1;
		}

	dx <<= 1;
	dy <<= 1;

	LcdPixel( x1, y1, mode );

	if ( dx > dy ) 
        {
            fraction = dy - (dx >> 1);
            while ( x1 != x2 )
            {
                            if ( fraction >= 0 ) 
                            {
                                    y1 += stepy;
                                    fraction -= dx;
                            }
                            x1 += stepx;
                            fraction += dy;
                            LcdPixel( x1, y1, mode );
            }
        }
        else {
			fraction = dx - (dy >> 1);
			while ( y1 != y2 ) {
					if ( fraction >= 0 ) {
							x1 += stepx;
							fraction -= dy;
						}
					y1 += stepy;
					fraction += dx;
					LcdPixel( x1, y1, mode );
				}
		}

	UpdateLcd = 1;
}

void LcdRect(unsigned int x,unsigned int y, unsigned int w,unsigned int h,unsigned char mode)
{
	LcdLine(x,y,x+w,y,mode);
	LcdLine(x,y+h,x+w,y+h,mode);

	LcdLine(x,y,x,y+h,mode);
	LcdLine(x+w,y,x+w,y+h,mode);
}


void LcdBox(unsigned int x,unsigned int y, unsigned int w,unsigned int h,unsigned char mode)
{
        unsigned int dx,i;
	dx =  x+w;
	for (i = 0;i<h;i++) {
			LcdLine(x,y+i,dx,y+i,mode);
		}

}

//-------------------------------------------------------------------------------------------------
// Funkcja inicjalizacji wyœwietlacza
//-------------------------------------------------------------------------------------------------
void GLCD_Initialize(void)
{

A0_1;
RD_1;
WR_1;
SED1335_CS_0 ;

RST_0;
__delay_cycles(10000);
__delay_cycles(10000);
RST_1;
__delay_cycles(10000);
__delay_cycles(10000);
RST_0;
__delay_cycles(10000);
__delay_cycles(10000);

//LcdCache[0] = 0;


/*
 GLCD_WriteCommand(0x40);    //System set (8 byte parameter)   
 GLCD_WriteData(0x30);   // IV=1(no line comp),ws=0(single drv),M2=0(8bit char height),M0=0(int CGROM), D4=1(by default)                       
 GLCD_WriteData(0x87);   // MOD=1 two frame AC drv, HorChar Size=8   
 GLCD_WriteData(0x07);   // VerChar Size=8   
 GLCD_WriteData(0x28);   // (320/8=40) char per line   
 GLCD_WriteData(0x47);   // 23+blanking (frame freq = 66Hz (clk ratio=1/4  @ 10MHz)   
 GLCD_WriteData(0xEF);   // (240-1=239) line per screen   
 GLCD_WriteData(0x28);   // Virtual screen width LSB   
 GLCD_WriteData(0x00);   // Virtual screen width MSB   
*/

//LcdCache[0] = 0;

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


GLCD_ClearText();
GLCD_ClearGraphic();
GLCD_TextGoTo(1,7);
}
//-------------------------------------------------------------------------------------------------
// Funkcja zapalaj¹ca piksel
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Funkcja zapalaj¹ca piksel
//-------------------------------------------------------------------------------------------------

void GLCD_SetPixel(unsigned int x,unsigned int y, int color)
{
unsigned char tmp = 0;
unsigned int address = SED1335_GRAPHICSTART + (40 * y) + (x/8);




GLCD_SetCursorAddress(address);
GLCD_WriteCommand(SED1335_MREAD);
//--tmp = GLCD_ReadData();


if(!color)
  tmp |= (1 << (SED1335_FX - (x % 8)));
else
  tmp &= ~(1 << (SED1335_FX - (x % 8)));

GLCD_SetCursorAddress(address);
GLCD_WriteCommand(SED1335_MWRITE);
GLCD_WriteData(tmp);
}

//-------------------------------------------------------------------------------------------------
// Funkcja wyswietlajaca tekst
//-------------------------------------------------------------------------------------------------
void GLCD_WriteText(char * tekst)
{
GLCD_WriteCommand(SED1335_MWRITE);
while(*tekst)
	GLCD_WriteData(*tekst++);
}
//-------------------------------------------------------------------------------------------------
// Funkcja ustawiaj¹ca adres kursora
//-------------------------------------------------------------------------------------------------
void GLCD_SetCursorAddress(unsigned int address)
{
GLCD_WriteCommand(SED1335_CSRW);
GLCD_WriteData((unsigned char)(address & 0xFF));
GLCD_WriteData((unsigned char)(address >> 8));
}

//-------------------------------------------------------------------------------------------------
/*
void GLCD_GetCursorAddress(char * point)
{
    unsigned int addr,temp;
    GLCD_WriteCommand(SED1335_CSRR);
    addr = GLCD_ReadData();
    temp = GLCD_ReadData();
    addr |= temp<<8;


    point[0] = addr%40;
    point[1] = addr/40;
}
*/
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y)
{
GLCD_SetCursorAddress((y * 40) + x);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned int x, unsigned int y)
{
GLCD_SetCursorAddress(SED1335_GRAPHICSTART + (y * 40) + x/8);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(void)
{
int i;
GLCD_TextGoTo(0,0);
GLCD_WriteCommand(SED1335_MWRITE);
for(i = 0; i < 1200; i++)
	GLCD_WriteData(' ');
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(void)
{
unsigned int i;
GLCD_SetCursorAddress(SED1335_GRAPHICSTART);
GLCD_WriteCommand(SED1335_MWRITE);
for(i = 0; i < (40 * 240); i++)
      GLCD_WriteData(0x00);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(char * bmp, int x, int y, int width, int height)
{
unsigned int i, j;
for(i = 0; i < height ; i++)
	{
	GLCD_GraphicGoTo(x, y+i);
	GLCD_WriteCommand(SED1335_MWRITE);
	for(j = 0; j < width/8; j++);
		//GLCD_WriteData(GLCD_ReadByteFromROMMemory(bmp+j+(40*i)));
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
