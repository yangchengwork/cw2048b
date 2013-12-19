// Functions control the logical power on/off for the chip

#include "cm_lib.h"
#include "cw_low.h"

// Power On Chip  
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
