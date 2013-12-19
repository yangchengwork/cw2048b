// Set User Zone
//

// CryptoMemory Library Include Files
#include "cm_lib.h"
#include "cw_low.h"

#ifndef  SUCCESS
	#define  SUCCESS 1
#endif
// Set User Zone
uchar cm_SetUserZone(uchar ucZoneNumber, uchar ucAntiTearing)
{
	uchar ucReturn;
	
	ucCM_InsBuff[0] = 0xb4;
	if (ucAntiTearing) ucCM_InsBuff[1] = 0x0b;
	else 	           ucCM_InsBuff[1] = 0x03;
    ucCM_InsBuff[2] = ucZoneNumber;
	ucCM_InsBuff[3] = 0x00;
	if ((ucReturn = CM_LOW_LEVEL.SendCommand(ucCM_InsBuff))!= SUCCESS) return ucReturn;
	cm_Stop();
	// save zone number and anti-tearing state
    ucCM_UserZone = ucZoneNumber;
	return	ucReturn;
}
