#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "udf.h"


void Uart1_Init(int baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Enable the USART1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//Clock Setting
/* Enable USART1, GPIOA, GPIOx and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
												 RCC_APB2Periph_AFIO, ENABLE);
	
  /* Configure USART1 Tx (PA.09) as alternate function open-drain */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*clear USART1 update interrupt*/
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	
	/* USART1 IT enable */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
	//USART1 Setting
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	
	/* USART1 enable */	
	USART_Cmd(USART1, ENABLE);

}

void Uart2_Init(int baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
  /* Enable the USART2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//Clock Setting RCC_APB1Periph_USART2
/* Enable USART2, GPIOA, GPIOx and AFIO clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
  /* Configure USART2 Tx (PA.02) as alternate function open-drain */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*clear USART2 update interrupt*/
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	
	/* USART2 IT enable */
//  USART_ITConfig(USART2, USART_IT_ORE, ENABLE);
//  USART_ITConfig(USART2, USART_IT_NE, ENABLE);
//  USART_ITConfig(USART2, USART_IT_FE, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
// 오버런, 노이즈 프레임에러 및 rx 인터럽트
//  USART_ITConfig(USART2, USART_IT_RXNE | USART_IT_ORE | USART_IT_NE | USART_IT_FE, ENABLE);
	
	//USART2 Setting
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	
	/* USART2 enable */	
	USART_Cmd(USART2, ENABLE);
}
void Uart3_Init(int baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	
  /* Enable the USART3 global Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
	
	//Clock Setting RCC_APB1Periph_USART3
/* Enable USART3, GPIOB, GPIOx and AFIO clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
  /* Configure USART3 Tx (PB.10) as alternate function open-drain */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /* Configure USART3 Rx (PB.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*clear USART3 update interrupt*/
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	
	/* USART3 IT enable */
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  	
	
	//USART3 Setting
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	
	/* USART2 enable */	
	USART_Cmd(USART3, ENABLE);
}

int Uart1_Send_Byte_Ex(unsigned char data)
{
		if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			return EC_FAILED;
		else
		{
			USART_SendData(USART1, data);
			return EC_SUCCESS;
		}
}

void Uart1_Send_Byte(unsigned char data)
{
		if(data=='\n')
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
				USART1->DR = 0x0d;
		}
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
		USART_SendData(USART1, data);
}

int Uart2_Send_Byte_Ex(unsigned char data)
{
		if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
			return EC_FAILED;
		else
		{
			USART_SendData(USART2, data);
			return EC_SUCCESS;
		}
}

void Uart2_Send_Byte(unsigned char data)
{
		if(data=='\n')
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
				USART2->DR = 0x0d;
		}
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
		USART_SendData(USART2, data);
}

int Uart3_Send_Byte_Ex(unsigned char data)
{
		if(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
			return EC_FAILED;
		else
		{
			USART_SendData(USART3, data);
			return EC_SUCCESS;
		}
}
void Uart3_Send_Byte(unsigned char data)
{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
		USART_SendData(USART3, data);
}


void Uart1_Send_String(unsigned char *pt)
{
    while(*pt!=0)
    {
        Uart1_Send_Byte(*pt++);
    }
}

void Uart2_Send_String(unsigned char *pt)
{
    while(*pt!=0)
    {
        Uart2_Send_Byte(*pt++);
    }
}


void Uart1_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart1_Send_String((unsigned char*)string);
    va_end(ap);
}

void Uart2_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart2_Send_String((unsigned char*)string);
    va_end(ap);
}


char Uart1_Get_Pressed(void)
{
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
		{
				return (char)USART1->DR;
		}
		else
		{
				return (char)0;
		}
}


char Uart1_Get_Char(void)
{
		char rx;
		
		do
		{
				rx = Uart1_Get_Pressed();
		}while(!rx);
		
		return rx;
}


void Uart1_Get_String(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart1_Get_Char())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart1_Printf("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart1_Send_Byte(c);
        }
    }
    *string='\0';
    Uart1_Send_Byte('\n');
}


int Uart1_Get_Int_Num(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart1_Get_String(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}
