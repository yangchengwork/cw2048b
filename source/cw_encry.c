// CryptoMemory Library Include Files
#include "cm_lib.h"
#include "cw_encry.h"
#include "cw_low.h"

unsigned int m_gpa;
uchar CWER,lsfr_out;
void cw_er_init(void);
uchar cw_ReadConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);
uchar cw_WriteConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount, uchar ucAntiTearing);

void cw_Encry(UINT8 * source,UINT8 * output, UINT8 len);
void cw_Decrypt(UINT8 * source,UINT8 * output, UINT8 len);

void cw_er_init(void)
{
	m_gpa=0x00000000; // 0xFFFFFFFF; // 
	CWER=1;	//enable or disable 
}

void cw_Encry(UINT8 * source,UINT8 * output,UINT8 len)
{
	unsigned char j,k =0,tmp;
	UINT8 tmp0,tmp1,tmp2;
	uchar source_tmp[16];
	
	for(k=0;k<len;k++) source_tmp[k]=source[k];
		
	for(k = 0; k < len; k++)	
	{

		for(j=8; j > 0; j--)
		{
			tmp = (source_tmp[k]>>(j-1))&0x01;
			lsfr_out= ((m_gpa>>31)&0x01);
			if(tmp^lsfr_out)	tmp0 = 1;
			else	tmp0 = 0;
			output[k] = (output[k]<<1)|tmp0;

			//((k == (ENCRYPT_SEND_CMD_LEN-1))&&(j == 1))	return;

			tmp0 = (m_gpa>>31)&0x01;
			tmp1 = (m_gpa>>1)&0x01;
			tmp2 = (m_gpa>>21)&0x01;
			m_gpa = m_gpa<<1;

			tmp = (tmp0^tmp1^tmp2)&0x01;
			m_gpa = (m_gpa&0xfffffffe)|tmp;		
		} 
	}
	lsfr_out= ((m_gpa>>31)&0x01);
}

void cw_Decrypt(UINT8 * source,UINT8 * output, UINT8 len)
{
	unsigned char j,k =0,tmp;
	UINT8 tmp0,tmp1,tmp2;



	for(k = 0; k < len; k++)	
	{
		//if(k == 0 )	j = 7;
		//else j = 8;
		tmp = (source[k]>>7)&0x01;
		if(tmp^lsfr_out)	tmp0 = 1;
		else	tmp0 = 0;
		output[k] = (output[k]<<1)|tmp0;

		for(j=7; j > 0; j--)
		{

			tmp = (source[k]>>(j-1))&0x01;
			lsfr_out= ((m_gpa>>31)&0x01);
			if(tmp^lsfr_out)	tmp0 = 1;
			else	tmp0 = 0;
			output[k] = (output[k]<<1)|tmp0;

			//((k == (ENCRYPT_SEND_CMD_LEN-1))&&(j == 1))	return;

			tmp0 = (m_gpa>>31)&0x01;
			tmp1 = (m_gpa>>1)&0x01;
			tmp2 = (m_gpa>>21)&0x01;
			m_gpa = m_gpa<<1;

			tmp = (tmp0^tmp1^tmp2)&0x01;
			m_gpa = (m_gpa&0xfffffffe)|tmp;
		} 
		tmp0 = (m_gpa>>31)&0x01;
		tmp1 = (m_gpa>>1)&0x01;
		tmp2 = (m_gpa>>21)&0x01;
		m_gpa = m_gpa<<1;

		tmp = (tmp0^tmp1^tmp2)&0x01;
		m_gpa = (m_gpa&0xfffffffe)|tmp;	

		lsfr_out= ((m_gpa>>31)&0x01);
	}

}
