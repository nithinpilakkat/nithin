#include <stdio.h>
#include <string.h>
#include "framebuffer.h"
#include "xWidget.h"
#include "bsp/LcdDriver.h"
#include "xEvent.h"

const char *key_row[]=
{
    " Q W E R T Y U I O P ",
    " A S D F G H I J K L ",
    " - Z X C V B N M . < ",
    " * # @ [   ] + < > = "
};
const char *keymap[]=
{
    "QWERTYUIOP",
    "ASDFGHIJKL",
    "-ZXCVBNM.<",
    "*#@   +<>="

};

void * active_form;
unsigned int cursor_x = 350;
unsigned int cursor_y = 260;
unsigned int x,y,w,h,offsetx,offsety;
char str_buf[32];
struct TForm *frm;
struct TButton *bt;
struct TTextbox *tx;
struct TSpinbox *sb;
struct TCombobox *cb;
struct TListbox *lb;
struct TCheckbox *ck;
struct TKeypad *kb;

void TCreateForm(unsigned int id ,void * obj,char * cap,void * next)
{
     frm  			= (struct TForm *) obj;
     frm->p.id 		= 					id;
     frm->p.type 	= 		     TYPE_FORM;
     frm->p.next 	= 			      next;
     frm->caption 	= 		  (char *) cap;
     frm->p.parent	= (struct TForm *) obj;
     
     LcdRect(frm->p.x,frm->p.y,frm->p.width,frm->p.height,1);
     LcdRect(frm->p.x+1,frm->p.y+1,frm->p.width-2,frm->p.height-2,1);
     
     LcdBox(frm->p.x+1,frm->p.y+1,frm->p.width-1,12,1);
     LcdPutStr2(cap,frm->p.x+1,frm->p.y+3);
}  

extern void TCreateButton(void * parent ,void * obj,char * cap,void * next)
{

     bt  = (struct TButton *) obj;
     bt->p.type = TYPE_BUTTON;
     bt->p.next = next;
     bt->caption = (char *) cap;
     bt->p.parent = parent;
     
     x = bt->p.parent->p.x + bt->p.x;
     y = bt->p.parent->p.y + bt->p.y;
     w = bt->p.width;
     h = bt->p.height;
  
     offsetx = x+ ((w-strlen(cap)*8)>>1);
     offsety = y+ ((h-8)>>1);
     
     LcdRect(x,y,w,h,1);
     LcdBox(x+2,y+2,w-4,h-3,1);
     LcdPutStr2(cap,offsetx,offsety+1);//  September 04, 2013  14:46:54  Nithin.p

}


extern void TCreateButton_Cust(void * parent ,void * obj,char * cap,void * next)
{
	/* Update_1:May 09, 2014,TIME->10:52:38  In TCreateButton_Cust Font Size is Different */


     bt  = (struct TButton *) obj;
     bt->p.type = TYPE_BUTTON;
     bt->p.next = next;
     bt->caption = (char *) cap;
     bt->p.parent = parent;
     
     x = bt->p.parent->p.x + bt->p.x;
     y = bt->p.parent->p.y + bt->p.y;
     w = bt->p.width;
     h = bt->p.height;
  
  //   offsetx = x+ ((w-strlen(cap)*8)>>1);
 //    offsety = y+ ((h-8)>>1);
     
     offsetx = x+ ((w-strlen(cap)*10)>>1);  //  September 04, 2013  14:52:03  Nithin.p
     offsety = y+ ((h-10)>>1);	 
	 
     LcdRect(x,y,w,h,1);
     LcdBox(x+2,y+2,w-4,h-3,1);
//	 LcdPutMsg2(cap,offsetx,offsety+1);
//   LcdPutStr2(cap,offsetx,offsety+1);//  September 04, 2013  14:46:54  Nithin.p
	 LcdPutStr1(cap,offsetx,offsety-6);
//   LcdPutStr3(cap,offsetx,offsety+1,2);
}

