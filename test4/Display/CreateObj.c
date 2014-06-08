#include "CreateObj.h"
#include "xWidget.h"
#include "xEvent.h"
#include "framebuffer.h"




	struct TButton  	TB_Hired,TB_Topay,TB_VACANT,TB_Status,TB_Accept,TB_Engage,TB_Engaged;
	struct TForm		Tfrm,Tfrm1,Tfrm2;
	struct xEvent		e,eFrm1,eBt1,eBt2,eBt3,eBt4,eBt5,eBt6,eBt7,eBt8;
	struct TCheckbox	Tcb1,Tcb2;
	struct TTextbox 	TtbLat,TtbLon,TtbSpeed,TtbGsm,TtbSms,TtbRtc;
	struct TSpinbox 	Tsb1,Tsb2;
	struct TCombobox	TCm1,TCm2;
	struct TListbox 	TLb1,TLb2;
 	struct TKeypad 		TKp1,TKp2;
	

		
	


void CreatButtonObject(struct TForm *Tb,unsigned int id,unsigned int x,unsigned int y,unsigned int wt,unsigned int ht,unsigned char type)
{
	Tb->p.id 		=	id;
	Tb->p.x 		=	x;
	Tb->p.y 		=	y;
	Tb->p.width		=	wt;
	Tb->p.height	=	ht;
	Tb->p.type 		=	type;
	Tb->p.parent 	=	0;	
}

void CreatFormObj(void * Obj,unsigned int x,unsigned int y,unsigned int wt,unsigned int ht,unsigned int id,unsigned char type)
{
	static struct TForm 	*Frm;
	static struct TButton 	*Tb;
	static struct TCheckbox *Tcb;
	static struct TTextbox  *TTb;
	static struct TSpinbox  *TSb;
	static struct TListbox  *TLb;
	static struct TCombobox	*TCm;
	static struct TKeypad 	*TKp;
	Frm->p.id 			=	   0;
	switch(type){
		case TYPE_FORM:
			Frm 				=	(struct TForm *)Obj;
			Frm->p.x 			=	x;
			Frm->p.y 			=	y;
			Frm->p.width		=	wt;
			Frm->p.height		=	ht;
			Frm->p.type 		=	type;
			Frm->p.parent 		=	0;	
			//Frm->caption		=   name;
		//	Frm->p.callback		=	
		break;
		case TYPE_BUTTON:
			
			Tb					=	(struct TButton *)Obj;
			Tb->p.id 			=	id;
			Tb->p.x 			=	x;
			Tb->p.y 			=	y;
			Tb->p.width			=	wt;
			Tb->p.height		=	ht;
			Tb->p.type 			=	type;
			Tb->p.parent 		=	0;	
			Tb->p.callback		=	&TButtonCallback;
		break;
		
		case TYPE_CHECKBOX:
			
			Tcb					=	(struct TCheckbox *)Obj;
			Tcb->p.id 			=	id;
			Tcb->p.x 			=	x;
			Tcb->p.y 			=	y;
			Tcb->p.width		=	wt;
			Tcb->p.height		=	ht;
			Tcb->p.type 		=	type;
			Tcb->p.parent 		=	0;	
			Tcb->p.callback		=	&TCallbackCheckbox;
		break;
		
		case TYPE_TEXTBOX:
			
			TTb					=	(struct TTextbox *)Obj;
			TTb->p.id 			=	id;
			TTb->p.x 			=	x;
			TTb->p.y 			=	y;
			TTb->p.width		=	wt;
			TTb->p.height		=	ht;
			TTb->p.type 		=	type;
			TTb->p.parent 		=	0;	
			TTb->p.callback		=	&TTextboxCallback;
		break;
		
		case TYPE_SPINBOX:
			
			TSb					=	(struct TSpinbox *)Obj;
			TSb->p.id 			=	id;
			TSb->p.x 			=	x;
			TSb->p.y 			=	y;
			TSb->p.width		=	wt;
			TSb->value			=	0;
			TSb->min			=	0;
			TSb->max			=	1000;
			TSb->ndigit			=	6;
			TSb->p.height		=	ht;
			TSb->p.type 		=	type;
			TSb->p.parent 		=	0;	
			TSb->p.callback		=	&TSpinboxCallback;
		break;
		
		case TYPE_LISTBOX:
			
			TLb					=	(struct TListbox *)Obj;
			TLb->p.id 			=	id;
			TLb->p.x 			=	x;
			TLb->p.y 			=	y;
			TLb->p.width		=	wt;
			TLb->p.height		=	ht;
			TLb->p.type 		=	type;
			TLb->p.parent 		=	0;	
			TLb->p.callback		=	&TCallbackListbox;
		break;
		
		case TYPE_COMBOBOX:
			
			TCm					=	(struct TCombobox *)Obj;
			TCm->p.id 			=	id;
			TCm->p.x 			=	x;
			TCm->p.y 			=	y;
			TCm->p.width		=	wt;
			TCm->p.height		=	ht;
			TCm->p.type 		=	type;
			TCm->p.parent 		=	0;	
			TCm->p.callback		=	&TComboboxCallback;
		break;
		
		case TYPE_KEYPAD:
			
			TKp					=	(struct TKeypad *)Obj;
			TKp->p.id 			=	id;
			TKp->p.x 			=	x;
			TKp->p.y 			=	y;
			TKp->p.width		=	wt;
			TKp->p.height		=	ht;
			TKp->p.type 		=	type;
			TKp->p.parent 		=	0;	
			TKp->p.callback		=	&TKeypadCallback;
		break;
		
			}
	
}

