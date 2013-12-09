// Functions that directly control the hardware that are not needed in all cases

#include "CM_LIB.H"
#include "CM_I2C.H"
#include "CM_I2C_L.H"
#include "CW_Encry.h"

/*
// Send a command byte
//
uchar cm_SendCmdByte(uchar ucCommand)
{
    uchar i, ucCmd;
	
    i = CM_START_TRIES;

    ucCmd = (ucCommand&0x0F)|CM_PORT_CFG.ucChipSelect;
    while (i) {
      cm_Start();
	    if (cm_Write(ucCmd) == 0) break;
	    if (--i == 0) return FAIL_CMDSTART;
    }

    return SUCCESS;
}
*/
// Send a command, modify by CW
//
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
    return SUCCESS;
}
