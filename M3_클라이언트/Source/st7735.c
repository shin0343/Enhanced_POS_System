#include "stm32f10x.h"
#include "st7735.h"
#include "macro.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern void TIM_Delay_ms(unsigned int time);
extern void TIM_Delay_us(unsigned int time);
extern void Uart1_Printf(char *fmt,...);
extern void LED_Display(unsigned int num);

unsigned int Trans_mode = 0;
unsigned int Shape_mode = 0;
unsigned int Shape_mode_color = 0;

void Lcd_Write_Cmd(unsigned short cmd)
{
	LCD_DC_CLR();
	while((SPI1->SR&SPI_I2S_FLAG_TXE) == 0);
	SPI1->DR = cmd;
	while((SPI1->SR&SPI_I2S_FLAG_BSY) != 0);
	LCD_DC_SET();
}

void Lcd_Write_Two_Byte_Data(unsigned short data)
{
	while((SPI1->SR&SPI_I2S_FLAG_TXE) == 0);
	SPI1->DR = data;
	while((SPI1->SR&SPI_I2S_FLAG_BSY) != 0);
}

void Lcd_Config_Read(unsigned char read_cmd)
{
	volatile  int i;

	Lcd_Write_Cmd(read_cmd);

	Peri_Clear_Bit(SPI1->CR1,6);                                  //// SPI1 Disable (SPIE_CLERA)
	Macro_Write_Block(GPIOA->CRL, 0xf, 0x3, (ST7735_PIN_SCL*4));  //scl PP output configure

	LCD_SCL_CLR();                                                //scl dummy clock
	LCD_SCL_SET();
	Macro_Write_Block(GPIOA->CRL, 0xf, 0xb, (ST7735_PIN_SCL*4));  // scl AFIO PP configure

	Macro_Write_Block(SPI1->CR1, 0x3, 0x2, 14);  // SPI_Direction_1Line_Rx
	Macro_Clear_Bit(SPI1->CR1, 11);              // SPI_DataSize_8b
	Macro_Write_Block(SPI1->CR1, 0x7, 0x2, 3);   // SPI_BaudRatePrescaler_8
	//Macro_Write_Block(SPI1->CR1, 0x7, 0x3, 3);   // SPI_BaudRatePrescaler_16

	i = SPI1->DR;
}

void Lcd_Config_Write_Line(void)
{
	LCD_CS_SET();

	Peri_Clear_Bit(SPI1->CR1,6);      /// SPI1 Disable ( SPIE_CLERA )

	Macro_Write_Block(SPI1->CR1, 0x3, 0x3, 14);  // SPI_Direction_1Line_Tx
	Macro_Set_Bit(SPI1->CR1, 11);                // SPI_DataSize_16b
	Macro_Write_Block(SPI1->CR1, 0x7, 0x1, 3);   // SPI_BaudRatePrescaler_4

	Peri_Set_Bit(SPI1->CR1,6);        /// SPI1 Enable ( SPIE_SET )

	LCD_CS_CLR();
}

unsigned char Lcd_Read_One_Byte_Data(void)
{
	unsigned char data = 0;

	__disable_irq();                  /// golbal INterrupt disable
	Peri_Set_Bit(SPI1->CR1,6);        /// SPI1 Enable ( SPIE_SET )

	while(Peri_Check_Bit_Clear(SPI1->SR,0));
	data = SPI1->DR;

	Peri_Clear_Bit(SPI1->CR1,6);      /// SPI1 Disable ( SPIE_CLERA )
	__enable_irq();                   /// golbal INterrupt enable

	return data;
}

unsigned long Lcd_Read_State(void)
{
	unsigned long data;

	Lcd_Config_Read(ST7735_RDDST_REG);
	data  = Lcd_Read_One_Byte_Data()<<24;
	data |= Lcd_Read_One_Byte_Data()<<16;
	data |= Lcd_Read_One_Byte_Data()<<8;
	data |= Lcd_Read_One_Byte_Data();

	Lcd_Config_Write_Line();

	return data;
}

