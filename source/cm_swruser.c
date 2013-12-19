// Write Small User Zone
//
// The Write Small User Zone function is used to write data to CryptoMemory devices that have
// 256 bytes or less in each user zone (AT88SC3216C, and smaller)

// CryptoMemory Library Include Files
#include "cm_lib.h"
//#include "cm_gpa.h"
#include "cw_low.h"

// Write Small User Zone
uchar cm_WriteSmallZone(uchar  ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;
	
	ucCM_InsBuff[0] = 0xb0;
    ucCM_InsBuff[1] = 0x00;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;
	// Write the data
	ucReturn = cm_WriteCommand(ucCM_InsBuff, pucBuffer,ucCount);

	return ucReturn;
}

// Write Small User Zone
uchar cm_WriteSmallZone_CK(uchar  ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;
	
	ucCM_InsBuff[0] = 0xb0;
    ucCM_InsBuff[1] = 0x00;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;	
	// Write the data
	ucReturn = cm_WriteCommand_NW(ucCM_InsBuff, pucBuffer,ucCount);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	cm_Delay(200);
	return ucReturn;
}
 
 uchar CW_write(uchar code,uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
    uchar ucReturn;	
    ucCM_InsBuff[0] = 0xb9; 
    ucCM_InsBuff[1] = code;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;
    // Do the write
	ucReturn = cm_WriteCommand(ucCM_InsBuff, pucBuffer,ucCount);
    return ucReturn;
}

