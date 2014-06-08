//#include "intrinsics.h"
#include "bsp/lcddriver.h"
#include "xWidget.h"


extern unsigned char const  font_19x31[];
extern unsigned char const  font_22x36[];
extern const unsigned char km[];

int font_char_index[65];
int font_char_index1[16];
//--int font_char_index2[96];
extern const unsigned char width_table[];
extern const unsigned char data_table[];
extern const unsigned int offset_table[];
extern void delay_ms(unsigned int );
//__no_init
unsigned char LcdCache[LCD_CACHE_SIZE];   // LCD FRAMEBUFFER
int   LcdCacheIdx;                        // CURRENT LCD POINTER
int   LoWaterMark;                        // LCD UPDATE START POINTER
int   HiWaterMark;                        // LCD UPDATE END POINTER 
unsigned char  UpdateLcd;                 // FLAG FOR UPDATING LCD

unsigned int  index;
unsigned int  offset;
unsigned char  dat;

void LcdPixel ( unsigned int x, unsigned int y, unsigned char mode )
{
	x= 319-x;
    y= 239-y;
	 
	if ( x > LCD_X_RES ) return;
	if ( y > LCD_Y_RES ) return;
       
	index = (y*(LCD_X_RES>>3)) + (x>>3);		// select byte from 504
	offset  = 7-(x%8);   //y - ((y / 8) * 8);    // select bit in selected byte

	dat = LcdCache[index];

	if ( mode == PIXEL_OFF ) 
	{
		dat &= (~(0x01 << offset));
	} 
	else if ( mode == PIXEL_ON ) 
	{
		dat |= (0x01 << offset);
	} 
	else if ( mode  == PIXEL_XOR ) 
	{
		dat ^= (0x01 << offset);
	}
	LcdCache[index] = dat;
  
	if ( index < LoWaterMark ) 
	{
		//  Update low marker.
		LoWaterMark = index;
	}

	if ( index > HiWaterMark ) 
	{
		//  Update high marker.
		HiWaterMark = index;
	}
    UpdateLcd = 1;
}