//*****************************************************************************
//! \brief GLCD State Print
//! \param None.
//! \return None.
//*****************************************************************************

void Lcd_State_Print(void)
{
	unsigned long data;

	data = Lcd_Read_State();

	Uart1_Printf("======================\n");
	Uart1_Printf("RDDST : 0x%08X\n",data);
	Uart1_Printf("  BSTON : %d  MY    : %d  MX    : %d  MV    : %d\n",
							 Macro_Check_Bit_Set(data, 31),
							 Macro_Check_Bit_Set(data, 30),
							 Macro_Check_Bit_Set(data, 29),
							 Macro_Check_Bit_Set(data, 28));
	Uart1_Printf("  ML    : %d  RGB   : %d  MH    : %d  ST24  : %d\n",
							 Macro_Check_Bit_Set(data, 27),
							 Macro_Check_Bit_Set(data, 26),
							 Macro_Check_Bit_Set(data, 25),
							 Macro_Check_Bit_Set(data, 24));
	Uart1_Printf("  ST23  : %d  IFPF2 : %d  IFPF1 : %d  IFPF0 : %d\n",
							 Macro_Check_Bit_Set(data, 23),
							 Macro_Check_Bit_Set(data, 22),
							 Macro_Check_Bit_Set(data, 21),
							 Macro_Check_Bit_Set(data, 20));
	Uart1_Printf("  IDMON : %d  PTLON : %d  SLOUT : %d  NORON : %d\n",
							 Macro_Check_Bit_Set(data, 19),
							 Macro_Check_Bit_Set(data, 18),
							 Macro_Check_Bit_Set(data, 17),
							 Macro_Check_Bit_Set(data, 16));
	Uart1_Printf("  VSSON : %d  ST14  : %d  INVON : %d  ST12  : %d\n",
							 Macro_Check_Bit_Set(data, 15),
							 Macro_Check_Bit_Set(data, 14),
							 Macro_Check_Bit_Set(data, 13),
							 Macro_Check_Bit_Set(data, 12));
	Uart1_Printf("  ST11  : %d  DISON : %d  TEON  : %d  GCS2  : %d\n",
							 Macro_Check_Bit_Set(data, 11),
							 Macro_Check_Bit_Set(data, 10),
							 Macro_Check_Bit_Set(data, 9),
							 Macro_Check_Bit_Set(data, 8));
	Uart1_Printf("  GCS1  : %d  GCS0  : %d  TEM   : %d  ST4   : %d\n",
							 Macro_Check_Bit_Set(data, 7),
							 Macro_Check_Bit_Set(data, 6),
							 Macro_Check_Bit_Set(data, 5),
							 Macro_Check_Bit_Set(data, 4));
	Uart1_Printf("  ST3   : %d  ST2   : %d  ST1   : %d  ST0   : %d\n",
							 Macro_Check_Bit_Set(data, 3),
							 Macro_Check_Bit_Set(data, 2),
							 Macro_Check_Bit_Set(data, 1),
							 Macro_Check_Bit_Set(data, 0));
}

//*****************************************************************************
//! \brief Set the cursor location.
//! \param st_x is the character index.
//! \param end_x is the line number.
//! The default location is (0,0). After set the location, the strings will
//! display at the (col, row).
//! \return None.
//*****************************************************************************
void Lcd_Set_Cur_Pos(unsigned short st_x, unsigned short end_x,
										 unsigned short st_y, unsigned short end_y)
{
	Lcd_Write_Cmd(ST7735_CASET_REG);
	Lcd_Write_Two_Byte_Data(st_x);
	Lcd_Write_Two_Byte_Data(end_x);
	Lcd_Write_Cmd(ST7735_RASET_REG);
	Lcd_Write_Two_Byte_Data(st_y);
	Lcd_Write_Two_Byte_Data(end_y);
	Lcd_Write_Cmd(ST7735_RAMWR_REG);
}

