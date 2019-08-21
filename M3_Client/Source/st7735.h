//*****************************************************************************
//#define LCD_VERTICAL_MAX        160 /// MV = 0
//#define LCD_HORIZONTAL_MAX      128
#define LCD_VERTICAL_MAX        128 /// MV = 1
#define LCD_HORIZONTAL_MAX      160

#define NOT_FONT_USE            0
#define FONT_USE                1
#define FONT_HANGLE_USE         NOT_FONT_USE
//*****************************************************************************
//
//! \addtogroup ST7735S_Signals ST7735 Signals
//
//*****************************************************************************
#define ST7735_PIN_RST          12   //PA12
#define ST7735_PIN_DC           11   //PA11
#define ST7735_PIN_SDA          7    //PA7
#define ST7735_PIN_SCL          5    //PA5
#define ST7735_PIN_CS           4    //PA4

#define LCD_RST_SET()   GPIOA->BSRR = (0x1<<12)
#define LCD_DC_SET()    GPIOA->BSRR = (0x1<<11)
#define LCD_SDA_SET()   GPIOA->BSRR = (0x1<<7)
#define LCD_SCL_SET()   GPIOA->BSRR = (0x1<<5)
#define LCD_CS_SET()    GPIOA->BSRR = (0x1<<4)
//
#define LCD_RST_CLR()   GPIOA->BRR = (0x1<<12)
#define LCD_DC_CLR()    GPIOA->BRR = (0x1<<11)
#define LCD_SDA_CLR()   GPIOA->BRR = (0x1<<7)
#define LCD_SCL_CLR()   GPIOA->BRR = (0x1<<5)
#define LCD_CS_CLR()    GPIOA->BRR = (0x1<<4)


//#define LCD_RST_SET()   ((*(volatile unsigned long *)(&(GPIOA->BSRR))) = (0x1<<12))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_DC_SET()    ((*(volatile unsigned long *)(&(GPIOA->BSRR))) = (0x1<<11))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_SDA_SET()   ((*(volatile unsigned long *)(&(GPIOA->BSRR))) = (0x1<<7))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_SCL_SET()   ((*(volatile unsigned long *)(&(GPIOA->BSRR))) = (0x1<<5))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_CS_SET()    ((*(volatile unsigned long *)(&(GPIOA->BSRR))) = (0x1<<4))//;{volatile int i;for(i=0;i<1000;i++);}
////
//#define LCD_RST_CLR()   ((*(volatile unsigned long *)(&(GPIOA->BRR))) = (0x1<<12))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_DC_CLR()    ((*(volatile unsigned long *)(&(GPIOA->BRR))) = (0x1<<11))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_SDA_CLR()   ((*(volatile unsigned long *)(&(GPIOA->BRR))) = (0x1<<7))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_SCL_CLR()   ((*(volatile unsigned long *)(&(GPIOA->BRR))) = (0x1<<5))//;{volatile int i;for(i=0;i<1000;i++);}
//#define LCD_CS_CLR()    ((*(volatile unsigned long *)(&(GPIOA->BRR))) = (0x1<<4))//;{volatile int i;for(i=0;i<1000;i++);}

#define ST7735_CS_DISABLE      1
#define ST7735_CS_ENABLE       0

#define ST7735_RS_COMMAND      0
#define ST7735_RS_DATA         1

#define ST7735_WR_LOW          0
#define ST7735_WR_HIGH         1

#define ST7735_RD_READ         0
#define ST7735_RD_WRITE        1

#define ST7735_RD_LOW          0
#define ST7735_RD_HIGH         1

#define ST7735_RST_ENABLE      0
#define ST7735_RST_DISABLE     1

#define ST7735_BACKLIGHT_ON    1
#define ST7735_BACKLIGHT_OFF   0

//*****************************************************************************
//
//! \addtogroup ST7735S_Registers ST7735S Registers
//
//*****************************************************************************

#define ST7735_DEVICE_CODE_READ_REG                                          \
0x00
#define ST7735_SWRESET_REG      0x01
#define ST7735_RDDID_REG        0x04
#define ST7735_RDDST_REG        0x09
#define ST7735_RDDPM_REG        0x0A
#define ST7735_RDDMADCTL_REG    0x0B
#define ST7735_RDDCOLMOD_REG    0x0C
#define ST7735_RDDIM_REG        0x0D
#define ST7735_RDDSM_REG        0x0E

