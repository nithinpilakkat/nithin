#include <string.h>
#include "lcddriver.h"
#include "xWidget.h"
#include "framebuffer.h"

void LoadForm1();
struct TForm form1;
struct TButton bt1;
struct TButton bt2;
struct TButton bt3;
struct TTextbox tx1;
struct TTextbox tx2;
struct TTextbox tx3;
struct TSpinbox sb1;
struct TSpinbox sb2;
struct TSpinbox sb3;
struct TCombobox cb1;
struct TCombobox cb2;
struct TCombobox cb3;
struct TListbox  lb1;
struct TCheckbox  ck1;
struct TCheckbox  ck2;
extern struct TKeypad   kb1;




char tx1_value[20];
char tx2_value[20];
char tx3_value[20];

const char  LIST1[] = "COM1\0 COM2\0 COM3\0 COM4\0 COM5\0 ";
const char  LIST2[] = "9600\0 19200\0 38400\0 115200\0 ";
const char  VOLT[] = "100\0 500\0 1000\0 1500\0 2000\0 2500\0 ";


extern void * active_form;

void  Button2Click(struct xEvent * e,void * obj)
{
  //strcpy(tx1.text,"BUTTON2");
  //TRedrawTextbox(&tx1);
}

void  Button1Click(struct xEvent * e,void * obj)
{
  //strcpy (tx1.text,"BUTTON1");
  //TRedrawTextbox(&tx1);
}

void  DefaultCallback(struct xEvent * e,void * obj)
{
  
}

void  SipCallback(struct xEvent * e,void * obj)
{

}
void  TextboxCallback(struct xEvent * e,void * obj)
{

}

void Spinbox1Callback(struct xEvent *e,void * obj)
{
   struct TSpinbox * sb = (struct TSpinbox * )obj;
   if(e->message == SPIN_UP)
   {
       sb->value++;
   }
   if(e->message == SPIN_DOWN)
   {
       sb->value--;
   }
   TRedrawSpinbox(sb);
}
void Combobox1Callback(struct xEvent *e,void * obj)
{
     
}
void KeypadCallback(struct xEvent *e,void * obj)
{

}

void LoadForm1()
{
    active_form = &form1;
    
    form1.p.x = 0;
    form1.p.y = 0;
    form1.p.width = 319;
    form1.p.height = 239;
    
    TCreateForm(1 ,(void *)&form1,"FORM1",&bt1);
    
    bt1.p.x = 140;
    bt1.p.y = 90;
    bt1.p.width = 80;
    bt1.p.height = 24;
    bt1.p.callback = &Button1Click;
    TCreateButton(&form1 ,(void *)&bt1,"NEXT",(void*)&bt2);


    bt2.p.x = 50;
    bt2.p.y = 90;
    bt2.p.width = 80;
    bt2.p.height = 24;
    bt2.p.callback = &Button2Click;
    TCreateButton(&form1 ,(void *)&bt2,"FINISH",&tx1);

    tx1.max = tx2.max = tx3.max = 8;
    
    tx1.p.x = 10;
    tx1.p.y = 60;
    tx1.p.width = 100;
    tx1.p.height = 14;
    tx1.p.callback = &TextboxCallback;
    TCreateTextbox(&form1 ,(void *)&tx1,tx1_value,&tx2);
    

    tx2.p.x = 140;
    tx2.p.y = 60;
    tx2.p.width = 80;
    tx2.p.height = 14;
    tx2.p.callback = &TextboxCallback;
    TCreateTextbox(&form1 ,(void *)&tx2,tx2_value,&tx3);
    

    tx3.p.x = 120;
    tx3.p.y = 200;
    tx3.p.width = 90;
    tx3.p.height = 14;
    tx3.p.callback = &TextboxCallback;
    TCreateTextbox(&form1 ,(void *)&tx3,tx3_value,&sb1);

    sb1.p.x = 10;
    sb1.p.y = 170;
    sb1.p.width = 19;
    sb1.p.height = 32;
    sb1.min = 1;
    sb1.max = 31;
    sb1.p.callback = &Spinbox1Callback;
    TCreateSpinbox(&form1 ,(void *)&sb1,1,&sb2);    


    sb2.p.x = 32;
    sb2.p.y = 170;
    sb2.p.width = 19;
    sb2.p.height = 32;
    sb2.min = 1;
    sb2.max = 12;
    sb2.p.callback = &Spinbox1Callback;
    TCreateSpinbox(&form1 ,(void *)&sb2,5,&sb3);    

    sb3.p.x = 54;
    sb3.p.y = 170;
    sb3.p.width = 35;
    sb3.p.height = 32;
    sb3.min = 2010;
    sb3.max = 2050;
    sb3.p.callback = &Spinbox1Callback;
    TCreateSpinbox(&form1 ,(void *)&sb3,2012,&cb1);    
    
    cb1.p.x = 50;
    cb1.p.y = 125;
    cb1.p.width = 60;
    cb1.p.height = 14;
    cb1.count = 5;
    cb1.p.callback = &Combobox1Callback;
    TCreateCombobox(&form1 ,(void *)&cb1,(char *)LIST1,&cb2);
    
    cb2.p.x = 150;
    cb2.p.y = 125;
    cb2.p.width = 60;
    cb2.p.height = 14;
    cb2.count = 4;
    cb2.p.callback = &Combobox1Callback;
    TCreateCombobox(&form1 ,(void *)&cb2,(char *)LIST2,&lb1);
   
    lb1.p.x = 225;
    lb1.p.y = 50;
    lb1.p.width = 86;
    lb1.p.height = 150;
    lb1.count = 4;
    lb1.p.callback = &DefaultCallback;
    TCreateListbox(&form1 ,(void *)&lb1,(char *)0,&ck1);
    
    ck1.p.x = 150;
    ck1.p.y = 20;
    ck1.p.width = 10;
    ck1.p.height = 10;
    ck1.state = 0;
    ck1.p.callback = &DefaultCallback;
    TCreateCheckbox(&form1 ,(void *)&ck1,"FLAG1",&ck2);
      

    ck2.p.x = 150;
    ck2.p.y = 40;
    ck2.p.width = 10;
    ck2.p.height = 10;
    ck2.state = 0;
    ck2.p.callback = &DefaultCallback;
    TCreateCheckbox(&form1 ,(void *)&ck2,"FLAG2",&kb1);

    kb1.p.x = 100;
    kb1.p.y = 185;
    kb1.p.width = 170;
    kb1.p.height = 45;
    kb1.state = 0;
    kb1.p.callback = &KeypadCallback;
    TCreateKeypad(&form1 ,(void *)&kb1,0);
    /*
    LcdPutStr1("Potent Embedded Solutions",8,15);
    LcdPutStr1("TWidget",8,27);
    LcdPutStr2("GUI Widget For 8/16/32 bit - Micro.",8,40);
    */

}

