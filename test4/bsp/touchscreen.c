#include "touchscreen.h"
#include "xWidget.h"


extern struct xEvent e;
volatile unsigned char pa;

	void delay1(unsigned int m);
	volatile unsigned short ad_result[4];
	volatile unsigned int touch_x,touch_y;
	extern int sort(const void *x,const void *y); 
	void WriteCharTo7843(unsigned char num)  ;
	unsigned int  ReadFromCharFrom7843();
void __delay_cycles(unsigned long ms)
{ 
	 while(ms--){};
} 
void __no_operation()
{
   nop();	 
}
/*
void delay(unsigned int ms)
{
  while(ms--)
  {
     __delay_cycles(1000);
  }
}
*/



void start() //SPI??   
 {   
	 	DCLK_0;   
	 	CS_1;   
	 	DIN_1;   
	 	DCLK_1;   
		CS_0;   
		delay1(100);   
	    WriteCharTo7843(0xd0);  
	    delay1(200);   
	    DCLK_1; __delay_cycles(BIT_DELAY);
	    DCLK_0; __delay_cycles(BIT_DELAY);
    	ad_result[0]=ReadFromCharFrom7843();   
    	WriteCharTo7843(0x90);  
   		delay1(90);   
    	DCLK_1; __delay_cycles(BIT_DELAY);
    	DCLK_0; __delay_cycles(BIT_DELAY);
    	ad_result[1]=ReadFromCharFrom7843(); 
    	CS_1;
 }


unsigned int  ReadFromCharFrom7843() //SPI ???   
 {   
 unsigned char count=0;   
 unsigned short Num=0;   
    
 for(count=0;count<12;count++)   
 {   
 Num<<=1;   
 DCLK_1;  __delay_cycles(BIT_DELAY);   
 DCLK_0;  __delay_cycles(BIT_DELAY);    
  if(DOUT) 
   Num|=0X01;   
 }   

 for(count=0;count<3;count++)   
 {   

 DCLK_1;  __delay_cycles(BIT_DELAY);   
 DCLK_0;  __delay_cycles(BIT_DELAY);    

 }   
    
 return (Num);   
 }   
    
    
    
 void delay1(unsigned int m)        // 12MHz Xtal, close to ms value   
 {   
  do{__no_operation();}   while(m--);   
 }   


void WriteCharTo7843(unsigned char num)    
 {   
 unsigned char count=0;   
 DCLK_0;   
 for(count=0;count<8;count++)   
 {   
 DIN_0;   
 if(num&0X80) DIN_1;   
 DCLK_0; __delay_cycles(BIT_DELAY);  
 DCLK_1; __delay_cycles(BIT_DELAY);   
 num<<=1;   
 }   
  
  
 } 
int mdown;
//...char d;
extern unsigned char  ir_toggle;

extern EventPointer getPos(void)
{
static unsigned int Y_Max,Y_Min,X_Max,X_Min;	
static struct xEvent Loc;
		Loc.x		=		0;
		Loc.y		=		0;
		Loc.flag	=		0;
	 	Y_Max		=   	3860;
	 	Y_Min	 	=	    230;
	 	X_Max		=	    3860;
	 	X_Min	 	=	    230;
			
while(!PORTE.PORT.BIT.B5){

    start(); //??SPI   
	

	    if((ad_result[0]>Y_Min && ad_result[0]< Y_Max) && (ad_result[1]> X_Min &&ad_result[1]<X_Max) )
	    {
		   Y_Max 	=	ad_result[0] + 100;	
		   Y_Min 	=	ad_result[0] - 100;
		   X_Max 	=	ad_result[1] + 100;
		   X_Min 	=	ad_result[1] - 100;
	       Loc.x	= 	(ad_result[0]	-  190)/11; 
	       Loc.y	= 	(ad_result[1]	-  300)/14;      
	       Loc.x 	=	320				-    Loc.x;  
		   Loc.y 	=	240				-    Loc.y;  
		   Loc.flag	++;
	    }
	if(Loc.flag>2)
		{
		return &Loc;
		}
	}
	
}

extern char getxy()
{

  //if(PORTE.PORT.BIT.B5)//  August 23, 2013  17:43:16  Nithin.p
  if(!PORTE.PORT.BIT.B5)
   {
    
    delay1(100); //??????????,?????????   
    start(); //??SPI   
    delay1(200);   
    WriteCharTo7843(0xd0); //X??,??????   
    delay1(90);   
    DCLK_1; 
    __delay_cycles(BIT_DELAY);
    DCLK_0;   
    __delay_cycles(BIT_DELAY);
    ad_result[0]=ReadFromCharFrom7843();   
    WriteCharTo7843(0x90); //y????(?????) ???   
    delay1(90);   
    DCLK_1; __delay_cycles(BIT_DELAY);
    DCLK_0; __delay_cycles(BIT_DELAY);
    ad_result[1]=ReadFromCharFrom7843(); 
    CS_1;
    if((ad_result[0]>200 && ad_result[0]<4000) && (ad_result[1]>314&&ad_result[1]<4000) )
    {
       touch_x= (ad_result[0]-190)/11; 
       touch_y= (ad_result[1]-300)/14;      
       touch_x =320-touch_x;  
	   touch_y =240-touch_y;  
	   mdown = 1;
	   return 1;
    }
 }
  return 0;
}