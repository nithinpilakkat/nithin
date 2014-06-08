#include <string.h>

#include "xWidget.h"
#include "framebuffer.h"
#include "bsp/LcdDriver.h"
#include "xEvent.h"



extern const char *keymap[];
struct TControl * o;
struct TButton *  mBt;
struct TTextbox * mTx;
struct TSpinbox * mSb;
struct TCombobox * mCb;
struct TCheckbox *mCk;
struct TKeypad   kb1;

struct TControl *restore1;
struct TControl *restore2;
struct xEvent   ke;
extern struct xEvent	eBt1,ecb1;



void TButtonCallback(void * obj)
{
     struct TButton *bt  = (struct TButton *) obj;
     x = bt->p.parent->p.x + bt->p.x;
     y = bt->p.parent->p.y + bt->p.y;
     w = bt->p.width;
     h = bt->p.height;
     offsetx = x+ ((w-strlen(bt->caption)*8)>>1);
     offsety = y+ ((h-8)>>1);
     LcdBox(x+2,y+2,w-4,h-3,2);  //XOR Inner box
     LcdUpdate();
     LcdDelay(100);////  1000 to 10 September 04, 2013  15:06:33  Nithin.p
     LcdBox(x+2,y+2,w-4,h-3,2);  //XOR Inner box
     LcdUpdate();
}

void TSpinboxCallback(void * obj,struct xEvent * e)
{
  
     struct TControl * o;
     mSb = (struct TSpinbox *) obj;
     o   = (struct TControl *) obj;
     
     x = o->parent->p.x + o->x;
     y = o->parent->p.y + o->y;
     w = o->width;
     h = o->height;
     
     e->message = 0;
     if(e->y >= y && e->y <= y+8)                           // SPIN UP
     {
        if(mSb->value < (mSb->max))
           e->message = SPIN_UP;
     }
     if(e->y >= (y+h)-9 && e->y <= (y+h))                  // DOWN
     {
       if(mSb->value > (mSb->min)) 
           e->message = SPIN_DOWN;
     }
	 TRedrawSpinbox(mSb,e);
	 LcdUpdate();
}

void TKeypadCallback(void * obj,struct xEvent * e)
{
    struct TForm * frm;
    struct TControl * o;
	 int row,col;			 //  August 21, 2013  12:59:59  Nithin.p
    o   = (struct TControl *) obj;
    frm = o->parent;
   

    if(240 - (kb1.tx->p.y +kb1.tx->p.height) > kb1.p.height)  // below control
    {
      kb1.p.y = kb1.tx->p.y + kb1.tx->p.height+1;     
    }
    else                                                      //above control
    {
      kb1.p.y = kb1.tx->p.y - kb1.p.height-1;
    }
    
    if(320 > (kb1.tx->p.x + kb1.p.width) )
    {
       kb1.p.x = kb1.tx->p.x;                
    }
    else
    {
       kb1.p.x = kb1.tx->p.x - ((kb1.tx->p.x+kb1.p.width)- 316);
    }
    

        
    x = o->parent->p.x + o->x;
    y = o->parent->p.y + o->y;
    w = o->width;
    h = o->height;
    
    if(kb1.state == 0)
    {
        kb1.state = 1;
        restore1 = (struct TControl *) frm->p.next;
        frm->p.next = (struct TControl *)&kb1;
        
    }
    else if(kb1.state == 1)
    {
         col = (e->x-x-4)/16;
         row = (e->y-(y+4))/10;
         
         ke.source =  2;
         ke.message = keymap[row][col];
         ke.x = 0;
         ke.y = 0;
         if(ke.message == '=')
         {
           kb1.state =0;
           frm->p.next = restore1;
         }
         else
         {
           TTextboxCallback((void *)kb1.tx , &ke);   //dispatch event to textbox
         }
    }
    TRedrawKeypad(&kb1);
    if(kb1.state == 0) TRedrawForm(&active_form);
    
}
char temp[] = {' ',' '};
void TTextboxCallback(void * obj,struct xEvent * e)
{
    struct TTextbox * etx = (struct TTextbox *) obj; 
    kb1.tx = etx;
    if(kb1.state == 0)
    {
      TKeypadCallback(&kb1,e);
    }
    if(e->source == 2)   // keypad event
    {
       
      switch(e->message)
      {
         case '>':
              kb1.tx->offset++;
          break;
          case '<':  
              kb1.tx->text[kb1.tx->offset] = 0;  
              if(kb1.tx->offset>0)  kb1.tx->offset--;
           break;
          default:
               if(kb1.tx->offset < kb1.tx->max)
               {
                   kb1.tx->text[kb1.tx->offset] = e->message;
                   kb1.tx->text[kb1.tx->offset+1] = 0;   // add null
                   kb1.tx->offset++;
               }
               
            break;
      }
      TRedrawTextbox(kb1.tx);


    }
}

