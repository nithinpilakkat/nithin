/****************************************************************************************/
/*                 PROJECTNAME       =              SmartNav             */
/*                 FILENAME                =              TCP.c                      */
/*                 WORKSPNAME       =              SmartNave              */
/*                 DESIGN ENG           =              NITHIN P                             */
/*                 DATE                        =             May 27, 2014                    */
/***************************************************************************************/





#include "SmartNav.h"
#include "TCP.h"


volatile   FnDelay   TCP_Delay;
volatile FnDelay CIPSTATUS_Delay;

void ToSend(enum GprsMsgType MsgType)
{
	switch(MsgType)
	{
		case RegularUpdate:
		//sprintf(GsmPtr->TxBuff,":SL%s_%ld,%s,%s,%d,%d,%d,%d,%d,%d,%d,%s,%s:%d,%u.%d,%d,%d,\r\n"
		sprintf(GsmPtr->TxBuff,":SL%s_%ld,%s,%s,%d,%d,%d,%d,%d,%d,%d,%s,%s,%u.%d,%d,%d,\r\n"
				  	,Navigation.D_Id,
					Navigation.BIDDING_ID,
					Navigation.Lat,
					Navigation.Lon,
				  	Navigation.FARE,
					Navigation.FARE_DEC,
					Navigation.DIST,
					Navigation.DIST_DEC,
				  	Navigation.WAITING_TIME_HOUR,
					Navigation.WAITING_TIME_MIN,
					Navigation.GMODE,
				  	Navigation.SPEED	,
					Navigation.TIMEBUFF,
					//PtrPCF8563->SEC,
					Navigation.ODDMETER,
				  	Navigation.ODDMETER_DEC,
					Navigation.FUEL,
					Navigation.IGNITION);
		break;//RegularUpdate
		
		case ButtonEvt:
		 /*sprintf(GsmPtr->TxBuff,":RP,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\r\n",last_state->FARE,last_state->FARE_DEC,last_state->DIST,last_state->DIST_DEC,(last_state->WAITING_TIME/3600)%12,(last_state->WAITING_TIME/60)%60,
				  mstats->TOTAL_TRIP,mstats->TOTAL_FARE,mstats->TOTAL_FARE_DEC,mstats->TOTAL_PAID_KM,mstats->TOTAL_PAID_KM_DEC,mstats->TOTAL_FREE_KM,mstats->TOTAL_FREE_KM_DEC,mstats->TOTAL_WAITING_HR,mstats->TOTAL_WAITING_MIN,mstats->TOTAL_POWER_FAIL,
                  mstats->DAY_TRIP,mstats->DAY_FARE,mstats->DAY_FARE_DEC,mstats->DAY_PAID_KM,mstats->DAY_PAID_KM_DEC,mstats->DAY_FREE_KM,mstats->DAY_FREE_KM_DEC,mstats->DAY_WAITING_HR,mstats->DAY_WAITING_MIN,mstats->DAY_POWER_FAIL);				  */
				
		break;//ButtonEvt
	}
				   
				  
				  
				  drv_sim900_sendframe(GsmPtr->TxBuff,get_size(GsmPtr->TxBuff));
}




