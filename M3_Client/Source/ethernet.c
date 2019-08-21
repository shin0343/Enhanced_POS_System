#include "stm32f10x.h"
#include "socket.h"	// Just include one header for WIZCHIP
#include "udf.h"
#include "macro.h"

/* Private typedef -----------------------------------------------------------*/

typedef struct _CONFIG_TYPE_DEF
{
	unsigned short port;
	unsigned char destip[4];
}CHCONFIG_TYPE_DEF;


/* Private define ------------------------------------------------------------*/

/*
 @brief Peer IP register address
*/
#define Sn_DIPR0(ch)                    (0x000C08 + (ch<<5))
#define Sn_DIPR1(ch)                    (0x000D08 + (ch<<5))
#define Sn_DIPR2(ch)                    (0x000E08 + (ch<<5))
#define Sn_DIPR3(ch)                    (0x000F08 + (ch<<5))
/**
 @brief Peer port register address
 */
#define Sn_DPORT0(ch)                   (0x001008 + (ch<<5))
#define Sn_DPORT1(ch)                   (0x001108 + (ch<<5))


#define ETH_DATA_BUF_SIZE   (512) 
#define ADD_IFC_DEBUG (1)
#define MAX_SOCK_NUM (8)
#define TX_RX_MAX_BUF_SIZE (2048)


#define tick_second 1

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Reset_W5500(void);
void W5500_CS(unsigned char val);

void W5500_IRQ_Init(void);

void Ethernet_Config(void);

void Ethernet_Init(void);
void Ethernet_Test(void);
unsigned char SPI2_SendByte(unsigned char byte);

unsigned int time_return(void);
void Stringto_Ipaddr(unsigned char*ip, char*str_ip);
/* Private variables ---------------------------------------------------------*/
unsigned char ETH_Data_buff[ ETH_DATA_BUF_SIZE ];

unsigned char TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
unsigned char RX_BUF[TX_RX_MAX_BUF_SIZE]; // RX Buffer for applications

unsigned char ch_status[MAX_SOCK_NUM] = { 0, }; /** 0:close, 1:ready, 2:connected */

unsigned short any_port = 1000;

unsigned int presentTime;

unsigned int my_time;

extern __IO uint32_t Timer2_Counter;

///////////////////////////////////
// Default Network Configuration //
///////////////////////////////////
wiz_NetInfo gWIZNETINFO = 
{ 
	.mac = {0x00, 0x08, 0xdc,0x1d, 0x4c, 0x53},
	.ip = {192, 168, 0, 100},
	.sn = {255,255,255,0},
	.gw = {192, 168, 0, 1},
	.dns = {0,0,0,0},
	.dhcp = NETINFO_STATIC
};

CHCONFIG_TYPE_DEF Chconfig_Type_Def={{3300},{192,168,0,0,}};

wiz_NetTimeout Net_timout={0x02,0x7D0};
uint8_t Dest_IP[4] = {192, 168, 0, 4}; //DST_IP Address
uint16_t Dest_PORT = 25000;

void Ethernet_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// clock configure SPI2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	
	
	
	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// W_CSn, W_RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_4 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
	
	W5500_CS(CS_HIGH);
	
	/* SPI2 Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	/* Enable SPI2 */
	SPI_Cmd(SPI2, ENABLE);
	
	Reset_W5500(); // reset
	
	TIM_Delay_ms(1000);
	
	Ethernet_Config(); // configure
}


void W5500_CS(unsigned char val)
{
	if (val == CS_LOW) 
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		
	}
	else if (val == CS_HIGH)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
}

void Reset_W5500(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	//	Delay_us(2);
	TIM_Delay_ms(400);	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	//	Delay_ms(150);  
	TIM_Delay_ms(400);
}

char W5500_Get_Rdy(void)
{
	//	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
}


void Ethernet_Config(void)
{
	
	unsigned char tmpstr[6];

	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
	
	ctlnetwork(CN_SET_TIMEOUT, (void*)&Net_timout);
	ctlnetwork(CN_GET_TIMEOUT, (void*)&Net_timout);
	
	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	Uart1_Printf("=== %s NET CONF ===\n",(char*)tmpstr);
	Uart1_Printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",gWIZNETINFO.mac[0],
				gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],
				gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	
	//Destination IP address for TCP Client
	Chconfig_Type_Def.destip[0] = Dest_IP[0]; Chconfig_Type_Def.destip[1] = Dest_IP[1];
	Chconfig_Type_Def.destip[2] = Dest_IP[2]; Chconfig_Type_Def.destip[3] = Dest_IP[3];
	Chconfig_Type_Def.port = Dest_PORT;
	
	Uart1_Printf("Version :%X\n", getVERSIONR());
	Uart1_Printf("======================\n");
	
}



