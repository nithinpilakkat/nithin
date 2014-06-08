//#include <intrinsics.h>
#include "iodefine_rpdl.h"//#include "iorx62n.h"
#include "i2c.h"
#include "BSP.h"

//**************************************************************
//*******  variables , functions and defines fro I2C(RTC) ******
//**************************************************************
#define iic_sda_d	PORT1.DDR.BIT.B3	// SDA Direction
#define iic_sda		PORT1.DR.BIT.B3	// SDA
#define iic_scl_d	PORT1.DDR.BIT.B2	// SCL Direction
#define iic_scl		PORT1.DR.BIT.B2	// SCL

uchar RTCBuff[8];		// Current data received from RTC
static _RTC PCF8563;
_RTC *PtrPCF8563 = &PCF8563;

// I2C Variables for PCF8563 RTC communication
IicPack IicData_w;
IicPack IicData_r;


//************************************************************************************
// Name			: initIicBus
// Parameters	: None
// Returns		: None
// Description	: initialize I2C-BUS port
//************************************************************************************
void initIicBus(void)
{
	iic_sda_d = 0;			// SDA input ("H" state) 
	iic_scl_d = 0;			// SCL input ("H" state) 
}

//************************************************************************************
// Name			: IicBusRead
// Parameters	: structure IicPack pointer
// Returns		: Acknowledge
// Description	: Sequential Ramdom Read Cycle (I2C-BUS)
//************************************************************************************
unsigned char IicBusRead(IicPack *IicData)
{
	unsigned char i,ret;
	// Ramdom Read Cycle / Sequential Ramdom Read Cycle 
	IicData->iic_DeviceAddress &= 0xFE;						// WRITE Setting Device Address 
	StartCondition();								// Start Condition 
	while (1) 
	{
		if ((ret=ByteWrite(IicData->iic_DeviceAddress)) == NOACK)	// WRITE Device Address 
			break;													// NoAck Detect 
		if ((ret=ByteWrite(IicData->iic_MemoryAddress)) == NOACK)	// WRITE Memory Address 
			break;													// NoAck Detect 
		IicData->iic_DeviceAddress |= 0x01;				// READ Setting Device Address
		StartCondition();						// ReStart Condition 
		if ((ret=ByteWrite(IicData->iic_DeviceAddress)) == NOACK)	// WRITE Device Address 
			break;													// NoAck Detect 
		for (i=1; i<IicData->iic_NumberOfByte; i++) 				// specified bytes as loop 
		{
			ByteRead(IicData->iic_Data, ACK);						// Read data (Ack output) 
			IicData->iic_Data++;									//  
		}
		ByteRead(IicData->iic_Data, NOACK);							// Read data (NoAck output) 
		break;
	}
	StopCondition();												// Stop Condition 
	return(ret);
}

//************************************************************************************
// Name			: IicBusWrite
// Parameters	: structure IicPack pointer
// Returns		: Acknowledge
// Description	: Byte Write or Page Write Cycle (I2C-BUS)
//************************************************************************************
unsigned char IicBusWrite(IicPack *IicData)
{
	unsigned char i,ret;											// Byte Write / Page Write 
	IicData->iic_DeviceAddress &= 0xFE;								// WRITE Setting Device Address 
	StartCondition();												// Start Condition 
	while (1) 
	{
		if ((ret=ByteWrite(IicData->iic_DeviceAddress)) == NOACK)	// Write Device Address 
			break;													// NoAck Detect 
		if ((ret=ByteWrite(IicData->iic_MemoryAddress)) == NOACK)	// Write Memory Addreess 
			break;													// NoAck Detect 
		for (i=0; i<IicData->iic_NumberOfByte; i++) 				// specified bytes as loop 
		{		
			if ((ret=ByteWrite(*(IicData->iic_Data))) == NOACK)		// Write Data 
				break;												// NoAck Detect 
			IicData->iic_Data++;									//  
		}
		break;
	}
	StopCondition();												// Stop Condition 
	return(ret);
}