void Tcp()
{
	unsigned static retry=0;
	if(GsmCheckMsg()){
	GsmClearBufffer();
	switch (GsmPtr->Event)
	{
		case T_CsqReceived:
		if(GsmPtr->Rssi<=2)
		{
		GsmPtr->State=CsqLow;	
		}
		else if(GsmPtr->Rssi<99)
		{
			GsmPtr->State=CsqHigh;	
		}
		else if(GsmPtr->Rssi==99)
		{
			GsmPtr->State=CsqNoSignal;
		}
		
		
		break; //CsqReceived
		
		case IP_INITIAL:
		GsmPtr->State=T_CCSTT;
		GsmPtr->Event				= 	EvtInit;
		break;//IP_INITIAL
		
		case IP_GPRSACT:
		GsmPtr->State=T_CCIFSR;
		GsmPtr->Event				= 	EvtInit;
		break;//IP_GPRSACT
			
		case IP_START:
		GsmPtr->State=T_CCIICR;
		GsmPtr->Event				= 	EvtInit;
		break;
	
		case ALREADY_CONNECTED:
		nop();
		GsmPtr->Event				= 	EvtInit;
		GsmPtr->State=TcpConnected;
		break;//ALREADY_CONNECTED
	
	
		case TCP_SEND_OK:
		GsmPtr->Event				= 	EvtInit;
		GsmPtr->State=Con_SENDOK;
		break;//TCP_SEND_OK
		case PDP_DEACT:
		GsmPtr->Event		= 	EvtInit;
		GsmPtr->State		=		T_CCIPSHUT	;
		break;//PDP_DEACT
			
		case TCP_CLOSED:
		GsmPtr->State=T_CCIPSHUT;
		GsmPtr->Tcp_State =	TCP_STATE_CONNECTING;
		GsmPtr->Event				= 	EvtInit;
		break;//TCP_CLOSED
		case TCP_CONNECTING:
		GsmPtr->State=T_CCIPSTATUS;
		break;//TCP_CONNECTING
		case CONNECT_OK:
		GsmPtr->Tcp_State			=	TCP_STATE_CONNECTED;
		GsmPtr->Event				= 	EvtInit;
		GsmPtr->State=TcpConnected;
		break;//CONNECT_OK
	
		case DATA_RECEIVED:
		GsmPtr->Event			= 	EvtInit;
		GsmPtr->State			=	Con_Received;
		break;//DATA_RECEIVED
		case TCP_SHUT_OK:
			GsmPtr->Event				= 	EvtInit;
		GsmPtr->Tcp_State =	TCP_STATE_CONNECTING;			
		GsmPtr->State=T_CCIPSTATUS;
		break;//TCP_SHUT_OK
	
		case	EvtGsmRing:
		break;//EvtGsmRing:
		case 	EvtGsmMsg:
		break;//EvtGsmMsg:
		
		case Tcp_CREG_oK:
		GsmPtr->State				=	T_CCIPSTATUS;
		GsmPtr->Event				= 	EvtInit;
		break;//Tcp_CREG_oK

		case MsgOk:
		GsmPtr->State				=	GsmPtr->NextState;
		GsmPtr->Event				= 	EvtInit;
		GsmClearBufffer();
		break;//MsgOk
		case MsgError:
		GsmPtr->State				=	T_CCIPSTATUS;
		GsmPtr->Event				= 	EvtInit;
		break;//MsgError,

	}}
	
		switch (GsmPtr->State)
				{
				/*case Con_ATE0:
					#ifdef DebugOn
	  			    GsmDisplayUpdate("ATE0    ",1)	;	
					#endif
				    drv_sim900_send_cmd(6,"ATE0\r\n");

					GsmConfig(Con_Running,T_CCREG,Con_ATE0,MsgSend,	MsgOk,Sec(1),GsmPtr->MaxDelay,TimerStart);
					if(GsmPtr->Retry>1)
					{
					GsmPtr->State				=	GsmPtr->NextState;	
					retry=0;
					}
				
				break;	*/
				case CsqLow:
				case CsqHigh:
					sprintf(GsmPtr->Gsm_Sms,"CSQ = %d",GsmPtr->Rssi);// same with csq precedded GsmDisplayUpdate(GsmPtr->_Csq,2);
					GsmPtr->State				=	GsmPtr->NextState;
					GsmPtr->Event				= 	EvtInit;
				break;//csqdetected
				
				case CheckCSQ:
					#ifdef DebugOn
				   	GsmDisplayUpdate("CSQ        ",1)	;	
					#endif	
					drv_sim900_send_cmd(6,"AT+CSQ\r\n");
					GsmConfig(Con_Running,T_CCREG,CheckCSQ,MsgSend,	T_CsqReceived,Sec(3),Sec(10),TimerStart);
					if(GsmPtr->Retry>5)
					{
					GsmPtr->State				=	T_CCIPSHUT;	
					drv_sim900_send_cmd(4,"AT\r\n");//test	
					GsmPtr->Retry=0;
					}
				break;
				case T_CCREG:
							#ifdef DebugOn
							   GsmDisplayUpdate("CCREG    ",1)	;
							#endif
					drv_sim900_send_cmd(get_size(CREG),CREG);
					GsmConfig(Con_Running,T_CCIPSTATUS, T_CCREG,MsgSend,	Tcp_CREG_oK,Sec(2),GsmPtr->MaxDelay,TimerStart);
					if(GsmPtr->Retry>5)
					{
					GsmPtr->State				=	T_CCIPSHUT;	
					GsmPtr->Retry=0;
					}
					break;//T_CCREG


					case T_CCSTT:
					#ifdef DebugOn
						   GsmDisplayUpdate("CSTT     ",1)	;
					#endif
					  GsmClearBufffer();
					  GsmConfig(Con_Running,T_CCIPSTATUS, T_CCSTT,MsgSend,	MsgOk,Sec(3),GsmPtr->MaxDelay,TimerStart);
			          drv_sim900_send_cmd(get_size(CSTT),CSTT);
			          drv_sim900_send_cmd(1,"\"");
					  drv_sim900_send_cmd(get_size("INTERNET"),"INTERNET");
			          drv_sim900_send_cmd(1,"\"");
			          drv_sim900_send_cmd(6,",\"\",\"\""); // ,"",""
			          drv_sim900_send_cmd(2,"\r\n");
					break;//T_CCIPSTATUS,
					case 	T_CIPCLOSE:
					#ifdef DebugOn
						   GsmDisplayUpdate("cipclose     ",1)	;
					#endif
					GsmClearBufffer();
					GsmPtr->Tcp_State =	TCP_STATE_CONNECTING;
					drv_sim900_send_cmd(get_size(CIPCLOSE),CIPCLOSE);
					break;//	CIPCLOSE,

					case T_CCIPCSGP:
					break;//T_CCIPCSGP

					case T_CCIICR:
					#ifdef DebugOn
						   GsmDisplayUpdate("CIICR     ",1)	;
					#endif
					  GsmClearBufffer();
  					  GsmConfig(Con_Running,T_CLPORT, T_CCIICR,MsgSend,	MsgOk,Sec(5),GsmPtr->MaxDelay,TimerStart);					  
					  drv_sim900_send_cmd(get_size(CIICR),CIICR);
					break;//T_CCSTT

					case T_CLPORT :
					#ifdef DebugOn
						   GsmDisplayUpdate("CLPORT    ",1)	;
					#endif
							GsmClearBufffer();
 							GsmConfig(Con_Running,T_CCIFSR, T_CLPORT,MsgSend,MsgOk,Sec(5),GsmPtr->MaxDelay,TimerStart);
							drv_sim900_send_cmd(get_size(CLPORT),CLPORT);
			             	drv_sim900_send_cmd(1,"\"");
			              	drv_sim900_send_cmd(get_size("60"),"60");
			              	drv_sim900_send_cmd(3,"\"\r\n");
				break;//T_CLPORT

					case T_CCIPSTART:
					#ifdef DebugOn
						   GsmDisplayUpdate("CCIPSTART    ",1)	;
					#endif
					GsmClearBufffer();
 					GsmConfig(Con_Running,T_CCIPSTATUS, T_CCIPSTART,MsgSend,	TCP_MSG,Sec(5),GsmPtr->MaxDelay,TimerStart);					
			        drv_sim900_send_cmd(get_size(CIPSTART),CIPSTART);
			        drv_sim900_send_cmd(1,"\"");
			        drv_sim900_send_cmd(get_size(GsmPtr->Ip_ADD),GsmPtr->Ip_ADD);   //"64.27.25.155\",\"10020\"\r\n"
			        drv_sim900_send_cmd(3,"\",\"");
			        drv_sim900_send_cmd(get_size(GsmPtr->PortNo),GsmPtr->PortNo);
			        drv_sim900_send_cmd(3,"\"\r\n");
					break;//T_CCIFSR

					case T_CCIFSR:
					#ifdef DebugOn
						   GsmDisplayUpdate("CCIFSR      ",1)	;
					#endif
					GsmClearBufffer();
			       	drv_sim900_send_cmd(get_size(CIFSR),CIFSR);
					GsmConfig(Con_Running,T_CCIPSTART, T_CCIFSR,MsgSend,	TCP_MSG,Sec(5),GsmPtr->MaxDelay,TimerStart);										
					break;//T_CCIPSTART,


					case T_CCIPSHUT:
							#ifdef DebugOn
						GsmDisplayUpdate("CIPSHUT   	",1)	;
				    	GsmDisplayUpdate("DisConnected  ",2)	;							   
					#endif
							GsmPtr->Tcp_State =	TCP_STATE_CONNECTING;
							drv_sim900_send_cmd(12,"AT+CIPSHUT\r\n");
							GsmConfig(Con_Running,CheckCSQ, CheckCSQ,MsgSend,TCP_MSG,Sec(3),GsmPtr->MaxDelay,TimerStart);	
					break;//T_CCIPSHUT
					case T_CCIPSTATUS:
					
				
						#ifdef DebugOn
							GsmDisplayUpdate("CCIPSTATUS    ",1)	;
						#endif
						GsmClearBufffer();
						drv_sim900_send_cmd(14,CIPSTATUS); //ok state Ip intial
						GsmConfig(Con_Running,T_CCIPSTATUS, T_CCIPSTATUS,MsgSend,	TCP_MSG,Sec(4),GsmPtr->MaxDelay,TimerStart);
					break;//T_CCIPSTATUS

					case TcpConnected:
					#ifdef DebugOn
				    GsmDisplayUpdate("TCP Connected",2)	;	
					#endif
					if(TCP_Delay.Function==FnEnter)
					{
						TCP_Delay.Delay=GsmPtr->L_Tick+ TCP_DELAY_Interval;//10sec
						TCP_Delay.Function=FnExit;
						ToSend(RegularUpdate);	
						GsmConfig(T_CCIPSTATUS,T_CCIPSTATUS, T_CCIPSTATUS,MsgSend,	TCP_SEND_OK,Sec(3),Sec(10),TimerStart);								
					}
					else
					{
						GsmPtr->State=T_CCIPSTATUS;	
						if(GsmPtr->L_Tick>TCP_Delay.Delay)
						{
						TCP_Delay.Function=FnEnter;	
						GsmConfig(T_CCIPSTATUS,T_CCIPSTATUS, T_CCIPSTATUS,MsgSend,	SERVER_LISTENING,Sec(3),GsmPtr->MaxDelay,TimerStart);																
						}				
					}
			
					break;//TcpConnected

					case Con_SENDOK:
					#ifdef DebugOn
				    GsmDisplayUpdate("SEND OK       ",2)	;
					#endif
					GsmConfig(T_CCIPSTATUS,T_CCIPSTATUS, T_CCIPSTATUS,MsgSend,	SERVER_LISTENING,Sec(1),GsmPtr->MaxDelay,TimerStart);
					break;//Con_SENDOK;
					
					case Con_Received:
					#ifdef DebugOn
				    GsmDisplayUpdate("DATA RECEIVED ",2)	;
					#endif
					GsmConfig(T_CCIPSTATUS,T_CCIPSTATUS, T_CCIPSTATUS,MsgSend,	SERVER_LISTENING,Sec(1),GsmPtr->MaxDelay,TimerStart);
					break;//Con_Received
				
					case Con_Running:
					nop();
					break;//Con_Running
				}
	


}




	
