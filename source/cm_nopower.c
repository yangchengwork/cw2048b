// Functions control the logical power on/off for the chip

#include "CM_LIB.H"
#include "CM_I2C.H"
#include "CM_I2C_L.H"
#include "CM_GPA.H"

// Power On Chip  
//
// Returns 0 (SUCCESS) if no error
//
void cm_PowerOn(void)   
{
	// Reset chip data
	cm_ResetCrypto();
    ucCM_UserZone = ucCM_AntiTearing = 0;
    
    // Sequence for powering on secure memory according to ATMEL spec
//    CM_DATA_OUT;                              // SDA and SCL start as outputs
//	CM_CLK_OUT;
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
