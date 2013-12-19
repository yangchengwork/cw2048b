// Code for Testing the CryptoMemory Library
// Microcontroller specific include
// #include "stm32f10x.h"
#include <linux/module.h>
#define SUCCESS 1
#include "cw_encry.h"
#define CW_secure_cmd 0x1e
#define CW_user_cmd 0x17
#define CW_power_off 0x16
// Global Variables
uchar rwdata[16],cpdata[16];
uchar L1_state,state,L1_return,L2_return,L_return,fuse,all_done;
// Function Prototypes	   
uchar CW_write(uchar code,uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);
uchar CW_read(uchar code,uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);
uchar ucReturn;
uchar ucData[16]; 
uchar ucReturn;

 /**************************************************************/
uchar L1_CW_test(void)
{
	cw_er_init();
	//rwdata[0]=0xff;rwdata[1]=0xff;rwdata[2]=0xff;rwdata[3]=0xff;
	//L1_return=CW_write(CW_user_cmd, 0x04,rwdata,4);	//write code 
	rwdata[0]=0;rwdata[1]=0;rwdata[2]=0;rwdata[3]=0;
	L1_return=CW_read(CW_user_cmd, 0x04,rwdata,4);	//read code				
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], L1_return);
	rwdata[0]=0x22;rwdata[1]=0x33;	//write user zone  
	L1_return=cm_WriteSmallZone(0x00,rwdata,2);
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], L1_return);
	rwdata[0]=0;rwdata[1]=0;
	L1_return=cm_ReadSmallZone(0x00,rwdata,2);	 //read user zone
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], L1_return);
	cm_SetUserZone(1,0);   //set user zone 	1
	rwdata[0]=0xAA;rwdata[1]=0x55;	  
	L1_return=cm_WriteSmallZone(0x080,rwdata,2);
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], L1_return);
	rwdata[0]=0;rwdata[1]=0;
	L1_return=cm_ReadSmallZone(0x080,rwdata,2);						
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], L1_return);
	return SUCCESS;	
}

uchar Main_authen_pw(void)		
{	
	L1_return = L1_CW_test();
	if(!L1_return) return L1_return;		
}
