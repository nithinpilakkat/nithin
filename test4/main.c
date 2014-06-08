
#include "SmartNav.h"

#define GpsOn

#ifndef GpsOn
	#define GpsOff
#endif

#define TimeAdvanceHr  5 	//india +5.30
#define TimeAdvanceMin 30  //india +5.30

SmartNav Navigation;
uchar DateTime[40]={0};

enum Button_State{
	KEY_Press,
	KEY_Init,
	
};

enum KeyEvent{
	Key_Hired,
	Key_Topay,
	Key_Vacant,
	Key_Status,
	Key_Accept,
	Key_Engage,
	Key_Engaged,
	Key_Start,
};


struct _Button{
enum Button_State B_Hired,B_Topay,B_Vacant,B_Status,B_Accept,B_Engage,B_Engaged;
enum KeyEvent B_Any;	
};
static struct _Button Key;
enum Taxi_State Taxi_Mode;


void Init_Display()
{
	CreateForm_Start();
}


void MainDispaly() //called from interrupt
{
    static EventPointer Location;
		Location 		=	getPos();
		if((Location->flag)>2){
		e.x				=	Location->x	;	
		e.y				=	Location->y	;
		active_form	 	=	&Tfrm	;
		xEventDispatch( &e);
		}

}

void InitVariable()
{
					strcpy(Navigation.D_Id,"60");
					Navigation.BIDDING_ID=0;
					Navigation.Lat=GpsPtr->Lat;
					Navigation.Lon=GpsPtr->Lon;
				  	Navigation.FARE=513;
					Navigation.FARE_DEC=65;
					Navigation.DIST=23;
					Navigation.DIST_DEC=43;
				  	Navigation.WAITING_TIME_HOUR=12;
					Navigation.WAITING_TIME_MIN=10;
					Navigation.GMODE=Vaccant_Mode;
				  	strcpy(Navigation.SPEED,"1:0");
					strcpy(Navigation.TIMEBUFF,"10:11:13");
					Navigation.TIMEBUFF[8]=0;
					Navigation.ODDMETER=12345;
				  	Navigation.ODDMETER_DEC=12;
					Navigation.FUEL=0;
					Navigation.IGNITION=0;
					Navigation.RTC_State=RTC_Init;
					Navigation.Engage_Status=Engage_State;
					
					//key press//
					Key.B_Hired		=KEY_Init;
					Key.B_Topay		=KEY_Init;
					Key.B_Vacant	=KEY_Init;
					Key.B_Status	=KEY_Init;
					Key.B_Accept	=KEY_Init;
					Key.B_Engage	=KEY_Init;
					Key.B_Engaged	=KEY_Init;
					Key.B_Any		=Key_Start;
					
					
					Taxi_Mode		=	Vacant_State;
}