//*****************************************************************************
//! \brief Draws a pixel on the screen.
//! \param x is the X coordinate of the pixel.
//! \param y is the Y coordinate of the pixel.
//! \param color is the color of the pixel.
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//! \return None.
//*****************************************************************************
void Lcd_Draw_One_Pixel(unsigned short x, unsigned short y, unsigned short color)
{
	Lcd_Set_Cur_Pos(x, x, y, y);
	Lcd_Write_Two_Byte_Data(color);
}

unsigned short Lcd_Read_One_Pixel(unsigned short x, unsigned short y)
{
	unsigned short color_data;

	Lcd_Write_Cmd(ST7735_COLMOD_REG);
	Lcd_Write_Two_Byte_Data(0x6600);

	Lcd_Set_Cur_Pos(x, x, y, y);
	Lcd_Config_Read(ST7735_RAMRD_REG);

	color_data = Lcd_Read_One_Byte_Data(); /// dummy data read!!

	color_data = ((Lcd_Read_One_Byte_Data()&((unsigned char)0xF8))<<8)|
		((Lcd_Read_One_Byte_Data()&((unsigned char)0xFC))<<3)|
			(Lcd_Read_One_Byte_Data()>>3);

	Lcd_Config_Write_Line();

	Lcd_Write_Cmd(ST7735_COLMOD_REG);
	Lcd_Write_Two_Byte_Data(0x0500);

	return color_data;
}

//*****************************************************************************
//
//! \brief Init the ST7735 LCD Device.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void Lcd_Init(void)
{
	volatile unsigned short i;
	unsigned short temp_data;

	/* GPIOA, SPI1 Periph clock enable */
	Peri_Set_Bit(RCC->APB2ENR, 0); // AFIO  Clock Enable
	Peri_Set_Bit(RCC->APB2ENR, 2); // GPIOA Clock Enable
	Peri_Set_Bit(RCC->APB2ENR,12); // SPI1 Clock Enable

	/* GPIO Configure */
	Macro_Write_Block(GPIOA->CRH, 0xf, 0x3, ((12-8)*4));  //ST7735_PIN_RST(GPIOA12) PP output configure
	Macro_Write_Block(GPIOA->CRH, 0xf, 0x3, ((11-8)*4));  //ST7735_PIN_DC (GPIOA11) PP output configure
	Macro_Write_Block(GPIOA->CRL, 0xf, 0x3, ((4)*4));     //ST7735_PIN_CS (GPIOA4 ) PP output configure
	Macro_Write_Block(GPIOA->CRL, 0xf, 0xB, ((7)*4));     //ST7735_PIN_SDA(GPIOA7 ) AF PP configure
	Macro_Write_Block(GPIOA->CRL, 0xf, 0xB, ((5)*4));     //ST7735_PIN_SCL(GPIOA5 ) AF PP configure

	/* GPIO_Value default Setting */
	LCD_CS_SET();

	/* SPI1 Config -------------------------------------------------------------*/
	//SPI_Direction_1Line_Tx
	//SPI_Mode_Master
	//SPI_DataSize_16b
	//SPI_CPOL_High
	//SPI_CPHA_2Edge
	//SPI_NSS_Soft
	//SPI_BaudRatePrescaler_4
	//SPI_FirstBit_MSB
	SPI1->CR1 = (0x3<<14)|(0x1<<11)|(0x0<<10)|(0x1<<9)|(0x1<<8)|(0x0<<7)|(0x1<<3)|(0x1<<2)|(0x1<<1)|(0x1<<0);
	SPI1->I2SCFGR &= ~(0x1<<11);    /// SPI mode is selected
	SPI1->CR1|= 0x1<<6;             /// SPIE Enable

	LCD_CS_CLR();

	LCD_RST_CLR();
	TIM_Delay_ms(100);
	LCD_RST_SET();
	TIM_Delay_ms(100);

	Lcd_Write_Cmd(ST7735_SLPOUT_REG);     //sleep out and booter on
	//    TIM_Delay_ms(5000);
	TIM_Delay_ms(1000);

	Lcd_Write_Cmd(ST7735_FRMCTR1_REG);   //Frame Rate Control in normal mode
	Lcd_Write_Two_Byte_Data(0x12C);	     // RTNA = 0x01, FPA = 0x2C
	Lcd_Write_Two_Byte_Data(0x2D00);     // BPA = 0x2D
	// Frame rate = 850kHz/((RTNAx2 +40)x(LINE+FPA+BPA+2)) = 81 Hz

	Lcd_Write_Cmd(ST7735_INVOFF_REG);
	Lcd_Write_Cmd(ST7735_MADCTL_REG);    // memory  access   control
	Lcd_Write_Two_Byte_Data(0xA400);

	Lcd_Write_Cmd(ST7735_COLMOD_REG);
	Lcd_Write_Two_Byte_Data(0x0500);

	/// LUT Table Setting
	Lcd_Write_Cmd(ST7735_RGBSET_REG);
	for(i=0;i<32;i+=2) // Red Tone Setting
	{
		temp_data = (i<<1)<<8;
		temp_data|= ((i+1)<<1);
		Lcd_Write_Two_Byte_Data(temp_data);
	}
	for(i=0;i<64;i+=2)
	{
		temp_data = i<<8;
		temp_data|= i+1;
		Lcd_Write_Two_Byte_Data(temp_data);
	}
	for(i=0;i<32;i+=2)
	{
		temp_data = (i<<1)<<8;
		temp_data|= ((i+1)<<1);
		Lcd_Write_Two_Byte_Data(temp_data);
	}

	Lcd_Write_Cmd(ST7735_DISPON_REG);

	Lcd_Write_Cmd(ST7735_CASET_REG);    // SET draw pos
	Lcd_Write_Two_Byte_Data(0);
	Lcd_Write_Two_Byte_Data(LCD_HORIZONTAL_MAX - 1);
	Lcd_Write_Cmd(ST7735_RASET_REG);
	Lcd_Write_Two_Byte_Data(0);
	Lcd_Write_Two_Byte_Data(LCD_VERTICAL_MAX - 1);

	Lcd_Write_Cmd(ST7735_RAMWR_REG);   // Clear GLCD RAM

	for(i = 0; i < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); i++)
	{
		Lcd_Write_Two_Byte_Data(0xff00);
	}
}

