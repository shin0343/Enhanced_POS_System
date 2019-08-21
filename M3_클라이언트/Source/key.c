#include "key.h"

#define KEY_PRESS_RELEASE_CHECK_COUNT 1000

void Key_Poll_Init(void)
{
	Macro_Set_Bit(rRCC_APB2ENR,2);
	Macro_Write_Block(rGPIOA_CRH,0xfff,0x444,20);
}

void Key_EXTI_Init(void)
{
	Macro_Set_Bit(rRCC_APB2ENR,2);                //  PortA clk enable
	Macro_Set_Bit(rRCC_APB2ENR,0);                //  Alternate function IO clock enable
	Macro_Write_Block(rGPIOA_CRH,0xfff,0x444,20); //  GPA15_13 configuration (input floating)

	rEXTI_PR = (0x7<<13);   //  GPA15_13 pending clear
	rNVIC_ICPR1 = 1<<8;    //  IRQ pendign clear

	Macro_Set_Bit(rNVIC_ISER1,8);  //  IRQ40 INT enable
	Macro_Write_Block(rAFIO_EXTICR4,0xfff,0x000,4); //  GPA15_13 select
	Macro_Write_Block(rEXTI_IMR,0x7,0x7,13);       	//  EXT15_13 intr unmask
	Macro_Write_Block(rEXTI_FTSR,0x7,0x7,13);      	//  Falling edge
}

U32 Key_Get_Pressed(void)
{
	U32 j, key, old_key = 0;
	U32 count = 0;

	for(j = 0 ; j < 3000 ; j++)
	{
		key = 0;

		switch((Macro_Extract_Area(rGPIOA_IDR,0x7,13)))
		{
		case 0x6 : key = 1; goto EXIT;
		case 0x5 : key = 4; goto EXIT;
		case 0x3 : key = 2; goto EXIT;
		}

	EXIT:
		if(old_key == key)
		{
			count++;
		}
		else
		{
			old_key = key;
			count = 0;
		}

		if(count >= KEY_PRESS_RELEASE_CHECK_COUNT) return key;
	}
	return 0;
}

void Key_Wait_Key_Released(void)
{
	while(Key_Get_Pressed());
}

U32 Key_Wait_Key_Pressed(void)
{
	U32 k;
	do
	{
		k = Key_Get_Pressed();
	}while(!k);
	return k;
}