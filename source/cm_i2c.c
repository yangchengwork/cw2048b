// Functions that directly control the hardware

#include "CM_LIB.H"
#include "CM_I2C.H"
#include "CM_I2C_L.H"
#include "CW_Encry.h"
// 1/2 Clock Cycle transition to HIGH
//
void cm_Clockhigh(void)
{
    cm_Delay(1);
    SCL_H;
    cm_Delay(1);
}

// 1/2 Clock Cycle transition to LOW
//
void cm_Clocklow(void)
{
    cm_Delay(1);
    SCL_L;
    cm_Delay(1);
}

// Do one full clock cycle
//
// Changed 1/19/05 to eliminate one level of return stack requirements
//
void cm_ClockCycle(void)
{
    cm_Delay(1);
    SCL_L;
    cm_Delay(2);
    SCL_H;
    cm_Delay(1);
}

// Do a number of clock cycles
//
void cm_ClockCycles(uchar ucCount)
{
    uchar i;
    
    for (i = 0; i < ucCount; ++i) cm_ClockCycle();
}

// Send a start sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
void cm_Start(void)
{
	  //CM_DATA_OUT;                         // Data line must be an output to send a start sequence
	  cm_Clocklow();
	  SDA_H;
	  cm_Delay(4);
	  cm_Clockhigh();
	  cm_Delay(4);
	  SDA_L;
	  cm_Delay(8);
	  cm_Clocklow();
	  cm_Delay(8);
}

// Send a stop sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
void cm_Stop(void)
{
	//CM_DATA_OUT;                         // Data line must be an output to send a stop sequence
	cm_Clocklow();
	SDA_L;
	cm_Delay(4);
	cm_Clockhigh();
	cm_Delay(8);
	SDA_H;
	cm_Delay(4);
}


// Write a byte
//
// Returns 0 if write successed, 1 if write fails failure
//
// Modified 7-21-04 to correctly control SDA
// 
uchar cm_Write(uchar ucData)
{
    uchar i;

    //CM_DATA_OUT;                         // Set data line to be an output
    for(i=0; i<8; i++) {                 // Send 8 bits of data
        cm_Clocklow();
        if (ucData&0x80) SDA_H;
        else             SDA_L;
        cm_Clockhigh();
        ucData = ucData<<1;
    }
    cm_Clocklow();

    // wait for the ack
    //CM_DATA_IN;                      // Set data line to be an input
    SDA_H;
    cm_Delay(8);
    cm_Clockhigh();
    while(i>1) {                    // loop waiting for ack (loop above left i == 8)
        cm_Delay(2);
        if (SDA_read) i--;        // if SDA is high level decrement retry counter
        else i = 0;
    }      
    cm_Clocklow();
    //CM_DATA_OUT;                     // Set data line to be an output
    return 0;
}


// Send a ACK or NAK or to the device
void cm_AckNak(uchar ucAck)
{
	//CM_DATA_OUT;                         // Data line must be an output to send an ACK
	cm_Clocklow();
	if (ucAck) SDA_L;               // Low on data line indicates an ACK
	else       SDA_H;               // High on data line indicates an NACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
}

#ifdef PIGS_FLY

// ------------------------------------------------------------------------------------- 
// Original Version
// ------------------------------------------------------------------------------------- 

// Send a ACK to the device
void cm_Ack(void)
{
	//CM_DATA_OUT;                         // Data line must be an output to send an ACK
	cm_Clocklow();
	SDA_L;                          // Low on data line indicates an ACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
	//SET_SDA;
}

// Send a NACK to the device
void cm_N_Ack(void)
{
	//CM_DATA_OUT;                         // Data line must be an output to send an NACK
	cm_Clocklow();
	SDA_H;                          // High on data line indicates an NACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
	//SET_SDA;
}

// ------------------------------------------------------------------------------------- 
// Version that uses one less level of call stack
// ------------------------------------------------------------------------------------- 

// Send a ACK to the device
void cm_Ack(void)
{
    //CM_DATA_OUT;                         // Data line must be an output to send an ACK
    cm_Delay(1);
    SCL_L;
    cm_Delay(1);
    SDA_L;                          // Low on data line indicates an ACK
    cm_Delay(3);
    SCL_H;
    cm_Delay(9);
    cm_Clocklow();
}

// Send a NACK to the device
void cm_N_Ack(void)
{
    //CM_DATA_OUT;                         // Data line must be an output to send an NACK
    cm_Delay(1);
    SCL_L;
    cm_Delay(1);
    SDA_H;                          // High on data line indicates an NACK
    cm_Delay(2);
    SCL_H;
    cm_Delay(9);
    cm_Clocklow();
}
#endif

