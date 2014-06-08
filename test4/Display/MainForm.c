#include "MainForm.h"
#include "CreateObj.h"
#include "bsp/Bsp.h"
#include "SmartNav.h"
#define ButtonLoc_x 230
#define ButtonSize  80


void CreateForm_Start(void)
{
		CreatFormObj(&Tfrm,0  	,0  ,320,240,  Form1Id		,TYPE_FORM  )	;  //x 320 y 240
		CreatFormObj(&TB_Hired 	,ButtonLoc_x, 15, ButtonSize, 40,BtHiredId	,TYPE_BUTTON)	;	//Hired 
		CreatFormObj(&TB_Topay 	,ButtonLoc_x, 15, ButtonSize, 40,BtTopayId	,TYPE_BUTTON)	;	//Topay
		CreatFormObj(&TB_VACANT ,ButtonLoc_x, 60, ButtonSize, 40,BtVacentId	,TYPE_BUTTON)	;	//vacent
		
		CreatFormObj(&TB_Status ,ButtonLoc_x,105,ButtonSize ,40 ,BtStatusId	,TYPE_BUTTON)	;	
		CreatFormObj(&TB_Accept	,ButtonLoc_x,150,ButtonSize ,40 ,BtAcceptId ,TYPE_BUTTON)	;
		CreatFormObj(&TB_Engage	,ButtonLoc_x,195,ButtonSize ,40 ,BtEngageId	,TYPE_BUTTON)	;
		CreatFormObj(&TB_Engaged,ButtonLoc_x,195,ButtonSize ,40 ,BtEngagedId,TYPE_BUTTON)	;
		
		CreatFormObj(&TtbLat	, 10, 15	,100  ,15 ,TBLatId    ,TYPE_TEXTBOX)	; 
		CreatFormObj(&TtbLon	, 10, 35	,100  ,15 ,TBLonId    ,TYPE_TEXTBOX)	; 
		CreatFormObj(&TtbSpeed	, 10, 55	,50   ,15 ,TBSpdId    ,TYPE_TEXTBOX)	; 
		CreatFormObj(&TtbGsm	, 10, 75	,150  ,15 ,TBGsmId    ,TYPE_TEXTBOX)	; 
		CreatFormObj(&TtbSms	, 10, 95	,150  ,15 ,TBSmsId    ,TYPE_TEXTBOX)	; 
		CreatFormObj(&TtbRtc	, 10, 115	,100  ,15 ,TBRtcId    ,TYPE_TEXTBOX)	;
			
	
		
	//	TCreateButton	(&Tfrm	,&Tb7	,"Topay "	,&Tb1	);					
	//	TCreateCheckbox	(&Tfrm	,&Tcb1	,"CkBox"	,&Ttb1	);
	//	TCreateTextbox	(&Tfrm	,&Ttb1	,"Tbox"		,&Tsb1	);
	//	TCreateSpinbox	(&Tfrm	,&Tsb1	,0			,&Tb1	);
	//	TCreateCombobox	(&Tfrm	,&TCm1	,"Combo"	,&TLb1	);
	//	TCreateListbox	(&Tfrm	,&TLb1	,"List"		,&Tb1	);
	//	TCreateKeypad	(&Tfrm	,&TKp1	,&TKp2);		
}

void DisplayForm_Start()
{
	static char i=0;	//Some dispaly issue


			switch(Taxi_Mode)
			{
			case Vacant_State:
						TCreateForm			(1		,&Tfrm		,"Smart meter "	,&TB_Hired	);				
						TCreateButton_Cust	(&Tfrm	,&TB_Hired	,"Hired "		,&TB_VACANT	);
				if(i==0)
				{
					i=1;
				}
				else
				{
					nop();
				}
			break;
				case Hired_State:
				i=0;
				TCreateForm			(1		,&Tfrm		,"Hired       "	 ,&TB_Topay	);
				TCreateButton_Cust	(&Tfrm	,&TB_Topay	,"Topay "		 ,&TB_VACANT )	;

				break;
			case Topay_State:
				i=0;
		 		TCreateForm			(1		,&Tfrm		,"Topay       "	 ,&TB_Topay	);
				TCreateButton_Cust	(&Tfrm	,&TB_Topay	,"Topay "		 ,&TB_VACANT )	;
			break;
			}
			TCreateButton_Cust	(&Tfrm	,&TB_VACANT	,"Vacant"			,&TB_Status	);					
			TCreateButton_Cust	(&Tfrm	,&TB_Status	,"Status"			,&TB_Accept);
			TCreateButton_Cust	(&Tfrm	,&TB_Accept	,"Accept"			,&TB_Engage);
			if(Navigation.Engage_Status==Engage_State)
			{
			TCreateButton_Cust	(&Tfrm	,&TB_Engage,"Engage "			,&Tfrm	);	
			}
			else
			{
			TCreateButton_Cust	(&Tfrm	,&TB_Engaged,"Engaged "			,&Tfrm	);
			}
			
			if(i==1)
			{
				i=2;
			show_forhire(5,180);
			}
			else
			{
				nop();
			}
			
			


		LcdUpdate();
		//TCreateButton	(&Tfrm	,&Tb5	,"Accept"		,&Tb6	);	
		//TCreateButton	(&Tfrm	,&Tb6	," Call "		,&Tb7	);

}

/*
void CreateFrom_Hired(void)//graphics
{
		CreatFormObj(&Tfrm1,0  		,0  ,320,240,0,TYPE_FORM  )	;
		CreatFormObj(&TB_Hired  	,20 ,180,100,50 ,1,TYPE_BUTTON)	;
		CreatFormObj(&TB_Topay  	,130,180,100,50 ,2,TYPE_BUTTON)	;
		CreatFormObj(&TB_VACANT  	,240, 95,70 ,30 ,3,TYPE_BUTTON)	;
		CreatFormObj(&TB_Status  	,240,130,70 ,30 ,4,TYPE_BUTTON)	;
		CreatFormObj(&TB_Accept  	,240,165,70 ,30 ,5,TYPE_BUTTON)	;
		CreatFormObj(&TB_Engage		,240,200,70 ,30 ,6,TYPE_BUTTON)	;
	 
}

void DisplayFrom_Hired(void)
{
		TCreateForm		(1		,&Tfrm1	,"Hired "	        ,&TB_Hired	);
		TCreateButton	(&Tfrm	,&TB_Hired	,"Topay "			,&TB_Topay	);
		TCreateButton	(&Tfrm	,&TB_Topay	,"Vacant"			,&TB_VACANT	);	
		TCreateButton	(&Tfrm	,&TB_VACANT	,"Status"			,&TB_Status	);
		TCreateButton	(&Tfrm	,&TB_Status	,"Accept"			,&TB_Accept	);	
		TCreateButton	(&Tfrm	,&TB_Accept	,"Engage"			,&TB_Engage	);
		TCreateButton	(&Tfrm	,&TB_Engage	," SoS  "			,&TB_Hired	);
		show_forhire(15,139);
		LcdUpdate();
	
}*/