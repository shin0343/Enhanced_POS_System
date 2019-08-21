#include "stm32f10x.h"
#include "macro.h"
#include "option.h"

extern void Uart1_Printf(char *fmt,...);


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
void TIM2_Init(void);
void TIM4_Init(void);
void TIM3_Buzzer_Init();

/*
TIM2 : interrupt 사용 (LED, 기타 플래그 )
TIM3 : 기타 딜레이 함수에 사용
TIM4 : 가속도센서 딜레이 추가에 사용(자간딜레이)
*/
void TIM_Config(void)
{
/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM2_Init();
	TIM3_Buzzer_Init();
	TIM4_Init();
}

void TIM2_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	
  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	

	
  /* ---------------------------------------------------------------------------
	TIM2 Configuration: TIM_CounterMode_Down:
	TIM2CLK = 72 MHz, Prescaler = 7199, TIM2 counter clock = 10 KHz, Period = 9
  ----------------------------------------------------------------------------*/
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
		/* Compute the prescaler value */
//	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 10000) - 1;
	PrescalerValue = (uint16_t) ( (SystemCoreClock) / 10000) - 1;
	
	
	/* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	 /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
	
  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);
	
 
}


void TIM4_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	
  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
	
  /* ---------------------------------------------------------------------------
	TIM4 Configuration: TIM_CounterMode_Down:
	TIM4 CLK = 72 MHz, Prescaler = 7199, TIM2 counter clock = 10 KHz, Period = 9
  ----------------------------------------------------------------------------*/
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( (SystemCoreClock) / 10000) - 1;
	
	
	/* Prescaler configuration */
  TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);
	
  /* TIM IT enable */
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

/* Delay Time Extended */
#define TIM2_TICK         (20) // usec
#define TIM2_FREQ 	  (1000000/TIM2_TICK) // Hz
#define TIME2_PLS_OF_1ms  (1000/TIM2_TICK)
#define TIM2_MAX	  (0xffffu)

void TIM2_Delay(int time)
{
  int i;
  unsigned int t = TIME2_PLS_OF_1ms * time;
  
  Peri_Set_Bit(RCC->APB1ENR, 0);
  
  TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
  TIM2->CR1 = (1<<4)|(1<<3);
  TIM2->ARR = 0xffff;
  Peri_Set_Bit(TIM2->EGR,0);
  Peri_Set_Bit(TIM2->DIER, 0);
    
  for(i=0; i<(t/0xffff); i++)
  {
    Peri_Set_Bit(TIM2->EGR,0);    
    Peri_Clear_Bit(TIM2->SR, 0);
    Peri_Set_Bit(TIM2->CR1, 0);
    while(Macro_Check_Bit_Clear(TIM2->SR, 0));
  }
  
  TIM2->ARR = t % 0xffff;
  Peri_Set_Bit(TIM2->EGR,0);      
  Peri_Clear_Bit(TIM2->SR, 0);
  Peri_Set_Bit(TIM2->CR1, 0);
  while (Peri_Check_Bit_Clear(TIM2->SR, 0));
    
  Peri_Clear_Bit(TIM2->CR1, 0);
  Peri_Clear_Bit(TIM2->DIER, 0);
  Peri_Clear_Bit(RCC->APB1ENR, 0);
}

#define TIM3_CLK    72000000

#define TIM3_TICK_us   100               // 100 ns
#define TIM3_FREQ_us  (1000000000/TIM3_TICK_us)
#define TIM3_PLS_OF_1us	(1000/TIM3_TICK_us)

#define TIM3_TICK_ms   20                      // usec
#define TIM3_FREQ_ms  (1000000/TIM3_TICK_ms)
#define TIM3_PLS_OF_1ms	(1000/TIM3_TICK_ms)


#define TIM3_FREQ 	  (8000000) 	      // Hz
#define TIM3_TICK	  (1000000/TIM3_FREQ) // usec
#define TIME3_PLS_OF_1ms  (1000/TIM3_TICK)

const static unsigned int Tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};