void LcdClear ()
{
	int i;

	for ( i = 0; i < LCD_CACHE_SIZE; i++ ) {
                   //LcdCache[i] =   0xff; //  August 22, 2013  16:12:04  Nithin.p
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

	if ( dx > dy ) {
			fraction = dy - (dx >> 1);
			while ( x1 != x2 ) {
					if ( fraction >= 0 ) {
							y1 += stepy;
							fraction -= dx;
						}
					x1 += stepx;
					fraction += dy;
					LcdPixel( x1, y1, mode );
				}
		} else {
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
	static unsigned int dx,i,h1; // static added, September 02, 2013  16:22:44  Nithin.p 
	dx =  x+w;
	h1=h;
	for (i = 0;i<h1;i++) 
	{
		LcdLine(x,y+i,dx,y+i,mode);
	}
}

void LcdPutChar1(unsigned char const * bmp,unsigned int x,unsigned int y,unsigned char w,unsigned char h)
{
	unsigned int i;
    int j,k,p;
	unsigned char scan;
	unsigned int n=0,nbyte,row;
    nbyte  = w;
    row = h;
    p = row<<1;
	for(i=0;i<p;i++)      // y_byte++
	{
    	for(j=0;j<nbyte;j++)   // x_byte++
		{
        	scan = 0x80;
			for(k=0;k<8;k++)  //  x_bit++
			{
	        	if(bmp[n]&scan)
				{
                	LcdPixel((x+(j*8)+k),y+(i>>1),2);
				}
		   	    scan >>=1;
            }
            n++;   
        }
		i++;			 // y++	
	}
}

void LcdPutChar_incl_offset(unsigned char const * bmp,unsigned int x,unsigned int y,unsigned char w,unsigned char h)
{
	unsigned int i;
        int j,k;
        int p;
	unsigned char scan;
	unsigned int n=0,nbyte,row;
        nbyte  = w;
        row = h;
        p = row<<1;
	for(i=0;i<p;i++)      // y_byte++
	{
                for(j=0;j<nbyte;j++)   // x_byte++
		{
                              scan = 0x80;
			for(k=0;k<8;k++)  //  x_bit++
			{
	                         if(bmp[n]&scan)
				 {
                                     LcdPixel((x+(j*8)+k),y+(i>>1),2);
				 }
		   	           scan >>=1;
                              
		        }
                             n++;   
                    }
			
		
	       i++;			 // y++	
	}
}

void font_index() // for fare
{
  int i;
  font_char_index[0]=0;
  for(i=0;i<27;)
  {
    i++;
    font_char_index[i]=font_char_index[i-1]+font_22x36[font_char_index[i-1]+1] * font_22x36[font_char_index[i-1]+2]+3;
    
  }
}
              

void font_index1()  // for km
{
  int i;
  font_char_index1[0]=0;
  for(i=0;i<15;)
  {
    i++;
   // font_char_index1[i]=font_char_index1[i-1]+km[font_char_index1[i-1]+1] * km[font_char_index1[i-1]+2]+3;
  }
}

void font_index2()  // for normal  font_10x18
{
  int i;
//  font_char_index2[0]=0;
  for(i=0;i<95;)
  {
    i++;
//    font_char_index2[i] = font_char_index2[i-1] + font_10x18[font_char_index2[i-1]+1]  *  font_10x18[font_char_index2[i-1]+2] + 3;
  }
}
void LcdUpdate ();

/*--
void LcdPutStr1(char *str,unsigned int x,unsigned y)
{
   int font_offset=0;
   char byte=0;
   char hight=0,width=0;
    while(*str)
    {
      font_offset=font_char_index2[*str-32]; 
      byte=font_10x18[font_offset+2];
      hight=font_10x18[font_offset+1];
      width=font_10x18[font_offset+0]+0;
      LcdPutChar_incl_offset(&font_10x18[font_char_index2[*str-32]+3],x,y,byte,hight);
    
      x+=width-4;                                         // x+=width+1(depend upon what u space need between two char)
      *str++;
    
     
    }
  
}
*/
/*

void LcdPutMsg(char *str,unsigned int x,unsigned y)
{
   int font_offset=0;
   int byte=0;
   int hight=0,width=0,sx;
   sx = x;
    while(*str)
    {
      if((*str) == 0x0a || (*str) == 0x0d)
      {
       y= y+12;
       x = sx;  
      }      
      else
	  {    
		  font_offset=font_char_index2[*str-32]; 
	      byte=font_10x18[font_offset+2];
	      hight=font_10x18[font_offset+1];
	      width=font_10x18[font_offset+0]+0;
	      LcdPutChar_incl_offset(&font_10x18[font_char_index2[*str-32]+3],x,y,byte,hight);
    
	      //x+=width;                                         // x+=width+1(depend upon what u space need between two char)
	      x+=width; 
	  }
      *str++;
    }
  
}
*/

 
void LcdPutMsg2(char *str,unsigned int x,unsigned y)
{
   int font_offset=0;
   int byte=0;
   int hight=0,width=0,sx;
    
   sx = x;
  
    while(*str)
    {
      if((*str) == 0x0a || (*str) == 0x0d)
      {
       y= y+17;
       x = sx;  
	   
      }      
      else
	  {    
		  //font_offset=font_char_index2[*str-32]; 
	      //byte=font_10x18[font_offset+2];
	      //hight=font_10x18[font_offset+1];
	      //width=font_10x18[font_offset+0]+0;
	
		  byte = width_table[*str-32]/8;
		  if(width_table[*str-32]%8 !=0)byte++;
	      LcdPutChar1(&data_table[offset_table[*str-32]],x,y,byte,23);
    
	      //x+=width;                                         // x+=width+1(depend upon what u space need between two char)
	      x+=width_table[*str-32]; 
		  if(*str == ':') x -=1;
	  }
      *str++;
    }
 }

void LcdPutMsg3(char *str,unsigned int x,unsigned y)
{
   int font_offset=0;
   int byte=0;
   int hight=0,width=0,sx;
   unsigned char nchar=0; 
   sx = x;
   
    while(*str)
    {
	  	
      if((*str) == 0x0a || (*str) == 0x0d ||  (x+width_table[*str-32]) >= 245)
      {
       y= y+17;
       x = sx;  
      }      
	  if((*str) != 0x0a && (*str) != 0x0d)
	  {
		  byte = width_table[*str-32]/8;
		  if(width_table[*str-32]%8 !=0)byte++;
	      LcdPutChar1(&data_table[offset_table[*str-32]],x,y,byte,23);
    
	      //x+=width;                                         // x+=width+1(depend upon what u space need between two char)
	      x+=width_table[*str-32]; 
		  if(*str == ':') x -=1;
	   }
      *str++;
	  nchar++;
	  if(y+17 > 119) break;
    }
 }



void print_clock(int x,int y)
{
	
// LcdPutChar1(clock,x,y,3,18);
	
}

void show_gps(int x,int y)
{
// LcdPutChar1(gps_symbole,x,y,2,12);
	
}
void show_power(int x,int y)
{
// LcdPutChar1(power_symbole,x,y,2,12);
//	LcdPutChar1(gps_symbole,x,y,2,12);
}

void show_gprs(int x,int y)
{
 //LcdPutChar1(gprs_symbole,x,y,2,12);
}
extern const unsigned char lable_forhire[];
void show_forhire(int x,int y)
{
	 LcdPutChar1(lable_forhire,x,y,0x1c,0x29);
}

void LcdPutStr1(char *str,unsigned int x,unsigned y)
{
  LcdPutMsg2(str, x,y);	
}
void LcdPutStr6(char *str,unsigned int x,unsigned y)
{
  
  int font_offset=0;
  char byte=0;
  char hight=0,width=0;
    while(*str)
    {
      font_offset=font_char_index[*str-32]; 
      byte=font_22x36[font_offset+2];
      hight=font_22x36[font_offset+1];
      width=font_22x36[font_offset+0]-7;
      LcdPutChar_incl_offset(&font_22x36[font_char_index[*str-32]+3],x,y,byte,hight);
    
      x+=width;                                         // x+=width+1(depend upon what u space need between two char)
      *str++;
    
     
    }
  
}
/*
void LcdPutStr_km(char *str,unsigned int x,unsigned y)
{
  
  int font_offset=0;
  int byte=0;
  int hight=0,width=0;
    while(*str)
    {
      font_offset	=font_char_index1[*str-48]; 
      byte			=km[font_offset+2];
      hight			=km[font_offset+1];
      width			=km[font_offset+0]+1;
      LcdPutChar_incl_offset(&km[font_char_index1[*str-48]+3],x,y,byte,hight);
    
      x+=width+2;                                         // x+=width+1(depend upon what u space need between two char)
      *str++;
    
     
    }
  
}
*/

/*--
void LcdPutStr5(char *str,unsigned int x,unsigned y)
{
    while(*str)
    {
        LcdPutChar1(&font_19x31[(*str-48)*93],x,y,3,31);//([(*str(hex of char i.e 1)-offset)*(no. of bytes for single char)],x,y,no. of byte,hight)
        x+=20;                                         // x+=width+1(depend upon what u space need between two char)
        *str++;
    }
}

*/


void LcdPutChar2(unsigned char const * bmp,unsigned int x,unsigned int y,unsigned char w,unsigned char h)
{
	unsigned int i;
        int k;
	unsigned char scan;
	unsigned int col;

        col = h;
	for(i=0;i<col;i++)      // y_byte++
	{
          scan = 0x01;
          for(k=0;k<8;k++)  //  x_bit++
          {
                   if(bmp[i]&scan)
                   {
                       LcdPixel(x+i,(y+k),2);
                   }
                   
                   scan <<=1;
                
           }
	}
}

void LcdPutStr2(char *str,unsigned int x,unsigned y)
{
    while(*str)
    {
       LcdPutChar2(f8x8a[*str],x,y,1,8);
	 
       x+=8;
       *str++;
    }
}


void LcdPutStr4(char *str,unsigned int x,unsigned y)
{
/*
   int sx =x; 
   while(*str)
    {
      if((*str) == 0x0a || (*str) == 0x0d)
      {
       y= y+10;
       x = sx;  
      }      
      else
      {
        LcdPutChar2(f8x8a[*str],x,y,1,8);
        x+=8;
      }
       *str++;
    }
	*/
}

void LcdPutStr41(char *str,char size,unsigned int x,unsigned y)
{
//   int sx =x; 
   /*
   while(size && *str)
    {
      if((*str) == 0x0a || (*str) == 0x0d)
      {
       y= y+10;
       x = sx;  
      }      
      else
      {
        LcdPutChar2(f8x8a[*str],x,y,1,8);
        x+=8;
      }
       *str++;
	   size--;
    }
	*/
}

void LcdPutChar3(unsigned char const * bmp,unsigned int x,unsigned int y,unsigned char w,unsigned char h,char mode)
{
	unsigned int i;
        int k;
	unsigned char scan;
	unsigned int col;
        
        col = h;
	for(i=0;i<col;i++)      // y_byte++
	{
          scan = 0x01;
          for(k=0;k<8;k++)  //  x_bit++
          {
                   if(bmp[i]&scan)
                   {
                       LcdPixel(x+i,(y+k),mode);
                   }
                   else
                     LcdPixel(x+i,(y+k),0);
                     
                     scan <<=1;
                
          }
	}
}
void LcdPutStr3(char *str,unsigned int x,unsigned y,char mode)
{
    
	while(*str)
    {
       LcdPutChar3(f8x8a[*str],x,y,1,8,mode);/* Update_1:May 06, 2014,TIME->11:07:10                */

       x+=8;
       *str++;
    }
	
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
        address = SED1335_GRAPHICSTART + (40 * ypos) + (xpos>>3);

        // set memroy write location
        GLCD_WriteCommand(SED1335_CSRW);
    
        GLCD_WriteData((unsigned char)(address & 0xFF));

        GLCD_WriteData((unsigned char)(address >> 8));
      
        // memroy write command
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