void Lcd_Put_Pixel(int x,int y,int color)
{
	if(!Shape_mode || (Shape_mode_color != color))
	{
		if(!Trans_mode)
		{
			Lcd_Draw_One_Pixel(x,y,color);
		}
		else
		{
			color = (Lcd_Read_One_Pixel(x,y)|color);
			Lcd_Draw_One_Pixel(x,y,color);
		}
	}
}

void Lcd_Set_Trans_Mode(int mode)
{
	if(mode) Trans_mode = 1;
	else Trans_mode = 0;
}

void Lcd_Set_Shape_Mode(int mode, int color)
{
	if(mode)
	{
		Shape_mode = 1;
		Shape_mode_color = color;
	}
	else
	{
		Shape_mode = 0;
	}
}

void Lcd_Draw_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1;
	dy=y2-y1;

	if(dx>=0)
	{
		if(dy >= 0) 	// dy>=0
		{
			if(dx>=dy) 	// 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   	// dy<0
		{
			dy=-dy;   	// dy=abs(dy)

			if(dx>=dy) 	// 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}
	else //dx<0
	{
		dx=-dx;			//dx=abs(dx)
		if(dy >= 0) 	// dy>=0
		{
			if(dx>=dy) 	// 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   	// dy<0
		{
			dy=-dy;   	// dy=abs(dy)

			if(dx>=dy) 	// 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}
}

void Lcd_Draw_Hline(int y, int x1, int x2, int color)
{
	int i, xx1, xx2;

	if(x1<x2)
	{
		xx1=x1;
		xx2=x2;
	}
	else
	{
		xx1=x2;
		xx2=x1;
	}
	for(i=xx1;i<=xx2;i++)
	{
		Lcd_Put_Pixel(i,y,color);
	}
}

void Lcd_Draw_Vline(int x, int y1, int y2, int color)
{
	int i, yy1, yy2;

	if(y1<y2)
	{
		yy1=y1;
		yy2=y2;
	}
	else
	{
		yy1=y2;
		yy2=y1;
	}
	for(i=yy1;i<=yy2;i++)
	{
		Lcd_Put_Pixel(x,i,color);
	}
}

void Lcd_Draw_Rect(int x1, int y1, int x2, int y2, int color)
{
	int xx1, yy1, xx2, yy2;

	if(x1<x2)
	{
		xx1=x1;
		xx2=x2;
	}
	else
	{
		xx1=x2;
		xx2=x1;
	}

	if(y1<y2)
	{
		yy1=y1;
		yy2=y2;
	}
	else
	{
		yy1=y2;
		yy2=y1;
	}

	Lcd_Draw_Hline(yy1,xx1,xx2,color);
	Lcd_Draw_Hline(yy2,xx1,xx2,color);
	Lcd_Draw_Vline(xx1,yy1,yy2,color);
	Lcd_Draw_Vline(xx2,yy1,yy2,color);
}

void Lcd_Draw_Bar(int x1, int y1, int x2, int y2, int color)
{
	int i, j;
	int xx1, yy1, xx2, yy2;

	if(x1<x2)
	{
		xx1=x1;
		xx2=x2;
	}
	else
	{
		xx1=x2;
		xx2=x1;
	}

	if(y1<y2)
	{
		yy1=y1;
		yy2=y2;
	}
	else
	{
		yy1=y2;
		yy2=y1;
	}

	for(i=yy1;i<=yy2;i++)
	{
		for(j=xx1;j<=xx2;j++)
		{
			Lcd_Put_Pixel(j,i,color);
		}
	}
}

void Lcd_Clr_Screen(int color)
{
	int i,j;

	for(j=0;j<LCD_VERTICAL_MAX;j++)
	{
		for(i=0;i<LCD_HORIZONTAL_MAX;i++)
		{
			Lcd_Put_Pixel(i, j, color);
		}
	}
}
///////////////////////// Font Display functions ////////////////////////
#include ".\Fonts\ENG8X16.H"

#define 	ENG_FONT_X 		8
#define 	ENG_FONT_Y 		16

#define COPY(A,B) 	for(loop=0;loop<32;loop++) *(B+loop)=*(A+loop);
#define OR(A,B) 	for(loop=0;loop<32;loop++) *(B+loop)|=*(A+loop);

void Lcd_Eng_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy)
{
	unsigned offset,loop;
	unsigned char xs,ys;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};

	offset=(unsigned)(data*16);
	COPY(eng8x16+offset,temp);

	for(ys=0;ys<16;ys++)
	{
		for(xs=0;xs<8;xs++)
		{
			if(temp[ys]&bitmask[xs])
			{

				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,color);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,color);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,color);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,color);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,color);
				}
			}
			else
			{
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,bkcolor);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,bkcolor);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,bkcolor);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,bkcolor);
				}
			}
		}
	}
}

void Lcd_Puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy)
{
	unsigned data;

	while(*str)
	{
		data=*str++;
		if(data>=128)
		{
			*str++;
			x+=zx*16;
		}
		else
		{
			Lcd_Eng_Putch(x, y, color, bkcolor, (int)data, zx, zy);
			x+=zx*ENG_FONT_X;
		}
	}
}

void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...)
{
	va_list ap;
	char string[256];

	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	Lcd_Puts(x, y, color, bkcolor, string, zx, zy);
	va_end(ap);
}

void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned short int *fp)
{
	*x =(int)fp[0];
	*y =(int)fp[1];
}

void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp)
{
	register int width = fp[0], height = fp[1];
	register int xx, yy;

	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			Lcd_Put_Pixel(xx+x,yy+y,(int)fp[yy*width+xx+2]);
		}
	}
}

void Lcd_Draw_Image(int x, int y, const unsigned short int *fp, int width, int height)
{
	// BIN 파일 자체에 해상도 정보가 없는 경우 사용
	register int xx, yy;

	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			Lcd_Put_Pixel(xx+x,yy+y,(int)fp[yy*width+xx]);
		}
	}
}

