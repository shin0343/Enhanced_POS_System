#include "device_driver.h"
#define BASE  (500) //msec
#define SERVER_IP	"192.168.0.4"
#define	SERVER_PORT	25000


RCC_ClocksTypeDef RCC_ClockFreq;

extern void WizFi250_Serial_Input_Mode(void);
extern volatile int wifi_mode;
extern int Key_Value;

extern volatile int ADC1_value;
extern volatile int ADC1_flag;

extern volatile int TIM4_Expired;
extern int recv_flag;
extern volatile int Uart2_Cmd_In;
extern volatile unsigned char Uart2_Rx_Buf[200];


int menu[5];
char arr[5] = {0,'A','C',0,'B'};
enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};

int main(void)
{
	float AD_value;
	char socket=0;
  signed char x, y, z;
	int isCustomerHere=0;
	
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	Key_EXTI_Init();
	TIM_Config();
	Uart1_Init(115200);
	Uart2_Init(115200);
	
	Lcd_Init();
	WiFi_Init();
	
	Factory_Default();
	wifi_mode = Run_Server();
  I2C_Setup();
	ADC_Configuration();
  TIM4_Repeat_Interrupt_Enable(1, 1000);
	
	for(;;)
	{
		if(TIM4_Expired)
    {
      TIM4_Expired = 0;
			
			//조도센서
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			if(ADC1_flag)
			{
				if(ADC1_value > 2450)
				{					
					Lcd_Clr_Screen(0);
				}
				else
				{
					Uart1_Printf("Customer Arrived!\n");
					
					Lcd_Printf(0 ,0,GREEN,RED,1,1,"A : %d",menu[1]);
					Lcd_Printf(0,20,GREEN,RED,1,1,"B : %d",menu[4]);
					Lcd_Printf(0,40,GREEN,RED,1,1,"C : %d",menu[2]);
	
				}
				
				ADC1_flag = 0;   
			}
			
			//가속도센서
      x = I2C_getbyte(0x38,0x29); //X-axis to read..
      y = I2C_getbyte(0x38,0x2b); //Y-Axis to read..
      z = I2C_getbyte(0x38,0x2d); //Z Axis to read..
			
      //Uart1_Printf("X: %3d, Y: %3d, Z: %3d\n",x, y, z);
			if(z < 0)
			{
				//send to server
				Uart2_Send_String("POS Flipped!!\n");
				Uart1_Printf("POS Flipped!!\n");
				
				//buzzer
				TIM3_Buzzer_Beep(C1,N4);	
				TIM3_Buzzer_Init();
			}
			
    }
		
		if(Key_Value){
			menu[Key_Value]++;
			
			
			Lcd_Printf(0 ,0,GREEN,RED,1,1,"A : %d",menu[1]);
			Lcd_Printf(0,20,GREEN,RED,1,1,"B : %d",menu[4]);
			Lcd_Printf(0,40,GREEN,RED,1,1,"C : %d",menu[2]);
			
			
			Uart1_Printf("%c\n",arr[Key_Value]);
			Uart2_Send_Byte(arr[Key_Value]);
			
			Key_Value = 0;
		}
		
		loopback_tcpc(socket,SERVER_IP,SERVER_PORT);
		/*if(Uart2_Cmd_In)
		{
			Uart2_Cmd_In = 0;
			Uart1_Printf("\r[recv]: %s\n: ",Uart2_Rx_Buf);			
		}*/
//		if(recv_flag)
//		{
//			menu[1]=0;
//			menu[2]=0;
//			menu[4]=0;
//			recv_flag=0;
//		}
	}
	
}