void TIM3_Buzzer_Init()
{
  Peri_Set_Bit(RCC->APB1ENR, 1);
  Peri_Set_Bit(RCC->APB2ENR, 3);
  Macro_Write_Block(GPIOB->CRL,0xf,0xb,0);
  Macro_Write_Block(TIM3->CCMR2,0x7,0x6,4);
  TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Buzzer_Beep(int tone, int duration)
{
	volatile int i = 0;
  TIM3->PSC = (unsigned int)(TIMXCLK/(double)TIM3_FREQ + 0.5)-1;
  TIM3->ARR = TIM3_FREQ/Tone_value[tone]-1;
  TIM3->CCR3 = TIM3->ARR/2;
  TIM3->CR1 = (1<<4)|(0<<3)|(0<<1);
  
  Peri_Set_Bit(TIM3->EGR,0);
  Peri_Set_Bit(TIM3->CR1,0);
  TIM2_Delay(duration);
  Peri_Clear_Bit(TIM3->CR1,0);
}

void TIM3_Buzzer_Stop(void)
{
  Peri_Clear_Bit(TIM3->CR1, 0);
  Peri_Clear_Bit(TIM3->DIER, 0);
  Peri_Clear_Bit(RCC->APB1ENR, 1);
  Peri_Clear_Bit(RCC->APB2ENR, 3);
}

void TIM_Delay_us(unsigned int time)
{
    int i;
	unsigned int t = TIM3_PLS_OF_1us * time;
	
    //TIM3 Enable
    Peri_Set_Bit(RCC->APB1ENR, 1);
    
    TIM3->PSC = (unsigned int)(TIM3_CLK/(double)TIM3_FREQ_us + 0.5)-1;
    TIM3->CR1 = (1<<4)+(1<<3)+(1<<2);    
    
    
    ////////////
    Peri_Set_Bit(TIM3->EGR,0);
	Peri_Set_Bit(TIM3->DIER, 0);

	for(i=0; i<(t/0xffff); i++)
	{
		TIM3->CNT = 0xffff;
		Peri_Clear_Bit(TIM3->SR, 0);
		Peri_Set_Bit(TIM3->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM3->SR, 0));
	}

	TIM3->CNT = t% 0xffff;
	Peri_Clear_Bit(TIM3->SR, 0);
	Peri_Set_Bit(TIM3->CR1, 0);
	while (Peri_Check_Bit_Clear(TIM3->SR, 0));


	Peri_Clear_Bit(TIM3->CR1, 0);
	Peri_Clear_Bit(TIM3->DIER, 0);
	Peri_Clear_Bit(RCC->APB1ENR, 1);
 
}

void TIM_Delay_ms(unsigned int time)
{
    volatile int i;
	unsigned int t = TIM3_PLS_OF_1ms * time;
	
    //TIM3 Enable
    Peri_Set_Bit(RCC->APB1ENR, 1);
    
    TIM3->PSC = (unsigned int)(TIM3_CLK/(double)TIM3_FREQ_ms + 0.5)-1;
    TIM3->CR1 = (1<<4)+(1<<3)+(1<<2); 
    
    
    ////////////
    Peri_Set_Bit(TIM3->EGR,0);
	Peri_Set_Bit(TIM3->DIER, 0);

	for(i=0; i<(t/0xffff); i++)
	{
		TIM3->CNT = 0xffff;
		Peri_Clear_Bit(TIM3->SR, 0);
		Peri_Set_Bit(TIM3->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM3->SR, 0));
	}

	TIM3->CNT = t% 0xffff;
	Peri_Clear_Bit(TIM3->SR, 0);
	Peri_Set_Bit(TIM3->CR1, 0);
	while (Peri_Check_Bit_Clear(TIM3->SR, 0));


	Peri_Clear_Bit(TIM3->CR1, 0);
	Peri_Clear_Bit(TIM3->DIER, 0);
	Peri_Clear_Bit(RCC->APB1ENR, 1);
 
}

void TIM_Stopwatch_Start(void)
{
	Peri_Set_Bit(RCC->APB1ENR, 1);

	TIM3->CR1 = (1<<4)+(1<<3);
	TIM3->PSC = (unsigned int)(TIM3_CLK/(double)TIM3_FREQ_ms + 0.5)-1;
	TIM3->CNT = 0xffffu;
	Peri_Set_Bit(TIM3->EGR,0);
	Peri_Set_Bit(TIM3->CR1, 0);
}

unsigned int TIM_Stopwatch_Stop(void)
{
	unsigned int time;

	Peri_Clear_Bit(TIM3->CR1, 0);
	time = (0xffffu - TIM3->CNT) * TIM3_TICK_ms;
	Peri_Clear_Bit(RCC->APB1ENR, 1);
	return time;
}

#define TIM4_TICK	  (20) // usec
#define TIM4_FREQ 	  (1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  (1000/TIM4_TICK)
#define TIM4_MAX	  (0xffffu)

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
  if(en)
  {
    Peri_Set_Bit(RCC->APB1ENR, 2);
    
    TIM4->CR1 = (1<<4)+(0<<3)+(0<<0);
    TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
    TIM4->ARR = TIME4_PLS_OF_1ms * time;
    
    Peri_Set_Bit(TIM4->EGR,0);
    Peri_Set_Bit(TIM4->SR, 0);
    NVIC_ClearPendingIRQ((IRQn_Type)30);
    Peri_Set_Bit(TIM4->DIER, 0);
    NVIC_EnableIRQ((IRQn_Type)30);
    Peri_Set_Bit(TIM4->CR1, 0);
  }
  
  else
  {
    NVIC_DisableIRQ((IRQn_Type)30);
    Peri_Clear_Bit(TIM4->CR1, 0);
    Peri_Clear_Bit(TIM4->DIER, 0);
    Peri_Clear_Bit(RCC->APB1ENR, 2);
  }
}