// Set User Zone
//

// CryptoMemory Library Include Files
#include "CM_I2C.H"
#include "CM_LIB.H"
#include "CM_GPA.H"
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

    // Only zone number is included in the polynomial
	cm_GPAGen(ucZoneNumber);
	
	if ((ucReturn = CM_LOW_LEVEL.SendCommand(ucCM_InsBuff))!= SUCCESS) return ucReturn;
	cm_Stop();
	// save zone number and anti-tearing state
    ucCM_UserZone = ucZoneNumber;
    ucCM_AntiTearing = ucAntiTearing;

    // done	
	//return  CM_LOW_LEVEL.ReceiveRet(NULL,0);
	return	ucReturn;
}