int main( void )
{
uchar temp[12];


	Init_SmartNavi();
	InitVariable();
	GpsInit();
    Init_Display();  // create the form, button and their events
	
	DisplayForm_Start();
	__enable_interrupt();
	
	
	    while(1)
        {
			GsmRun();
			#ifdef GpsOn
			if(GpsPtr->State==UPDATED)
			{
			GpsPtr->State=STARTED;
			gps_process();
			tone(1);
					switch(Navigation.RTC_State)
					{
				
						case RTC_Init:
						PtrPCF8563=Read_RTC();
						temp[0]=' ';
						temp[1]=GpsPtr->Time[0];
						temp[2]=GpsPtr->Time[1];
						temp[3]=0;
					    PtrPCF8563->HOUR=atoi(&temp[0])+TimeAdvanceHr;
						temp[0]=' ';
						temp[1]=GpsPtr->Time[2];
						temp[2]=GpsPtr->Time[3];
						temp[3]=0;
					    PtrPCF8563->MIN=atoi(&temp[0])+TimeAdvanceMin;	
						if(PtrPCF8563->MIN>59)
						{
							PtrPCF8563->HOUR=PtrPCF8563->HOUR+1;
							PtrPCF8563->MIN=(PtrPCF8563->MIN-60);
						}
						else
						{
						nop()	;
						}
						sendtoRTC(PtrPCF8563->SEC,
								  PtrPCF8563->MIN,
								  PtrPCF8563->HOUR,
								  PtrPCF8563->DAYS,
								  PtrPCF8563->WEEKDAYS,
								  PtrPCF8563->MONTH,			
								  PtrPCF8563->YEAR);
						Navigation.RTC_State=RTC_RUN;
						break;
				
						case RTC_RUN:
							PtrPCF8563=Read_RTC(); // 7 bytes data
						break;
						
					}//Navigation.RTC_State
			TCreateTextbox		(&Tfrm	,&TtbLat	, GpsPtr->Lat	    ,&TtbLat	);
			TCreateTextbox		(&Tfrm	,&TtbLon	, GpsPtr->Lon	    ,&TtbLon	);
			TCreateTextbox		(&Tfrm	,&TtbSpeed	, GpsPtr->Time	    ,&TtbSpeed	);
			
			sprintf(&Navigation.TIMEBUFF,"%2d:%2d:%2d",(uchar)PtrPCF8563->HOUR,PtrPCF8563->MIN,PtrPCF8563->SEC);
			Navigation.TIMEBUFF[8]=0;
			TCreateTextbox		(&Tfrm	,&TtbRtc	, &Navigation.TIMEBUFF   ,&TtbRtc	);

			LcdUpdate();
		}
			else
			{
				;
			}
			#endif
			if(GsmPtr->Display==Update)
			{
			GsmPtr->Display=Done;
			TCreateTextbox		(&Tfrm	,&TtbGsm	, GsmPtr->Gsm_Status,&TtbGsm	);
			TCreateTextbox		(&Tfrm	,&TtbSms	, GsmPtr->Gsm_Sms   ,&TtbSms	);
					
			LcdUpdate();
			}
			else
			{
				;
			}
			
			if(eFrm1.flag)			// value changes from 
			{
				eFrm1.flag=0;
  			switch (eFrm1.message)
			{
				case Form1Id:
					break;
				case BtHiredId:
				//eBt1.flag	=	1;
				Key.B_Any		=Key_Hired;
				Key.B_Hired		=KEY_Press;
				break;
				case BtTopayId:
				Key.B_Any		=Key_Topay;
				Key.B_Topay		=KEY_Press;
				//eBt2.flag	=	1;
					break;
				case BtVacentId:
				Key.B_Any		=Key_Vacant;
				Key.B_Vacant	=KEY_Press;				
				eBt3.flag	=	1;				
					break;
				case BtStatusId:
				Key.B_Any		=Key_Status;
				Key.B_Status		=KEY_Press;				
				eBt4.flag	=	1;				
					break;
				case BtAcceptId:
				Key.B_Any		=Key_Accept;
				Key.B_Accept		=KEY_Press;					
				eBt5.flag	=	1;				
					break;
				case BtEngageId:
				Key.B_Any		=Key_Engage;
				Key.B_Engage		=KEY_Press;					
				eBt6.flag	=	1;				
					break;
				case BtEngagedId:
				Key.B_Any		=Key_Engaged;
				Key.B_Engaged		=KEY_Press;					
				eBt7.flag	=	1;				
					break;
			}
			if(Key.B_Any!=Key_Start)
			{
				switch (Taxi_Mode)
				{
					case Vacant_State:
					switch (Key.B_Any)
							{
								case 	Key_Hired:
								Taxi_Mode=Hired_State;
								LcdClear();	
								tone(1);
								Navigation.GMODE=HiredDAY_AC_mode;
								break;//Key_Hired
								/*case 	Key_Topay:
								break;//Key_Topay:*/
						
							/*	case 	Key_Vacant:
								tone(1);
								break;//Key_Vacant:
						*/
								case 	Key_Status:
								tone(1);
								break;//Key_Status
						
								case	Key_Accept:
								Navigation.GMODE=Bid_Accept_MOde;
								tone(1);
								break;//Key_Accept:
						
								case    Key_Engage:
									tone(1);
									Navigation.Engage_Status=Engage_State;
								break;//  Key_Engage:
						
								case 	Key_Engaged:
									tone(1);
									Navigation.Engage_Status=Engaged_State;						
								break;//	Key_Engaged:
						}
					
					
					break;//Vacant_State
					
					case Hired_State:
					if(Navigation.GMODE==Bid_Accept_MOde)
					{
					Navigation.GMODE=Transit_mode;	
					}
						switch (Key.B_Any)
					{
						/*case 	Key_Hired:
						Taxi_Mode=Hired_State;
						break;//Key_Hired*/
						case 	Key_Topay:
						tone(1);
						Navigation.GMODE=Topay_Mode;						
						Taxi_Mode=Topay_State;
						break;//Key_Topay:
						
						/*case 	Key_Vacant:
						break;//Key_Vacant:
						
						case 	Key_Status:
						break;//Key_Status
						
						case	Key_Accept:
						break;//Key_Accept:
						
						case    Key_Engage:
							Navigation.Engage_Status=Engage_State;
						break;//  Key_Engage:
						
						case 	Key_Engaged:
							Navigation.Engage_Status=Engaged_State;						
						break;//	Key_Engaged:*/
						
					
					}
					
					
					break;//Hired_State
					
					case Topay_State:
						switch (Key.B_Any)
					{
						/*case 	Key_Hired:
						Taxi_Mode=Hired_State;
						break;//Key_Hired
						case 	Key_Topay:

						break;//Key_Topay:
						
						*/
						case 	Key_Vacant:
										tone(1);
						Taxi_Mode=Vacant_State;
						Navigation.GMODE=Vaccant_Mode;
						break;//Key_Vacant:
						/*
						case 	Key_Status:
						break;//Key_Status
						
						case	Key_Accept:
						break;//Key_Accept:
						
						case    Key_Engage:
							Navigation.Engage_Status=Engage_State;
						break;//  Key_Engage:
						
						case 	Key_Engaged:
							Navigation.Engage_Status=Engaged_State;						
						break;//	Key_Engaged:*/
						
					
					}
					
					
					
					break;//Topay
				}
			if(GsmPtr->Tcp_State ==	TCP_STATE_CONNECTED)
			{
										ToSend(RegularUpdate);	
			}
			DisplayForm_Start();
				
			Key.B_Any=Key_Start	;
			}
			
			
			}
	    }
}


