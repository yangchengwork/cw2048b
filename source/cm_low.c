// Data and Functions used by other low level functions
//
// Note: this module must be after all other low level functions in the library
//       to assure that any reference to functions in this library are satistified.

#include "cm_lib.h"
#include <linux/delay.h>

// Zone Data
uchar ucCM_UserZone;
uchar ucCM_AntiTearing;

// Chip state
uchar ucCM_Encrypt;
uchar ucCM_Authenticate;

// Global data
uchar ucCM_InsBuff[4];

// Delay
void cm_Delay(uchar ucDelay)
{
#if 0
	//uchar ucTimer;
	uint ucTimer; 
	
    while(ucDelay) {
    	ucTimer = CM_TIMER;
        while(ucTimer) ucTimer--;
    	ucDelay--;
    }
#endif
	udelay(1);    
}