void TComboboxCallback(void * obj,struct xEvent * e)
{
   char i,n; 
   struct TForm * frm;
   struct TControl * o;
   mCb = (struct TCombobox *) obj;
   o   = (struct TControl *) obj;
   frm = o->parent;
   
   x = o->parent->p.x + o->x;
   y = o->parent->p.y + o->y;
   w = o->width;
   h = o->height;

   offsetx = x+2;
   offsety = y+ ((h-8)>>1);   
   
  if(mCb->state == 0)
   {
       LcdBox(x,y+h+1,w,16*mCb->count,0);  
       LcdRect(x,y+h+1,w,16*mCb->count,1);  
       mCb->state = 1;
       restore1 = (struct TControl *) frm->p.next;
       restore2 = (struct TControl *) mCb->p.next;
       mCb->p.next = 0;                           // set next to null
       mCb->p.height += (16*mCb->count)+1;
       frm->p.next = (struct TControl *) mCb;     // set active control

         
       n = 0;
       i =0;
       while(1)
       {
          LcdPutStr3(&mCb->list[n],offsetx,offsety+17+(i*16),1);
          if(strlen(&mCb->list[n]) == 0) break;
          n += strlen(&mCb->list[n])+2;
          i++;
       }
       
       
   }
   else if(mCb->state == 1)
   {

       
       frm->p.next = restore1;                   // restore FORM  next object pointer 
       mCb->p.next = restore2;                   // restore combobox  next object pointer 
       mCb->p.height -=(16*mCb->count)+1;
       mCb->index = (e->y - (y+mCb->p.height))>>4;
       
       LcdBox(x+1,(y+mCb->p.height)+(mCb->index*16)+4,w-2,12,2);    
       LcdUpdate();
       LcdBox(x+1,(y+mCb->p.height)+(mCb->index*16)+4,w-2,12,2);    
       LcdBox(x,y+h-(16*mCb->count),w,(16*mCb->count)+1,0);    
       
       TRedrawForm(o->parent);
       mCb->state = 0;
   }

}
void TCallbackListbox(void * obj,struct xEvent * e)
{
  
}

void TCallbackCheckbox(void * obj,struct xEvent * e)
{
  //- char i,n; 
  //- struct TForm * frm;
   struct TControl * o;
   mCk = (struct TCheckbox *) obj;
   o   = (struct TControl *) obj;
  //- frm = o->parent;
   
   x = o->parent->p.x + o->x;
   y = o->parent->p.y + o->y;
   w = o->width;
   h = o->height;

   //--offsetx = x+2;
   //--offsety = y+ ((h-8)>>1);   
   
  
  if(mCk->state == 0 )
  {
      LcdBox(x+2,y+2,w-4,h-3,0);
      mCk->state =1;
  } 
  else if(mCk->state == 1)
  {
      LcdBox(x+2,y+2,w-4,h-3,1);
      mCk->state = 0;
  }
  
  
}

void xEventDispatch(struct xEvent * e)
{
      int eventx,eventy;
	  unsigned char Status=1;
	 
      o = (struct TControl *) active_form;
      o =  o->next;
      while((o)&&(Status))
      {
        eventx 			= 	o->x+o->parent->p.x	;
        eventy 			= 	o->y+o->parent->p.y	;
        
        if(e->x >= eventx   && e->x <= eventx+o->width)
         {
           if(e->y >= eventy  && e->y <= eventy+o->height)
           {
               switch(o->type)              //call default system callback function
               {
                   case TYPE_FORM:
                     break;
                   case TYPE_BUTTON:
                     if(!(e->flag & FLAG_MDOWN))
                     {
                       TButtonCallback(o);
                     }
                     break;
                   case TYPE_TEXTBOX:
                     if((e->flag & FLAG_MDOWN))
                     TTextboxCallback(o,e);
                     break;
                   case TYPE_SPINBOX:
                     if(!(e->flag & FLAG_MDOWN))
                     TSpinboxCallback(o,e);
                     break;
                   case TYPE_COMBOBOX:
                      if(!(e->flag & FLAG_MDOWN))
                      {
                         TComboboxCallback(o,e);
                      }
                     break;
               case   TYPE_LISTBOX:
                      if(!(e->flag & FLAG_MDOWN))
                      {
                         TCallbackListbox(o,e);
                      }
                      break;
               case   TYPE_CHECKBOX:
                      if(!(e->flag & FLAG_MDOWN))
                      {
                        TCallbackCheckbox(o,e);
                      }
                      break;
                   case TYPE_KEYPAD:
                       if(kb1.state == 1)
                       TKeypadCallback(o,e);
                     break;
               }
			Status=0;
			eFrm1.flag 		=	1;
			eFrm1.message 	=	o->id;	

               //TCursorUpdate(o);
               //o->callback(e,o);             //call user callback function
           }
         }
		 if(o->id==LastId){
			 Status=0;}
         o = o->next; 


      }

}
