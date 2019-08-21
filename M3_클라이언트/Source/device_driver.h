#include "stm32f10x.h"
#include "macro.h"
#include "option.h"
#include "w5500.h"
#include "adc.h"

// Led.c

extern void LED_Init(void);
extern void LED_Display(unsigned int num);
extern void LED_All_On(void);
extern void LED_All_Off(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern int Key_Wait_Key_Pressed(void);
extern void Key_ISR_Enable(int en);

// Uart.c

extern void Uart1_Init(int baud);
extern void Uart2_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(char *pt);
extern void Uart1_Printf(char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);
extern void Uart1_Get_String(char *string);
extern int Uart1_Get_Int_Num(void);
extern void Uart1_RX_Interrupt_Enable(int en);
extern void Uart1_Wait_for_TX_Complete(void);
extern void Uart2_Send_String(unsigned char *pt);


// SysTick.c

extern void SysTick_Run(unsigned int msec);
extern int SysTick_Check_Timeout(void);
extern unsigned int SysTick_Get_Time(void);
extern unsigned int SysTick_Get_Load_Time(void);
extern void SysTick_Stop(void);
extern void SysTick_OS_Tick(unsigned int msec);

// Timer.c
void TIM_Config(void);
extern void TIM2_Delay(int time);
extern void TIM2_Stopwatch_Start(void);
extern unsigned int TIM2_Stopwatch_Stop(void);
extern void TIM4_Repeat(int time);
extern int TIM4_Check_Timeout(void);
extern void TIM4_Stop(void);
extern void TIM4_Change_Value(int time);
extern void TIM3_Buzzer_Init(void);
extern void TIM3_Buzzer_Beep(int tone, int duration);
extern void TIM3_Buzzer_Stop(void);
extern void TIM4_Repeat_Interrupt_Enable(int en, int time);
extern void TIM_Delay_ms(unsigned int time);


// DMA.c

struct _DMA_CCR_ST
{
  unsigned int temp : 4;
  unsigned int dir : 1;
  unsigned int circ : 1;
  unsigned int pinc : 1;
  unsigned int minc : 1;
  unsigned int psize : 2;
  unsigned int msize : 2;
  unsigned int pl : 2;
  unsigned int m2m : 1;
};

union _DMA_CCR_UNI
{
  struct _DMA_CCR_ST st_data;
  unsigned int ui_data;
};

#define DMA_STATUS_READY			(0)
#define DMA_STATUS_RUN				(1)
#define DMA_STATUS_COMPLETE			(2)
#define DMA_STATUS_HALFTRANSFERED	        (3)
#define DMA_STATUS_ERROR			(4)

#define	DMA_SIZE_8BIT				(0x0)
#define	DMA_SIZE_16BIT				(0x1)
#define	DMA_SIZE_32BIT				(0x2)

#define DMA_PL_LOW				(0x0)
#define DMA_PL_MEDIUM				(0x1)
#define DMA_PL_HIGH				(0x2)
#define DMA_PL_VERYHIGH				(0x3)

#define DMA_DIR_P2M				(0x0)
#define DMA_DIR_M2P				(0x1)

#define DMA_TRIGGER_HW				(0x0)
#define DMA_TRIGGER_SW				(0x1)

#define DMA_ADDR_FIX				(0x0)
#define DMA_ADDR_INC				(0x1)

#define DMA_CIRCULAR_DIS			(0x0)
#define DMA_CIRCULAR_EN				(0x1)

extern void DMA1_Ch_Init(int ch, void* maddr, void* paddr, union _DMA_CCR_UNI ccr, unsigned int tr_cnt);
extern unsigned int DMA1_Get_Status(int ch);
extern void DMA1_Start(int ch);
extern void DMA1_Stop(int ch);
extern void DMA1_ISR_Enable(int ch, unsigned int tei, unsigned int hti, unsigned int tci);
extern void DMA1_Set_Status(int ch, unsigned int status);

// BKT.c

extern void BKP_Register_Write(int no, int data);
extern int BKP_Register_Read(int no);
extern void BKP_Reset(void);
extern void BKP_Init(void);
extern void BKP_Tamper_Enable(int en, int low_high);

// IWDG.c

extern void IWDG_Start(int ms);
extern void IWDG_Reset(void);

// WWDG.c

extern void WWDG_Start(int low, int high);
extern void WWDG_Reset(int low, int high);
extern void WWDG_IRQ_Enable(int low, int high);

// rtc.c

typedef struct{
  int hour;
  int min;
  int sec;
} RTC_DATA;

extern void BKP_RTC_Reset(void);
extern void RTC_Init(void);
extern void RTC_Config_Start(void);
extern void RTC_Config_End(void);
extern void RTC_Set_Time(RTC_DATA* rtcdata);
extern void RTC_Get_Time(RTC_DATA* rtcdata);
extern void RTC_Set_Alarm(RTC_DATA* rtcdata);
extern void RTC_ISR_Enable(int overflow, int alarm, int sec);
extern void RTC_Alarm_ISR_Enable(int overflow, int alarm, int sec, int irq);

// power.c

extern void PWR_Normal_Mode(void);
extern void PWR_Sleep_Mode(int sev_on_pend, int sleep_on_exit, int wfe_wfi);
extern void PWR_Stop_Mode(int low_power, int wfe_wfi);
extern void PWR_Standby_Mode(int event_irq);
extern void PWR_WKUP_Enable(int en);

// adc.c
extern void ADC1_Init(void);
extern void ADC1_ISR_Enable(int en,int ch);
extern void ADC1_ISR_Start(void);
extern void ADC_Configuration(void);

// i2c.c
extern void I2C_Setup (void);
extern unsigned char I2C_Read (int ack);
extern unsigned char I2C_getbyte(unsigned char address, unsigned char cmd);
extern void I2C_putbyte(unsigned char address, unsigned char cmd, unsigned char data);

// ethernet.c
extern void Ethernet_Init(void);
extern void Ethernet_Config(void);
extern void W5500_IRQ_Init(void);
extern void loopback_tcps(char s, unsigned short port);
extern void loopback_tcpc(char s, char *str_ip,unsigned short port);
extern void loopback_udp(char s, unsigned short port);
extern unsigned char * rxbuf;
extern unsigned short RSR_len;
extern unsigned short received_len;
#define TX_RX_MAX_BUF_SIZE (2048)


// wifi_ex.c
extern void Disp_Help(void);
extern void Factory_Default(void);
extern void WiFi_Init(void);
extern void SPI2_Receive(void);
extern unsigned char SPI2_SendByte(unsigned char byte);
extern void SPI2_Send_String(unsigned char *pt);
extern int Check_Cmd(int n);
extern void WizFi250_Serial_Data_Mode(void);
extern int	Run_Server(void);

#define WIFI_IFC_UART_MODE	(0)
#define WIFI_IFC_SPI_MODE	(1)

#define  W_Func_Low()   GPIO_ResetBits(GPIOB, GPIO_Pin_5);
#define  W_Func_High()  GPIO_SetBits(GPIOB, GPIO_Pin_5);

#define  W_Rst_Low()    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define  W_Rst_High()   GPIO_SetBits(GPIOA, GPIO_Pin_8);

#define  W_CS_Low()    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
#define  W_CS_High()   GPIO_SetBits(GPIOB, GPIO_Pin_9);


// system_stm32f10x.c

extern void Set_Clock_72MHz_HSE(void);
