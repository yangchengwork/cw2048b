// Protect Library against multiple inclusion
#ifndef CM_LOW_H
#define CM_LOW_H

#include "cm_lib.h"

// Constants used in low level functions
// Power on clocks (spec call for 5, but California Card uses 15)
#define CM_PWRON_CLKS (15)
#define SUCCESS		1

// Define control of the secure memory in terms of the pins defined in CM_PORT_CFG
#define CM_POWER_PIN (CM_PORT_CFG.ucPowerPin)
#define CM_POWER_POL (CM_PORT_CFG.ucPowerPolarity)
#define CM_TIMER      (CM_PORT_CFG.ucDelayCount)
#define CM_START_TRIES (CM_PORT_CFG.ucStartTries)

// Mid-Level Functions
uchar cm_ReadCommand(puchar pucInsBuff, puchar pucRetVal, uchar ucLen);
uchar cm_ReadCommand_rd(puchar pucInsBuff, puchar pucRetVal, uchar ucLen);
uchar cm_WriteCommand(puchar pucInsBuff, puchar pucSendVal, uchar ucLen);
uchar cm_WriteCommand_NW(puchar pucInsBuff, puchar pucSendVal, uchar ucLen);

// Functions in sw_low.c used internally by other low level functions
void cw_gpio_init(void);

void cm_ClockCycles(uchar ucCount);
void cm_Stop(void);
void cm_WaitClock(uchar loop);
uchar cm_SendCommand(puchar pucInsBuff);
void cm_Delay(uchar ucDelay);

uchar cm_TRUE(void);
void cm_PowerOff(void);
void cm_PowerOn(void);
uchar cm_ReceiveData(puchar pucReceiveData, uchar ucLength);
uchar cm_SendData(puchar pucSendData, uchar ucLength);
uchar cm_SendCmdByte(uchar ucCommand);

void cm_RandGen(puchar pucRandomData);

void cm_GPAGen(uchar );
void cm_GPAcmd2(uchar *);
void cm_GPAencrypt(uchar, uchar *, uchar);
void cm_GPAdecrypt(uchar, uchar *, uchar);
void cm_GPAencrypt_WLM(uchar, uchar *, uchar);
#endif
