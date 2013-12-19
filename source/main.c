/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The EEPROM 24C02 application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-29
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

// CryptoMemory Library Include Files

#include "cm_lib.h"
#include "cm_i2c.h"
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define AT88_Power_On  GPIOE->BSRR = GPIO_Pin_7	 /* GPIO_SetBits(GPIOE , GPIO_Pin_7)   */	
#define AT88_Power_Off 	GPIOE->BRR = GPIO_Pin_7





// Function Prototypes
void Step(uchar ucStp);
void Done(uchar ucReturn, uchar ucStatus);
void Delay(uchar ucDelay);

/* Private function prototypes -----------------------------------------------*/
void USART_Configuration(void);
void I2C_Configuration(void);
void LED_Configuration(void);
uchar Main_authen_pw(void);
void delay_ms(void);


/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
unsigned char i,j,tmp;
unsigned char PAC_WR7;//PAC_WR7=0xFF则表明校验SC正确
unsigned char AAC;    //AAC=0xFF则表明校验GCx正确
unsigned char PAC;    //PAC=0xFF则表明校验PWx正确

const unsigned char Ci1[]={0XFF,0xC1,0x06,0xB9,0x7B,0x28,0xDD,0xD5};

//******************delay_ms*********************************
//功能:延时1ms
//*****************************************************************
void delay_ms(void)
{
unsigned int j;
     for(j=0x00;j<255*4;j++)
        {
          j++;
        }
}

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : AT88管脚配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  /* Configure I2C2 pins: PD7->SCL and PD12->SDA and PE7->Power*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : LED_Configuration
* Description    : LED管脚配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LED_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = 0x00FF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX -> PA10  
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

int main(void)
{		
	USART_Configuration();
	I2C_Configuration();
	LED_Configuration();
	Main_authen_pw();

}

//*********************************************************************************************************
//      END FILE
//*********************************************************************************************************
