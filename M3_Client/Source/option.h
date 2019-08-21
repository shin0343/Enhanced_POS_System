#define SYSCLK	72000000
#define HCLK	SYSCLK
#define PCLK2	HCLK
#define PCLK1	(HCLK/2)
#define TIMXCLK ((HCLK == PCLK1)?(PCLK1):(PCLK1*2))

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07E0
#define RED		0xf800

#define NO_ERROR	0