void TCreateCheckbox(void * parent ,void * obj,char * cap,void * next)
{
     ck  = (struct TCheckbox *) obj;
     ck->p.type = TYPE_CHECKBOX;
     ck->p.next = next;
     ck->caption = (char *) cap;
     ck->p.parent = parent;
     
     x = ck->p.parent->p.x + ck->p.x;
     y = ck->p.parent->p.y + ck->p.y;
     w = ck->p.width;
     h = ck->p.height;
     offsetx = x+w+2;
     offsety = y+ ((h-8)>>1);
     
     LcdRect(x,y,w,h,1);
     //LcdBox(x+2,y+2,w-4,h-3,1);
     LcdPutStr2(cap,offsetx,offsety+1);
  
}

void TCreateTextbox(void * parent ,void * obj,char * text,void * next)
{

     tx  = (struct TTextbox *) obj;
     tx->p.type = TYPE_TEXTBOX;
     tx->p.next = next;
     tx->text = (char *) text;
     tx->p.parent = parent;
     
     x = tx->p.parent->p.x + tx->p.x;
     y = tx->p.parent->p.y + tx->p.y;
     w = tx->p.width;
     h = tx->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);
     
     LcdRect(x,y,w,h,1);
     LcdPutStr3(tx->text,offsetx,offsety+1,1);
}
void TCreateSpinbox(void * parent ,void * obj,int value,void * next)
{
     sb  = (struct TSpinbox *) obj;
     sb->p.type = TYPE_SPINBOX;
     sb->p.next = next;
     sb->value = value;
     sb->p.parent = parent;
     
     x = sb->p.parent->p.x + sb->p.x;
     y = sb->p.parent->p.y + sb->p.y;
     w = sb->p.width;
     h = sb->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);

     LcdBox(x,y,w,h,0);
     LcdRect(x,y,w,h,1);

     LcdPutStr3("+",x+(w>>1)-3,y+2,1);
     LcdBox(x+2,y+2,w-4,8,2);
     
     LcdPutStr3("-",x+(w>>1)-3,(y+h)-9,1);
     LcdBox(x+2,(y+h)-9,w-4,8,2);
     
     sprintf(str_buf,"%02d",sb->value);
     LcdPutStr3(str_buf,offsetx,offsety+1,1);
  
}

void TCreateCombobox(void * parent ,void * obj,char * list,void * next)
{

     cb  = (struct TCombobox *) obj;
     cb->p.type = TYPE_COMBOBOX;
     cb->p.next = next;
     cb->list = list;
     cb->p.parent = parent;
     cb->state = 0;  // close
     cb->index = 0;
     x = cb->p.parent->p.x + cb->p.x;
     y = cb->p.parent->p.y + cb->p.y;
     w = cb->p.width;
     h = cb->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);
     
     LcdRect(x,y,w,h,1);
     LcdPutStr3("+",x+w-9,y+4,1);
     LcdBox(x+w-10,y+2,8,h-3,2);
      LcdPutStr3(&cb->list[0],offsetx,offsety,1); 

}


void TCreateListbox(void * parent ,void * obj,char * list,void * next)
{

     cb  = (struct TCombobox *) obj;
     cb->p.type = TYPE_LISTBOX;
     cb->p.next = next;
     cb->list = list;
     cb->p.parent = parent;
     cb->state = 0;  // close
     cb->index = 0;
     x = cb->p.parent->p.x + cb->p.x;
     y = cb->p.parent->p.y + cb->p.y;
     w = cb->p.width;
     h = cb->p.height;
     offsetx = x+2;
     offsety = y+h-10;
     
     LcdRect(x,y,w,h,1);
     LcdPutStr3("<<",x+(w>>2)-8,offsety,1);
     LcdBox(x+1,y+h-16,(w>>1)-3,16,2);
     LcdPutStr3(">>",x+(w>>1)+(w>>2)-8,offsety,1);
     LcdBox(x+(w>>1)+2,y+h-16,(w>>1)-3,16,2);
}