///************************************************************************************
// Name			: StartCondition
// Parameters	: None
// Returns		: None
// Description	: Output Start Condition (I2C-BUS)
// Note			: *1 adjust a wait time
//*************************************************************************************
void StartCondition(void)
{
	iic_scl = 0;				// SCL="L" 
	iic_scl_d = 1;				// SCL output 
	_WaitTime1us();				// wait *1 
	iic_sda_d = 0;				// SDA="H" 
	_WaitTime1us();				// wait 
	_WaitTime1us();				// wait *! 
	iic_scl = 1;				// SCL="H" 
	_Wait_tSU_STA;				// wait 
	iic_sda = 0;				// SDA="L" 
	iic_sda_d = 1;				// SDA output 
	_Wait_tHD_STA;				// wait 
	_WaitTime1us();				// wait *1 
	iic_scl = 0;				// SCL="L" 
}

//************************************************************************************
// Name			: StopCondition
// Parameters	: None
// Returns		: None
// Description	: Output Stop Condition (I2C-BUS)
// Note			: *1 adjust a wait time
//************************************************************************************
void StopCondition(void)
{
	iic_scl = 0;				// SCL="L" 
	iic_scl_d = 1;				// SCL output 
	_WaitTime1us();				// wait *1 
	iic_sda = 0;				// SDA="L" 
	iic_sda_d = 1;				// SDA output 
	_WaitTime1us();				// wait *1 
	iic_scl = 1;				// SCL="H" 
	_Wait_tSU_STO;				// wait 
	iic_sda_d = 0;				// SDA="H" 
	_WaitTime1us();				// wait 
	_WaitTime1us();				// wait *1 
	iic_scl = 0;				// SCL="L" 
}

//************************************************************************************
// Name			: ByteWrite
// Parameters	: Write data
// Returns		: Acknowledge
// Description	: byte data Output (I2C-BUS)
// Note			: *1 adjust a wait time
//************************************************************************************
unsigned char ByteWrite(unsigned char iic_writeData)
{
	unsigned char maskData=0x80;			// MSB first 
	unsigned char ret=ACK;					// Ack/NoAck 

	while (maskData) 						// 8times as loop 
	{						
		iic_sda = 0;						// initialize port-latch 
		if (iic_writeData & maskData) 		// "H" output ? 
		{
			iic_sda_d = 0;					// Yes SDA="H" 
		}
		else
		{
			iic_sda_d = 1;					// No  SDA="L" 
			//asm("nop");						// wait *1 
			//asm("nop");						// wait *1 
			//asm("nop");						// wait *1 
                        __delay_cycles(bit_delay);
		}
		_Wait_tSU_DAT;						// wait 
		_WaitTime1us();						// wait *1 
		iic_scl = 1;						// SCL="H" 
		_Wait_tHIGH;						// wait 
		_WaitTime1us();						// wait *1 
		iic_scl = 0;						// SCL="L" 
		maskData >>= 1;						// change mask data 
		_WaitTime1us();						// wait *1 
	}
	iic_sda_d = 0;							// SDA input 
	_Wait_tAA;								// wait 
	_WaitTime2us();							// wait *1 
	iic_scl = 1;							// SCL="H" 
	if (iic_sda) ret=NOACK;					// NoAck Detect 
	_Wait_tHIGH;							// wait 
	_WaitTime1us();							// wait *1 
	iic_scl = 0;							// SCL="L" 
	_Wait_tHD_DAT;							// wait 
	return(ret);
}