void W5500_IRQ_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Configure Key Button GPIO Pin as input floating (Key Button EXTI Line) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Connect Key Button EXTI Line to Key Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	
	/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}


unsigned char SPI2_SendByte(unsigned char byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}
	SPI_I2S_SendData(SPI2, byte);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){}
	return (unsigned char)SPI_I2S_ReceiveData(SPI2);
}

extern volatile int Uart1_Rx_In;
extern volatile int Uart1_Rx_Data;

unsigned short RSR_len;
unsigned short received_len;
unsigned char * txbuf = TX_BUF;
unsigned char * rxbuf = RX_BUF;
int index=0;
unsigned char ret;

void loopback_tcpc(char s, char *str_ip, unsigned short port)
{	
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:                 /* if connection is established */
		if(ch_status[s]==1)
		{
			Uart1_Printf("\r\n%d : Connected",s);
			Uart1_Printf("\r\n[send]: ");
			ch_status[s] = 2;
		}
		
		if(Uart1_Rx_In)
		{
			Uart1_Rx_In = 0;
			
			if(Uart1_Rx_Data!='\r')
			{
				txbuf[index++]=Uart1_Rx_Data;
				Uart1_Printf("%c",Uart1_Rx_Data);
			}
			else
			{
				txbuf[index]='\0';
				send(s, txbuf, index);
				Uart1_Printf("\n[send]: ");
				index=0;
			}
		}
		
		if ((RSR_len = getSn_RX_RSR(s)) > 0)        /* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */                                                                           /* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, rxbuf, RSR_len);      /* read the received data */
			rxbuf[RSR_len]='\0';
			Uart1_Printf("\r[recv]: %s\n",rxbuf);
		}
		
		break;
	case SOCK_CLOSE_WAIT:                                 /* If the client request to close */
		Uart1_Printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0)         /* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;   /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, rxbuf, RSR_len);         /* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */
		if(!ch_status[s])
		{
			Uart1_Printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			ch_status[s] = 1;
		}
		if(socket(s, Sn_MR_TCP, any_port++, 0x00) != s)    		/* reinitialize the socket */
		{
			Uart1_Printf("\r\n%d : Fail to create socket.",s);
			ch_status[s] = 0;
		}
		break;
	case SOCK_INIT:     /* if a socket is initiated */
		if(time_return() - presentTime >= (tick_second * 3)) 
		{  
			/* For TCP client's connection request delay : 3 sec */
			Chconfig_Type_Def.port = port;
			Stringto_Ipaddr(Chconfig_Type_Def.destip, str_ip);
			connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port); /* Try to connect to TCP server(Socket, DestIP, DestPort) */
			presentTime = time_return();
		}
		break;
	default:
		break;
	}
}

void Stringto_Ipaddr(unsigned char*ip, char*str_ip)
{
	int i;
	int str_offset=0;
	unsigned char temp1;
	unsigned char temp2=0;
	unsigned char digit=0;

	for(i=0;i<4;)
	{
		temp1 = *(str_ip + str_offset);		
		if( temp1 == '\0')
		{
			ip[i] = temp2;
			break;
		}
		else if( temp1 != '.')
		{
			if (digit == 0)
			{
				temp2 = temp1-'0';
				digit++;
			}
			else if(digit == 1)
			{
				temp2 *= 10;
				temp2 += (temp1-'0');
				digit++;
			}
			else
			{
				temp2 *= 10;
				temp2 += (temp1-'0');
			}
			str_offset++;
		}
		else
		{
			ip[i++] = temp2;
			
			temp2 = 0;
			digit = 0;
			str_offset++;
		}
	}
}

/*******************************************************************************
* Function Name  : time_return
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

unsigned int time_return(void)
{
    return my_time;
}

void Timer2_ISR(void)
{
  if (Timer2_Counter++ > 1000) { // 1m x 1000 = 1sec
    Timer2_Counter = 0;
    my_time++;
  }
}