void TCreateKeypad(void * parent ,void * obj,void * next)
{
     kb  = (struct TKeypad *) obj;
     kb->p.type = TYPE_KEYPAD;
     kb->p.next = next;
     kb->p.parent = parent;

     x = kb->p.parent->p.x + cb->p.x;
     y = kb->p.parent->p.y + cb->p.y;
     w = kb->p.width;
     h = kb->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);
     if(kb->state == 1)
     {
       LcdBox(x,y,w,h,0);
       LcdRect(x,y,w,h,1);
              
     }
}

void  TRedrawKeypad(void * obj)
{
     kb  = (struct TKeypad *) obj;
     x = kb->p.parent->p.x + kb->p.x;
     y = kb->p.parent->p.y + kb->p.y;
     w = kb->p.width;
     h = kb->p.height;
     //offsetx = x+2;
     //offsety = y+ ((h-8)>>1);
     if(kb->state == 1)
     {
       LcdBox(x,y,w,h,0);
       LcdRect(x,y,w,h,1);
       LcdPutStr3((char *)key_row[0],x+1,y+4+2,1);
           //LcdLine(x+1,y+14,x+169,y+14,1);
       LcdPutStr3((char *)key_row[1],x+1,y+14+2,1);
           //LcdLine(x+1,y+24,x+169,y+24,1);
       LcdPutStr3((char *)key_row[2],x+1,y+24+2,1);
           //LcdLine(x+1,y+34,x+169,y+34,1);
       LcdPutStr3((char *)key_row[3],x+1,y+34+2,1);

     }
     if(kb->state == 0)
     {
       LcdBox(x,y,w,h+1,0);
       //TRedrawForm(&active_form);
     }
   
}
void TRedrawForm(void * obj)
{  
     struct TControl * t = (struct TControl *) obj;
     
     while(t)
     {
         switch(t->type)              //call redraw function
         {
             
           case TYPE_FORM:
                  
             break;
           case TYPE_BUTTON:
                TRedrawButton((struct TButton *)t);
             break;
               
           case TYPE_TEXTBOX:
                TRedrawTextbox((struct TTextbox *)t);
             break;
               
           case TYPE_SPINBOX:
                TRedrawSpinbox((struct TSpinbox *)t); 
             break;
           case TYPE_COMBOBOX:
                 TRedrawCombobox((struct TCombobox *)t);  
             break;
         case TYPE_LISTBOX:
                TRedrawListbox((struct TListbox *)t);
           break;
         case TYPE_CHECKBOX:
               TRedrawCheckbox((struct TCheckbox *)t);
           break;
           
           case TYPE_KEYPAD:
                //TRedrawKeypad((struct TKeypad *)t);  
             break;
             
         }
        t = t->next;
     }
  
}
void TRedrawTextbox(void * obj)
{

     struct TTextbox *tx  = (struct TTextbox *) obj;
    
     x = tx->p.parent->p.x + tx->p.x;
     y = tx->p.parent->p.y + tx->p.y;
     w = tx->p.width;
     h = tx->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);

     LcdBox(x,y,w,h,0);
     LcdRect(x,y,w,h,1); 
     LcdPutStr3(tx->text,offsetx,offsety+1,1);
}


void TRedrawButton(void * obj)
{
     struct TButton *bt  = (struct TButton *) obj;
     x = bt->p.parent->p.x + bt->p.x;
     y = bt->p.parent->p.y + bt->p.y;
     w = bt->p.width;
     h = bt->p.height;
     offsetx = x+ ((w-strlen(bt->caption)*8)>>1);
     offsety = y+ ((h-8)>>1);

     LcdBox(x,y,w,h,0);  //clear component

     LcdRect(x,y,w,h,1);
     LcdBox(x+2,y+2,w-4,h-3,1);
     LcdPutStr2(bt->caption,offsetx,offsety+1);
}

