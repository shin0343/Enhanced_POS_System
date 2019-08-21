// Bit Operation

#define	Macro_Set_Bit(dest, pos)					((dest) |=  ((unsigned)0x1<<(pos)))
#define	Macro_Clear_Bit(dest, pos)					((dest) &= ~((unsigned)0x1<<(pos)))
#define	Macro_Invert_Bit(dest, pos)				((dest) ^=  ((unsigned)0x1<<(pos)))

#define	Macro_Clear_Area(dest, bits, pos)			((dest) &= ~(((unsigned)bits)<<(pos)))
#define	Macro_Set_Area(dest, bits, pos)			((dest) |=  (((unsigned)bits)<<(pos)))
#define	Macro_Invert_Area(dest, bits, pos)			((dest) ^=  (((unsigned)bits)<<(pos)))

#define	Macro_Write_Block(dest, bits, data, pos)	((dest) = (((unsigned)dest) & ~(((unsigned)bits)<<(pos))) | (((unsigned)data)<<(pos)))
#define Macro_Extract_Area(dest, bits, pos)		((((unsigned)dest)>>(pos)) & (bits))

#define Macro_Check_Bit_Set(dest, pos)				((((unsigned)dest)>>(pos)) & 0x1)
#define Macro_Check_Bit_Clear(dest, pos)			(!((((unsigned)dest)>>(pos)) & 0x1))

//Bit Banding

#define Peri_Bit(reg, bit) 	(*(volatile unsigned long *)(PERIPH_BB_BASE+((unsigned int)&(reg) - PERIPH_BASE)*32+(unsigned int)(bit)*4))
#define Mem_Bit(mem, bit) 	(*(volatile unsigned long *)(SRAM_BB_BASE+((unsigned int)(mem) - SRAM_BASE)*32+(unsigned int)(bit)*4))

#define Peri_Set_Bit(reg, bit) 	(*(volatile unsigned long *)(PERIPH_BB_BASE+((unsigned int)&(reg) - PERIPH_BASE)*32+(unsigned int)(bit)*4) = 1)
#define Peri_Clear_Bit(reg, bit) 	(*(volatile unsigned long *)(PERIPH_BB_BASE+((unsigned int)&(reg) - PERIPH_BASE)*32+(unsigned int)(bit)*4) = 0)
#define Mem_Set_Bit(mem, bit) 	(*(volatile unsigned long *)(SRAM_BB_BASE+((unsigned int)(mem) - SRAM_BASE)*32+(unsigned int)(bit)*4) = 1)
#define Mem_Clear_Bit(mem, bit) 	(*(volatile unsigned long *)(SRAM_BB_BASE+((unsigned int)(mem) - SRAM_BASE)*32+(unsigned int)(bit)*4) = 0)

#define Peri_Check_Bit_Set(reg, bit) 	(*(volatile unsigned long *)(PERIPH_BB_BASE+((unsigned int)&(reg) - PERIPH_BASE)*32+(unsigned int)(bit)*4))
#define Peri_Check_Bit_Clear(reg, bit) 	(!*(volatile unsigned long *)(PERIPH_BB_BASE+((unsigned int)&(reg) - PERIPH_BASE)*32+(unsigned int)(bit)*4))
#define Mem_Check_Bit_Set(mem, bit) 	(*(volatile unsigned long *)(SRAM_BB_BASE+((unsigned int)(mem) - SRAM_BASE)*32+(unsigned int)(bit)*4))
#define Mem_Check_Bit_Clear(mem, bit) 	(!*(volatile unsigned long *)(SRAM_BB_BASE+((unsigned int)(mem) - SRAM_BASE)*32+(unsigned int)(bit)*4))

// Reset, Set Register

#define GPIO_Set_Bit(port, pos)                       (GPIO##port->BSRR = 0x1u<<(pos))
#define GPIO_Clear_Bit(port, pos)                     (GPIO##port->BRR = 0x1u<<(pos))

#define GPIO_Set_Area(port, bits, pos)                (GPIO##port->BSRR = (unsigned int)(bits)<<(pos))
#define GPIO_Clear_Area(port, bits, pos)              (GPIO##port->BRR = (unsigned int)(bits)<<(pos))
#define GPIO_Write_Block(port, bits, data, pos)       (GPIO##port->BSRR = ((unsigned int)(bits)<<(pos+16u))+((unsigned int)(data)<<(pos)))

#define GPIO_Invert_Bit(port, pos)                    (GPIO##port->ODR ^=  (0x1u<<(pos)))
#define GPIO_Invert_Area(port, bits, pos)             (GPIO##port->ODR ^=  (((unsigned)(bits))<<(pos)))
#define GPIO_Extract_Area(port, bits, pos)            (((GPIO##port->IDR)>>(pos)) & (bits))
#define GPIO_Read(port)                               (GPIO##port->IDR)

#define GPIO_Check_Bit_Set(port, pos)	              (((GPIO##port->IDR)>>(pos)) & 0x1u)
#define GPIO_Check_Bit_Clear(port, pos)	              (!(((GPIO##port->IDR)>>(pos)) & 0x1u))

#define GPIO_Lock_Bit(port, pos)                      {volatile int a = (GPIO##port->LCKR & 0xffff)|(1u<<(pos)); GPIO##port->LCKR = a|0x10000u; GPIO##port->LCKR = a; GPIO##port->LCKR = a|0x10000u; a = GPIO##port->LCKR; a = GPIO##port->LCKR;}
#define GPIO_Lock_Multi(port, data)                   {volatile int a = (GPIO##port->LCKR & 0xffff)|((data) & 0xffff); GPIO##port->LCKR = a|0x10000u; GPIO##port->LCKR = a; GPIO##port->LCKR = a|0x10000u; a = GPIO##port->LCKR; a = GPIO##port->LCKR;}

