// vim:et:ts=4:sw=4:
/*
 * Copyright (C) 2013 Y-COM, Inc.
 *
 * Initial Code:
 * 	Gump Yang
 *
 */

#include <linux/module.h>
#include <mach/gpio.h>

#include "cw_low.h"
#include "cw_encry.h"

#define SCL_PIN RK30_PIN3_PA7
#define SDA_PIN RK30_PIN3_PB6

#define SCL_H  gpio_set_value(SCL_PIN,GPIO_HIGH)
#define SCL_L  gpio_set_value(SCL_PIN,GPIO_LOW)
#define SDA_H  gpio_set_value(SDA_PIN,GPIO_HIGH)
#define SDA_L  gpio_set_value(SDA_PIN,GPIO_LOW)
#define CM_SCL_OUT gpio_direction_output(SCL_PIN,GPIO_HIGH)
#define SDA_read  gpio_get_value(SDA_PIN)
#define CM_DATA_IN gpio_direction_input(SDA_PIN)
#define CM_DATA_OUT gpio_direction_output(SDA_PIN,GPIO_HIGH)

void cw_gpio_init(void)
{
    int ret = 0;
    
    ret = gpio_request(SCL_PIN, "cw2048b");
    if (ret < 0) {
        gpio_free(SCL_PIN);
        gpio_request(SCL_PIN, "cw2048b");
    }
    CM_SCL_OUT;
    
    ret = gpio_request(SDA_PIN, "cw2048b");
    if (ret < 0) {
        gpio_free(SDA_PIN);
        gpio_request(SDA_PIN, "cw2048b");
    }
	gpio_pull_updown(SDA_PIN, PullDisable);
    CM_DATA_OUT;

    CWER = 1;
}

// 1/2 Clock Cycle transition to HIGH
//
static inline void cm_Clockhigh(void)
{
    cm_Delay(1);
    SCL_H;
    cm_Delay(1);
}

// 1/2 Clock Cycle transition to LOW
//
static inline void cm_Clocklow(void)
{
    cm_Delay(1);
    SCL_L;
    cm_Delay(1);
}

// Do one full clock cycle
//
// Changed 1/19/05 to eliminate one level of return stack requirements
//
static inline void cm_ClockCycle(void)
{
    cm_Delay(1);
    SCL_L;
    cm_Delay(2);
    SCL_H;
    cm_Delay(1);
}

//     Read a byte from device, MSB
//
// Modified 7-21-04 to correctly control SDA
// 
static uchar cm_Read(void)
{
	uchar i;
	uchar rByte = 0;
	
	SDA_H;
	CM_DATA_IN;                          // Set data line to be an input
	for(i=0x80; i; i=i>>1)
	{
		cm_ClockCycle();
		if (SDA_read) rByte |= i;
		cm_Clocklow();
	}
	CM_DATA_OUT;                         // Set data line to be an output
	return rByte;
}

// Send a ACK or NAK or to the device
static inline void cm_AckNak(uchar ucAck)
{
	CM_DATA_IN; // CM_DATA_OUT;                         // Data line must be an output to send an ACK
	cm_Clocklow();
	if (ucAck) SDA_L;               // Low on data line indicates an ACK
	else       SDA_H;               // High on data line indicates an NACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
}

// Send a start sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
static void cm_Start(void)
{
    CM_DATA_OUT;                         // Data line must be an output to send a start sequence
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

// Write a byte
//
// Returns 0 if write successed, 1 if write fails failure
//
// Modified 7-21-04 to correctly control SDA
// 
uchar cm_Write(uchar ucData)
{
    uchar i;

    CM_DATA_OUT;                         // Set data line to be an output
    for(i=0; i<8; i++) {                 // Send 8 bits of data
        cm_Clocklow();
        if (ucData&0x80) SDA_H;
        else             SDA_L;
        cm_Clockhigh();
        ucData = ucData<<1;
    }
    cm_Clocklow();

    // wait for the ack
    SDA_H;
    CM_DATA_IN;                      // Set data line to be an input
    cm_Delay(8);
    cm_Clockhigh();
    while(i>1) {                    // loop waiting for ack (loop above left i == 8)
        cm_Delay(2);
        if (SDA_read) i--;        // if SDA is high level decrement retry counter
        else i = 0;
    }      
    cm_Clocklow();
    CM_DATA_OUT;                     // Set data line to be an output
    return 0;
}

// Do a number of clock cycles
//
void cm_ClockCycles(uchar ucCount)
{
    uchar i;
    
    for (i = 0; i < ucCount; ++i) cm_ClockCycle();

//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

// Send a stop sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
void cm_Stop(void)
{
	CM_DATA_OUT;                         // Data line must be an output to send a stop sequence
	cm_Clocklow();
	SDA_L;
	cm_Delay(4);
	cm_Clockhigh();
	cm_Delay(8);
	SDA_H;
	cm_Delay(4);
//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
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
//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

// Send a command
//
uchar cm_SendCommand(puchar pucInsBuff)
{
    uchar i, ucCmd;
    uchar cw_CmdBuff[4];

//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
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

//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
    for(i = 0; i < (ucLen-1); i++) {
        pucRecBuf[i] = cm_Read();
        cm_AckNak(TRUE);
    }
    if(pucRecBuf!=NULL) pucRecBuf[i] = cm_Read();
    cm_AckNak(FALSE);
    cm_Stop();

    if(CWER) {
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
	
//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
    return SUCCESS;
}
#if 0
void cm_Delay(uchar ucDelay)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}
#endif
void cm_RandGen(puchar pucRandomData)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

uchar cm_SendChecksum(puchar pucChkSum)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
    return SUCCESS;
}

uchar cm_SendCmdByte(uchar ucCommand)
{
    uchar i, ucCmd;
    uchar cw_CmdBuff;

    i = CM_START_TRIES;
    ucCmd = (ucCommand&0x0F)|CM_PORT_CFG.ucChipSelect;
    if(CWER)
    {
        cw_Encry(&ucCmd,&cw_CmdBuff,1);
        ucCmd=cw_CmdBuff;
    }
    while (i) {
        cm_Start();
        if (cm_Write(ucCmd) == 0) break;
        if (--i == 0) return FAIL_CMDSTART;
    }
//    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
    return SUCCESS;
}

// Power On Chip  
//
// Returns 0 (SUCCESS) if no error
//
void cm_PowerOn(void)   
{
	// Reset chip data
    ucCM_UserZone = ucCM_AntiTearing = 0;
    
    // Sequence for powering on secure memory according to ATMEL spec
    SCL_L;                                // Clock should start LOW
    SDA_H;                               // Data high during reset
    cm_ClockCycles(CM_PWRON_CLKS);            // Give chip some clocks cycles to get started

    // Chip should now be in sync mode and ready to operate
}

// Shut down secure memory
//
void cm_PowerOff(void)
{
	cm_Delay(1);
	SCL_L;
	cm_Delay(6);
}
void cm_GPAGen(uchar ucZoneNumber)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

void cm_GPAcmd2(uchar *ucZoneNumber)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

void cm_GPAencrypt(uchar a, uchar *b, uchar c)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

void cm_GPAdecrypt(uchar a, uchar *b, uchar c)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

void cm_GPAencrypt_WLM(uchar a, uchar *b, uchar c)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}
