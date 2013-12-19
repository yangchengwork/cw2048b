#ifndef _ENCRYPT_MID_
#define _ENCRYPT_MID_

#include "cm_lib.h"
//#include "cm_i2c.h"
#define UINT8 unsigned char
extern uchar CWER;
extern uchar cw_WriteSmallZone(uchar  ucCryptoAddr, puchar pucBuffer, uchar ucCount);
extern uchar cw_ReadSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);
extern uchar cw_ReadConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);
extern uchar cw_WriteConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount, uchar ucAntiTearing);
extern void cw_er_init(void);

extern void cw_Encry(UINT8 * source,UINT8 * output, UINT8 len);
extern void cw_Decrypt(UINT8 * source,UINT8 * output, UINT8 len);

#endif