#define ST7735_SLPIN_REG        0x10
#define ST7735_SLPOUT_REG       0x11
#define ST7735_PTLON_REG        0x12
#define ST7735_NORON_REG        0x13

#define ST7735_INVOFF_REG       0x20
#define ST7735_INVON_REG        0x21
#define ST7735_GAMSET_REG       0x26
#define ST7735_DISPOFF_REG      0x28
#define ST7735_DISPON_REG       0x29
#define ST7735_CASET_REG        0x2A
#define ST7735_RASET_REG        0x2B
#define ST7735_RAMWR_REG        0x2C
#define ST7735_RGBSET_REG       0x2D
#define ST7735_RAMRD_REG        0x2E

#define ST7735_PTLAR_REG        0x30
#define ST7735_TEOFF_REG        0x34
#define ST7735_TEON_REG         0x35
#define ST7735_MADCTL_REG       0x36
#define ST7735_IDMOFF_REG       0x38
#define ST7735_IDMON_REG        0x39
#define ST7735_COLMOD_REG       0x3A

#define ST7735_FRMCTR1_REG      0xB1
#define ST7735_FRMCTR2_REG      0xB2
#define ST7735_FRMCTR3_REG      0xB3
#define ST7735_INVCTR_REG       0xB4
#define ST7735_DISSET5_REG      0xB6

#define ST7735_PWCTR1_REG       0xC0
#define ST7735_PWCTR2_REG       0xC1
#define ST7735_PWCTR3_REG       0xC2
#define ST7735_PWCTR4_REG       0xC3
#define ST7735_PWCTR5_REG       0xC4
#define ST7735_VMCTR1_REG       0xC5
#define ST7735_VMOFCTR_REG      0xC7

#define ST7735_WRID2_REG        0xD1
#define ST7735_WRID3_REG        0xD2
#define ST7735_NVCTR1_REG       0xD9
#define ST7735_RDID1_REG        0xDA
#define ST7735_RDID2_REG        0xDB
#define ST7735_RDID3_REG        0xDC
#define ST7735_NVCTR2_REG       0xDE
#define ST7735_NVCTR3_REG       0xDF

#define ST7735_GAMCTRP1_REG     0xE0
#define ST7735_GAMCTRN1_REG     0xE1

#define ST7735_EXTCTRL_REG      0xF0
#define ST7735_PWCTR6_REG       0xFC
#define ST7735_VCOM4L_REG       0xFF

//*****************************************************************************
//
//! \addtogroup ST7735S_Exported_APIs  ST7735S Driver APIs
//! \brief API Refrence of ST7735S Driver.
//
//*****************************************************************************
extern unsigned int Trans_mode;
extern unsigned int Shape_mode;
extern unsigned int Shape_mode_color;

extern void Lcd_Write_Cmd(unsigned short cmd);
extern void Lcd_Write_Two_Byte_Data(unsigned short data);
extern void Lcd_Config_Read(unsigned char read_cmd);
extern void Lcd_Config_Write_Line(void);
extern unsigned char Lcd_Read_One_Byte_Data(void);

extern void Lcd_Set_Cur_Pos(unsigned short st_x, unsigned short end_x,
														unsigned short st_y, unsigned short end_y);
extern void Lcd_Draw_One_Pixel(unsigned short x, unsigned short y, unsigned short color);
extern unsigned short Lcd_Read_One_Pixel(unsigned short x, unsigned short y);

extern void Lcd_Init(void);
extern unsigned long Lcd_Read_State(void);
extern void Lcd_State_Print(void);
extern void Lcd_Put_Pixel(int x,int y,int color);

extern void Lcd_Set_Trans_Mode(int mode);
extern void Lcd_Set_Shape_Mode(int mode, int color);

extern void Lcd_Draw_Line(int x1,int y1,int x2,int y2,int color);
extern void Lcd_Draw_Hline(int y, int x1, int x2, int color);
extern void Lcd_Draw_Vline(int x, int y1, int y2, int color);
extern void Lcd_Draw_Rect(int x1, int y1, int x2, int y2, int color);
extern void Lcd_Draw_Bar(int x1, int y1, int x2, int y2, int color);
extern void Lcd_Clr_Screen(int color);
extern void Lcd_Puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy);
extern void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);

extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);
extern void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned short int *fp);
extern void Lcd_Draw_Image(int x, int y, const unsigned short int *fp, int width, int height);