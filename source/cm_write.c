// Mid Level Utility Function: cm_WriteCommand()
//
// Note: this module must be after all low level functions in the library and
//       before all high level user function to assure that any reference to
//       this function in this library are satistified.

#include "cm_lib.h"
#include "cw_low.h"

uchar cm_WriteCommand(puchar pucInsBuff, puchar pucSendVal, uchar ucLen)
{ 
    uchar ucReturn;

    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
    //return CM_LOW_LEVEL.SendData(pucSendVal, ucLen);
    ucReturn=CM_LOW_LEVEL.SendData(pucSendVal, ucLen);
	CM_LOW_LEVEL.WaitClock(10);
	return ucReturn;
}

uchar cm_WriteCommand_NW(puchar pucInsBuff, puchar pucSendVal, uchar ucLen)
{ 
    uchar ucReturn;

    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
    //return CM_LOW_LEVEL.SendData(pucSendVal, ucLen);
    ucReturn=CM_LOW_LEVEL.SendData(pucSendVal, ucLen);
//	CM_LOW_LEVEL.WaitClock(10);
	return ucReturn;
}
