#include "device_driver.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void I2C_Start (void);
void I2C_Stop (void);
void I2C_Addr (unsigned char adr);
void i2c_write (unsigned char c);
void I2C_Setup (void);
unsigned char I2C_Read (int ack);
unsigned char I2C_getbyte(unsigned char address, unsigned char cmd);
void I2C_putbyte(unsigned char address, unsigned char cmd, unsigned char data);
unsigned short int I2C_getword(unsigned char address, unsigned char cmd);
unsigned int I2C_sr(void);

/*******************************************************************************
* I2C communication status *
* Parameter: *
* Return: status *
* LIS302DL ID : 0x38 * AT SDO(12) LOW
*******************************************************************************/
/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize I2C interface in master mode *
* Parameter: *
* Return: *
*******************************************************************************/
//static __inline 
unsigned int I2C_sr(void)
{
    unsigned int sr;

    sr = I2C1->SR1;
    sr |= I2C1->SR2 << 16;
    return (sr);
}

void I2C_Setup(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	/* Enable clock for I2C1, GPIOB and AFIO */
	RCC->APB2ENR |= (1 << 3) | (1 << 0); //
	RCC->APB1ENR |= (1 << 21);

	/* I2C1 pins remapped, use PB6, PB7 */
	GPIOB->CRL |= 0xFF000000; // Alternate IO 
	I2C1->CR1 = 0x0000;

	/* Configure I2C peripheral */
	I2C1->CR1 = 0x0001; // PE - Peripheral Enable
	I2C1->CR2 = 0x0024; // Freq 36 MHz
	I2C1->CR1 = 0x0000; // PE Disable>TRISE = 0x0025; //Time Rise - program when PE=0
	I2C1->CCR = 0x00B4; // 0x005A = 400 KHz (36MHz / 90) 0x00B4 = 200 KHz
	I2C1->CR1 |= 0x0401;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);

	I2C_putbyte(0x38,0x20,0x47); // Xen=1, Yen=1, Zen=1, Pd = 1(White);
}
/*******************************************************************************
* Generate start condition on I2C bus *
* Parameter: *
* Return: *
*******************************************************************************/

void I2C_Start (void) {
if ( I2C1->CR1 & ( 1 << 10 ) ) {
		I2C1->CR1 &= ~(1 << 10 );
	} 
  I2C1->CR1 |= 0x0100; //start genneration when bus free
  while (!(I2C_sr() & 0x0001));
}

/*******************************************************************************
* Generate stop condition on I2C bus                                           *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void I2C_Stop (void) {

  I2C1->CR1 |= 0x0200;
  while (I2C_sr() & 0x00020000);        /* Wait until BUSY bit reset          */
}


/*******************************************************************************
* Write address on I2C interface                                               *
*   Parameter:    adr:    address to be written                                *
*   Return:                                                                    *
*******************************************************************************/

void I2C_Addr (unsigned char adr) {

  I2C1->DR = adr;
  while (!(I2C_sr() & 0x0002));	//Addr sent 
}


/*******************************************************************************
* Write a byte to I2C interface                                                *
*   Parameter:    c:      data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

void I2C_Write (unsigned char c) {

  I2C1->DR = c;
  while (!(I2C_sr() & 0x00000004));     /* Wait until BTF bit set             */ 
}


/*******************************************************************************
* Read a byte from I2C interface                                               *
*   Parameter:                                                                 *
*   Return:               read data                                            *
*******************************************************************************/

unsigned char I2C_Read (int ack) {

  /* Enable/disable Master acknowledge                                        */
  if (ack) I2C1->CR1 |=  0x0400;
  else     I2C1->CR1 &= ~0x0400;

  while (!(I2C_sr() & 0x00000040));     /* Wait until RxNE bit set            */
  return (I2C1->DR);
}

/******************************************************************************/

unsigned char I2C_getbyte(unsigned char address, unsigned char cmd) {
	unsigned char uc;
	I2C_Start();		  	// Initial Start bit sequence
	I2C_Addr(address);	 	// Address I2C Device. (Base address is Write Address)
        I2C_Write(cmd);			// Transfer Command to I2C Device (Register to be Read) 
	I2C_Start(); 			// Repeated start bit sequence
	I2C_Addr(address+1);	// Address I2C Device. (Base address + 1 is Read Address)
	uc = I2C_Read(0);		// Read 1 byte without Acknowledge
	I2C_Stop();				// Stop I2C transfer
	return( uc );
}

unsigned short int I2C_getword(unsigned char address, unsigned char cmd) {
	unsigned short int uw;
	//unsigned short int uw2;
	I2C_Start();		  	// Initial Start bit sequence
	I2C_Addr(address);	 	// Address I2C Device. (Base address is Write Address)
	I2C_Write(cmd);			// Transfer Command to I2C Device (Register to be Read) 
	I2C_Start(); 			// Repeated start bit sequence
	I2C_Addr(address+1);	// Address I2C Device. (Base address + 1 is Read Address)
	uw = I2C_Read(1) << 8;		// Read MSB  without Acknowledge
	uw |= I2C_Read(0); 			// Read LSB	 with Acknowledge
	I2C_Stop();				// Stop I2C transfer
	return( uw );
}
void I2C_putbyte(unsigned char address, unsigned char cmd, unsigned char data) {
	I2C_Start();		  	// Initial Start bit sequence
	I2C_Addr(address);	 	// Address I2C Device. (Base address is Write Address
	I2C_Write(cmd);			// Transfer Command to I2C Device (Register to be Read) 
	I2C_Write(data);		// Transfer Data to I2C device
	I2C_Stop();				// Stop I2C transfer
}

int I2C_getbytearray(unsigned char address, unsigned char cmd, int number, unsigned char *data) {
	int count;
	I2C_Start();		  	// Initial Start bit sequence
	I2C_Addr(address);	 	// Address I2C Device. (Base address is Write Address)
	I2C_Write(cmd);			// Transfer Command to I2C Device (Register to be Read) 
	I2C_Start(); 			// Repeated start bit sequence
	I2C_Addr(address+1);	// Address I2C Device. (Base address + 1 is Read Address)
	// Read number - 1 bytes with Acknowledge
	for ( count=0; count < number - 2; count++ ) {
		data[count] = I2C_Read(1);		// Read with Acknowledge
	}
	data[count] = I2C_Read(0);		// Last byte without Acknowledge
	I2C_Stop();				// Stop I2C transfer
	return( count+1 );
}