//************************************************************************************
// Name			: ByteRead
// Parameters	: Read data strage location pointer, Select Ack/NoAck
// Returns		: None
// Description	: byte data input with Ack output (I2C-BUS)
// Note			: *1 adjust a wait time
//************************************************************************************
void ByteRead(unsigned char *iic_readData, unsigned char ackData)
{
	unsigned char maskData=0x80;				// MSB first 
	unsigned char readData;
	*iic_readData = 0;							//  
	while (maskData) 							// 8times as loop 
	{						
		readData = *iic_readData | maskData;	//
		iic_sda_d = 0;							// initialize port-latch 
		_Wait_tAA;								// wait 
		iic_scl = 1;							// SCL="H" 
		
               
                if (PORT1.PORT.BIT.B3)
                //if (iic_sda) 							// SDA="H" ? 
		{						
			*iic_readData = readData;			// Yes  
		}else
		{
                __delay_cycles(8);
		}
                __delay_cycles(bit_delay);
               
		_Wait_tHIGH;						// wait 
		_WaitTime1us();						// wait *1 
		iic_scl = 0;						// SCL="L" 
		maskData >>= 1;						// Change mask data 
		_WaitTime1us();						// wait *1 
	}
	if (!ackData) 							// Ack output ? 
	{										// Ack output 
		iic_sda = ACK;						// Yes SDA="L" 
		iic_sda_d = 1;						// SDA output 
	}else
	{										// NoAck output 
		iic_sda = NOACK;					// No  SDA="H" 
		iic_sda_d = 0;						// SDA input 
	}
	_Wait_tSU_DAT;							// wait 
	_WaitTime1us();							// wait *1 
	iic_scl = 1;							// SCL="H" 
	_Wait_tHIGH;							// wait 
	_WaitTime1us();							// wait *1 
	iic_scl = 0;							// SCL="L" 
	iic_sda_d = 0;							// SDA input 
	_WaitTime1us();							// wait *1 
}

//************************************************************************************
// Name			: _WaitTime0us
// Parameters	: None
// Returns		: None
// Description	: a 0us wait
//************************************************************************************
void _WaitTime0us(void)
{
}

//************************************************************************************
// Name			: _WaitTime1us
// Parameters	: None
// Returns		: None
// Description	: a 1us wait
//************************************************************************************
void _WaitTime1us(void)
{
        __delay_cycles(wait_i2c);
}

//************************************************************************************
// Name			: _WaitTime2us
// Parameters	: None
// Returns		: None
// Description	: a 2us wait
//************************************************************************************
void _WaitTime2us(void)
{
  __delay_cycles(wait_i2c);
  
}

//*******************************************************************************
// Function	 	: Read_RTC()
// Input(s)	 	: None
// Output(s) 	: None
// Description	: Read the data from RTC - PCF8563
//*******************************************************************************
uchar * Read_RTC()
{
	//IicData_r.iic_DeviceAddress = 0xDE;		// RTC Read address
        IicData_r.iic_DeviceAddress = 0xA3;		// RTC Read address
	IicData_r.iic_MemoryAddress = 0x02;		// RTC memory address from where read done
	IicData_r.iic_Data = RTCBuff;			// Buffer to store the data
	IicData_r.iic_NumberOfByte = 7;			// No. of bytes to be read
	if(IicBusRead(&IicData_r) == ACK) {}	
    __delay_cycles(150);

	RTCBuff[0] &=0x7f;     //sec  |1|1|1|1| |1|1|1|1|
	
	RTCBuff[1] &=0x7f;    //min	  tens 	     units
	RTCBuff[2] &=0x03f;    //hour 
	RTCBuff[3]			= RTCBuff[3]&0x3f; // DAYS
	RTCBuff[4]			= RTCBuff[4]&0x0f; // week day
	RTCBuff[5]			= RTCBuff[5]&0x1f;  // month
	RTCBuff[6]			= RTCBuff[6]&0x3f;  // 19
	PtrPCF8563->SEC		=bcdToDec(RTCBuff[0]);
	PtrPCF8563->MIN		=bcdToDec(RTCBuff[1]);
	PtrPCF8563->HOUR	=bcdToDec(RTCBuff[2]);
	PtrPCF8563->DAYS	=bcdToDec(RTCBuff[3]);
	PtrPCF8563->WEEKDAYS=bcdToDec(RTCBuff[4]);
	PtrPCF8563->MONTH	=bcdToDec(RTCBuff[5]);
	PtrPCF8563->YEAR	=bcdToDec(RTCBuff[6]);
	
   return RTCBuff;
}




//*******************************************************************************
// Function	 	: Write_RTC()
// Input(s)	 	: None
// Output(s) 	: None
// Description	: To set RTC - Will be used for RTC set function
//*******************************************************************************
void Write_RTC()
{
	IicData_w.iic_DeviceAddress = 0xA3;		// RTC write address
	IicData_w.iic_MemoryAddress = 0x02;		// 
	IicData_w.iic_Data = RTCBuff;			//
	IicData_w.iic_NumberOfByte = 7;			//
	if (IicBusWrite(&IicData_w) == ACK) {}

}