//     Read a byte from device, MSB
//
// Modified 7-21-04 to correctly control SDA
// 
uchar cm_Read(void)
{
	uchar i;
	uchar rByte = 0;
	
	//CM_DATA_IN;                          // Set data line to be an input
	SDA_H;
	for(i=0x80; i; i=i>>1)
	{
		cm_ClockCycle();
		if (SDA_read) rByte |= i;
		cm_Clocklow();
	}
	//CM_DATA_OUT;                         // Set data line to be an output
	return rByte;
}


void cm_WaitClock(uchar loop)
{
	uchar i, j;

	cm_Clocklow();		//add by joe
	cm_Delay(4);
	
	SDA_L;
	for(j=0; j<loop*5; j++) {
		//cm_Start();
		for(i = 0; i<200; i++) cm_ClockCycle();
		//cm_Stop();
	}
}


//************************AckPolling*******************************
//功能:检查所发送的命令是否已被执行
//只有当AckPolling_return=1或bytecounter=0时，该程序才退出
// added by Joe 2013-3-1 to replcace cm_WaitClock
//*****************************************************************
void Wait_For_AckPolling(void)
//void cm_WaitClock(uchar loop)
{
unsigned char  AckPolling_return;
unsigned char  temp, bitcounter;
unsigned char  bytecounter;

cm_Start();
temp=0xb6;
bytecounter=0xff;//如CPU速度过快须加大其值
AckPolling_return=0;

do{
  for(bitcounter=0;bitcounter<8;bitcounter++)
     {
     if ((temp&0x80)==0x80)
        {SDA_H;}
     else
        {SDA_L;}
      cm_Delay(1);
       SCL_H;cm_Delay(1);
       SCL_L;cm_Delay(1);
       temp=temp<<1;
     }

  SDA_H;cm_Delay(1);
  SCL_H;cm_Delay(1);
  bytecounter--;

  if(!SDA_read)
    {SCL_L;AckPolling_return=1;}
  else
    {SCL_L;AckPolling_return=0;cm_Start();temp=0xb6;}

  if(AckPolling_return==1)
    {break;}

  }while(bytecounter>0);

cm_Stop();
}

// Send a command
//
uchar cm_SendCommand(puchar pucInsBuff)
{
    uchar i, ucCmd;

	uchar cw_CmdBuff[4];
	
   i = CM_START_TRIES;
   ucCmd = (pucInsBuff[0]&0x0F)|CM_PORT_CFG.ucChipSelect;
   if(CWER)
   	{
   		pucInsBuff[0]=ucCmd;
		cw_Encry(pucInsBuff,cw_CmdBuff,4);
		for(i=0;i<4;i++) pucInsBuff[i]=cw_CmdBuff[i];
		ucCmd=pucInsBuff[0];
   	}
   ucCmd=pucInsBuff[0];
    while (i) {
        cm_Start();
        if (cm_Write(ucCmd) == 0) break;
        if (--i == 0) return FAIL_CMDSTART;
    }
	
    for(i = 1; i< 4; i++) {
        if (cm_Write(pucInsBuff[i]) != 0) return FAIL_CMDSEND;
    }
    return SUCCESS;
}

uchar cm_ReceiveData(puchar pucRecBuf, uchar ucLen)
{
    int i;
	uchar cw_DataBuff[256];
	
    for(i = 0; i < (ucLen-1); i++) {
        pucRecBuf[i] = cm_Read();
        cm_AckNak(TRUE);
    }
    if(pucRecBuf!=NULL) pucRecBuf[i] = cm_Read();
    cm_AckNak(FALSE);
    cm_Stop();

	if(CWER)
		{
		cw_Decrypt(pucRecBuf, cw_DataBuff, ucLen);
		for(i=0;i<ucLen;i++) pucRecBuf[i]=cw_DataBuff[i];	
		}
    return SUCCESS;
}

uchar cm_SendData(puchar pucSendBuf, uchar ucLen)
{
	int i;
	uchar cw_DataBuff[16];
	if(CWER)
   	{
		cw_Encry(pucSendBuf,cw_DataBuff,ucLen);
		//for(i=0;i<ucLen;i++) pucSendBuf[i]=cw_DataBuff[i];	
   	}
	
	for(i = 0; i< ucLen; i++) {
		if(CWER) 
		{
			if (cm_Write(cw_DataBuff[i])==1) return FAIL_WRDATA;
		}
		else 
		{
			if (cm_Write(pucSendBuf[i])==1) return FAIL_WRDATA;
		}
	}
	cm_Stop();
	
	return SUCCESS;
}