void TRedrawSpinbox(void * obj,struct xEvent * e)
{
     struct TSpinbox *sb  = (struct TSpinbox *) obj;
     
     x = sb->p.parent->p.x + sb->p.x;
     y = sb->p.parent->p.y + sb->p.y;
     w = sb->p.width;
     h = sb->p.height;
	 if(e->message == SPIN_DOWN){
		 --(sb->value);
	 }
	 else if(e->message == SPIN_UP){
		 ++(sb->value);
	 }
	 
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);
     
     LcdBox(x,y,w,h,0);
     LcdRect(x,y,w,h,1);

     LcdPutStr3("+",x+(w>>1)-3,y+2,1);
     LcdBox(x+2,y+2,w-4,8,2);
     
     LcdPutStr3("-",x+(w>>1)-3,(y+h)-9,1);
     LcdBox(x+2,(y+h)-9,w-4,8,2);
     
     sprintf(str_buf,"%02d",sb->value);
     LcdPutStr3(str_buf,offsetx,offsety+1,1);
}

void TRedrawCombobox(void * obj)
{
     char i,n;
     struct TCombobox *cb  = (struct TCombobox *) obj;
     x = cb->p.parent->p.x + cb->p.x;
     y = cb->p.parent->p.y + cb->p.y;
     w = cb->p.width;
     h = cb->p.height;
     offsetx = x+2;
     offsety = y+ ((h-8)>>1);
     
     LcdBox(x,y,w,h,0);
     LcdRect(x,y,w,h,1);
     LcdPutStr3("+",x+w-9,y+4,1);
     LcdBox(x+w-10,y+2,8,h-3,2);
     
     //--LcdPutStr3(&cb->list[cb->index][0],offsetx,offsety,1);  
     i = 0; 
     
     n = 0;
     while(1)
     {
        if(cb->index == i) 
        {
           LcdPutStr3(&cb->list[n],offsetx,offsety,1);  
        }
        if(strlen(&cb->list[n]) == 0) break;
        n += strlen(&cb->list[n])+2;
        i++;
     }

}

void TRedrawListbox(void * obj)
{

     cb  = (struct TCombobox *) obj;
     //cb->p.type = TYPE_LISTBOX;
     //cb->p.next = next;
     //cb->list = list;
     //cb->p.parent = parent;
     //cb->state = 0;  // close
     //cb->index = 0;
     x = cb->p.parent->p.x + cb->p.x;
     y = cb->p.parent->p.y + cb->p.y;
     w = cb->p.width;
     h = cb->p.height;
     offsetx = x+2;
     offsety = y+h-10;
     
     LcdBox(x,y,w,h,0);
     LcdRect(x,y,w,h,1);
     LcdPutStr3("<<",x+(w>>2)-8,offsety,1);
     LcdBox(x+1,y+h-16,(w>>1)-3,16,2);
     LcdPutStr3(">>",x+(w>>1)+(w>>2)-8,offsety,1);
     LcdBox(x+(w>>1)+2,y+h-16,(w>>1)-3,16,2);
}

void TRedrawCheckbox(void * obj)
{
     ck  = (struct TCheckbox *) obj;
     //ck->p.type = TYPE_CHECKBOX;
     //ck->p.next = next;
     //ck->caption = (char *) cap;
     //ck->p.parent = parent;
     
     x = ck->p.parent->p.x + ck->p.x;
     y = ck->p.parent->p.y + ck->p.y;
     w = ck->p.width;
     h = ck->p.height;
     offsetx = x+w+2;
     offsety = y+ ((h-8)>>1);
     
     LcdRect(x,y,w,h,1);
     //LcdBox(x+2,y+2,w-4,h-3,1);
     LcdPutStr3(ck->caption,offsetx,offsety+1,1);
  
}



void TCursorUpdate(void * obj)
{
  struct TControl * t = (struct TControl *) obj;
  mTx = (struct TTextbox *) obj;

  x = t->parent->p.x + t->x;
  y = t->parent->p.y + t->y;
  
  if(t->type == TYPE_TEXTBOX)
  {
    LcdBox(cursor_x,cursor_y,8,11,2);  //clear prev. cursor
    cursor_x = x+2 + strlen(mTx->text)*8;
    cursor_y = y+2;
    LcdBox(cursor_x,cursor_y,8,11,1);  //set cursor to new location
  }
}