void Start_RTC()
{
	
        RTCBuff[0]=0x10;
        RTCBuff[1]=0x00;
        RTCBuff[2]=0x00;
        RTCBuff[3]=0x00;
        IicData_w.iic_DeviceAddress = 0xA3;		// RTC write address
	IicData_w.iic_MemoryAddress = 0x00;		// 
	IicData_w.iic_Data = RTCBuff;			//
	IicData_w.iic_NumberOfByte = 4;			//
	if (IicBusWrite(&IicData_w) == ACK) {}
        __delay_cycles(5000);

}

void Write_Byte(unsigned char address,unsigned char data)
{
	IicData_w.iic_DeviceAddress = 0xA3;		// RTC write address
	IicData_w.iic_MemoryAddress = address;		// 
	IicData_w.iic_Data = &data;			//
	IicData_w.iic_NumberOfByte = 1;			//
	if (IicBusWrite(&IicData_w) == ACK) {}
    //Delay(200);	
	__delay_cycles(5000);
}

void Write_Erom(unsigned char address,unsigned char *data,unsigned char size)
{
	 unsigned char i;
	 for(i=0;i<size;i++)
	 {
		 Write_Byte(address+i,data[i]);
	 }
}

unsigned char Read_Byte(unsigned char address)
{
	unsigned char data;
	IicData_r.iic_DeviceAddress = 0xA3;		// RTC Read address
	IicData_r.iic_MemoryAddress = address;		// RTC memory address from where read done
	IicData_r.iic_Data = &data;			// Buffer to store the data
	IicData_r.iic_NumberOfByte = 1;			// No. of bytes to be read
	if(IicBusRead(&IicData_r) == ACK) {}	
   // Delay(100);
   __delay_cycles(5000);
	return data;
}

void Read_Erom(unsigned char address,unsigned char *data,unsigned char size)
{
	 unsigned char i;
	 for(i=0;i<size;i++)
	 {
		 data[i]= Read_Byte(address+i);
	 }
}

//*******************************************************************************
// Function	 	: sedntoRTC()
// Input(s)	 	: None
// Output(s) 	: None
// Description	: To set the RTC timing
//*******************************************************************************
void sendtoRTC(uchar SEC,
			   uchar MIN,
			   uchar HOUR,
			   uchar DAYS,
			   uchar WEEKDAYS,
			   uchar MONTH,
			   uchar YEAR) 
{

        initIicBus();
        Start_RTC();

	/*
	RTCBuff[0]= 0x30; // SEC 
	RTCBuff[1]= 0x10; // MINUTE
	RTCBuff[2]= 0x18; // HOUR   
	RTCBuff[3]= 0x20; // DAYS
	RTCBuff[4]= 0x03; // week day
	RTCBuff[5]= 0x05; // month
    RTCBuff[6]= 0x14; // year
    */
	
	RTCBuff[0]= decToBcd(SEC); // SEC
	RTCBuff[1]= decToBcd(MIN); // MINUTE
	RTCBuff[2]= decToBcd(HOUR); // HOUR
	RTCBuff[3]= decToBcd(DAYS); // DAYS
	RTCBuff[4]= decToBcd(WEEKDAYS); // week day
	RTCBuff[5]= decToBcd(MONTH); // month
    RTCBuff[6]= decToBcd(YEAR); // year    


    Write_RTC();
	
//	Read_RTC();
//    Read_RTC();
}
/*
uchar BCDToDCM(uchar temp)
{uchar i,j;
	i=(temp>>4)*10;
	j=temp&0x0f;
	return(i+j);
}



uchar DCMToBCD(uchar temp)
{
uchar i,j;
	i=(temp>>4)*10;
	j=temp&0x0f;
	return(i+j);	
}
*/
uchar decToBcd(uchar val)
{
return ( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
uchar bcdToDec(uchar val)
{
return ( (val/16*10) + (val%16) );
}