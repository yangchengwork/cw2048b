// Mid Level Utility Function: cm_ReadCommand()
//
// Note: this module must be after all low level functions in the library and
//       before all high level user function to assure that any reference to
//       this function in this library are satistified.

#include "CM_LIB.H"
#include "CM_I2C.H"

uchar cm_ReadCommand(puchar pucInsBuff, puchar pucRetVal, uchar ucLen)
{ 
    uchar ucReturn;
    
    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
    return CM_LOW_LEVEL.ReceiveRet(pucRetVal, ucLen);
}

uchar cm_ReadCommand_rd(puchar pucInsBuff, puchar pucRetVal, uchar ucLen)
{ 
    uchar ucReturn;
    uchar rand_cmd=0xB1;
	
    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
	if ((ucReturn = CM_LOW_LEVEL.SendCmdByte(rand_cmd)) != SUCCESS) return ucReturn;
    return CM_LOW_LEVEL.ReceiveRet(pucRetVal, ucLen);
}

