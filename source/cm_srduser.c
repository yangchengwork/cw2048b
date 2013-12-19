// Read Small User Zone
//
// The Read Small User Zone function is used to read data from CryptoMemory devices that
// have 256 bytes or less in each user zone (AT88SC3216C, and smaller)

// CryptoMemory Library Include Files
#include "cm_lib.h"
#include "cw_low.h"

// Read Small User Zone
uchar cm_ReadSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;

    ucCM_InsBuff[0] = 0xb2;
    ucCM_InsBuff[1] = 0;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;
    
    // Read the data
    if ((ucReturn = cm_ReadCommand(ucCM_InsBuff, pucBuffer, ucCount)) != SUCCESS) return ucReturn;

    // Done
    return SUCCESS;
}

uchar cm_ReadSmallZone_rd(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;

    ucCM_InsBuff[0] = 0xb0;
    ucCM_InsBuff[1] = 0;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = 4;    
    // Read the data
    if ((ucReturn = cm_ReadCommand_rd(ucCM_InsBuff, pucBuffer, ucCount)) != SUCCESS) return ucReturn;
    // Done
    return SUCCESS;
}

uchar CW_read(uchar code,uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
    uchar ucReturn;	
    ucCM_InsBuff[0] = 0xbe; 
    ucCM_InsBuff[1] = code;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;
    // Do the write
	ucReturn=cm_ReadCommand(ucCM_InsBuff, pucBuffer, ucCount);
    return ucReturn;
}