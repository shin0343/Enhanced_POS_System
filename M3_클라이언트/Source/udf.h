


///////////////////////////////////////////
//////////////ERROR CODE///////////////////
///////////////////////////////////////////
#define EC_SUCCESS		(0)
#define EC_FAILED			(1)
#define EC_DATA_FULL	(100)
#define EC_DATA_EMPTY	(101)







#define WIFI_IFC_UART_MODE	(0)
#define WIFI_IFC_SPI_MODE	(1)



// uart.c
void Uart1_Init(int baud);
void Uart1_Printf(char *fmt,...);

void Uart2_Init(int baud);
void Uart2_Printf(char *fmt,...);

// timer.c
void TIM_Config(void);
void TIM_Delay_ms(unsigned int time);