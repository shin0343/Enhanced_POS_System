/*   ----------- RCC -----------   */
#define rRCC_CR        (*(volatile unsigned long*)0x40021000)
#define rRCC_CFGR      (*(volatile unsigned long*)0x40021004)
#define rRCC_CIR       (*(volatile unsigned long*)0x40021008)
#define rRCC_APB2RSTR  (*(volatile unsigned long*)0x4002100C)
#define rRCC_APB1RSTR  (*(volatile unsigned long*)0x40021010)
#define rRCC_AHBENR    (*(volatile unsigned long*)0x40021014)
#define rRCC_APB2ENR   (*(volatile unsigned long*)0x40021018)
#define rRCC_APB1ENR   (*(volatile unsigned long*)0x4002101C)
#define rRCC_BDCR      (*(volatile unsigned long*)0x40021020)
#define rRCC_CSR       (*(volatile unsigned long*)0x40021024)

/*   ----------- Flash -----------   */
#define rFlash_memory  (*(volatile unsigned long*)0x40022000)

/*   ----------- GPIO -----------   */
#define rGPIOA_CRL      (*(volatile unsigned long*)0x40010800)
#define rGPIOA_CRH      (*(volatile unsigned long*)0x40010804)
#define rGPIOA_IDR      (*(volatile unsigned long*)0x40010808)
#define rGPIOA_ODR      (*(volatile unsigned long*)0x4001080C)
#define rGPIOA_BSRR     (*(volatile unsigned long*)0x40010810)
#define rGPIOA_BRR      (*(volatile unsigned long*)0x40010814)
#define rGPIOA_LCKR     (*(volatile unsigned long*)0x40010818)

#define rGPIOB_CRL      (*(volatile unsigned long*)0x40010C00)
#define rGPIOB_CRH      (*(volatile unsigned long*)0x40010C04)
#define rGPIOB_IDR      (*(volatile unsigned long*)0x40010C08)
#define rGPIOB_ODR      (*(volatile unsigned long*)0x40010C0C)
#define rGPIOB_BSRR     (*(volatile unsigned long*)0x40010C10)
#define rGPIOB_BRR      (*(volatile unsigned long*)0x40010C14)
#define rGPIOB_LCKR     (*(volatile unsigned long*)0x40010C18)

#define rGPIOC_CRL      (*(volatile unsigned long*)0x40011000)
#define rGPIOC_CRH      (*(volatile unsigned long*)0x40011004)
#define rGPIOC_IDR      (*(volatile unsigned long*)0x40011008)
#define rGPIOC_ODR      (*(volatile unsigned long*)0x4001100C)
#define rGPIOC_BSRR     (*(volatile unsigned long*)0x40011010)
#define rGPIOC_BRR      (*(volatile unsigned long*)0x40011014)
#define rGPIOC_LCKR     (*(volatile unsigned long*)0x40011018)


/*   ----------- AFIO -----------   */
// 0x4001 0000 - 0x4001 03FF
#define rAFIO_EXTICR2   (*(volatile unsigned long*)0x4001000C)          // External interrupt configuration register 2 
#define rAFIO_EXTICR4   (*(volatile unsigned long*)0x40010014)          // External interrupt configuration register 4 


/*   ----------- Interrupt -----------   */
// 0x4001 0400 - 0x4001 07FF      
#define rEXTI15_10      (*(volatile unsigned long*)0x000000E0)
#define rEXTI_IMR       (*(volatile unsigned long*)0x40010400)          // Interrupt mask register
#define rEXTI_EMR       (*(volatile unsigned long*)0x40010404)          // Event mask register
#define rEXTI_RTSR      (*(volatile unsigned long*)0x40010408)          // Rising Trigger selection register 
#define rEXTI_FTSR      (*(volatile unsigned long*)0x4001040c)          // Falling Trigger selection register 
#define rEXTI_SWIER     (*(volatile unsigned long*)0x40010410)          // Software interrupt event register 
#define rEXTI_PR        (*(volatile unsigned long*)0x40010414)          // Pending register 

#define rNVIC_ISPR0     (*(volatile unsigned long*)0xE000E380)          // Position [0:31] Pending Interrupt vector address
#define rNVIC_ISPR1    (*(volatile unsigned long*)0xE000E384)    // Position [32:42] Pending Interrupt vector address
#define rNVIC_ICPR0     (*(volatile unsigned long*)0xE000E380)    // Position [0:31] Pending Interrupt vector address
#define rNVIC_ICPR1    (*(volatile unsigned long*)0xE000E384)    // Position [32:42] Pending Interrupt vector address
#define rNVIC_ISER0    (*(volatile unsigned long*)0xE000E100)    // Position [0:31] Interrupt Set-Enable Register address
#define rNVIC_ISER1   (*(volatile unsigned long*)0xE000E104)    // Position [32:42] Interrupt Set-Enable Register address 
#define rNVIC_ICER0   (*(volatile unsigned long*)0xE000E180)    // Position [0:31] Interrupt Set-Disable Register address
#define rNVIC_ICER1  (*(volatile unsigned long*)0xE000E184)    // Position [32:42] Interrupt Set-Disable Register addres

/*   ----------- Uart1 -----------  base : 0x4001 3800 */
#define rUSART1_SR      (*(volatile unsigned long*)0x40013800)
#define rUSART1_DR      (*(volatile unsigned long*)0x40013804)
#define rUSART1_BRR     (*(volatile unsigned long*)0x40013808)
#define rUSART1_CR1     (*(volatile unsigned long*)0x4001380C)
#define rUSART1_CR2     (*(volatile unsigned long*)0x40013810)  
#define rUSART1_CR3     (*(volatile unsigned long*)0x40013814)  
#define rUSART1_GTPR    (*(volatile unsigned long*)0x40013818)


/*   ----------- Uart2 -----------  base : 0x4000 4400 */
#define rUSART2_SR      (*(volatile unsigned long*)0x40004400)    
#define rUSART2_DR      (*(volatile unsigned long*)0x40004404)
#define rUSART2_BRR     (*(volatile unsigned long*)0x40004408)
#define rUSART2_CR1     (*(volatile unsigned long*)0x4000440C)
#define rUSART2_CR2     (*(volatile unsigned long*)0x40004410)  
#define rUSART2_CR3     (*(volatile unsigned long*)0x40004414)
#define rUSART2_GTPR    (*(volatile unsigned long*)0x40004418)

/*   ----------- Timer -----------  base : 0x4000 000 */
#define rTIM2_CR1        (*(volatile unsigned long*)0x40000000)
#define rTIM2_CR2        (*(volatile unsigned long*)0x40000004)
#define rTIM2_SMCR       (*(volatile unsigned long*)0x40000008)
#define rTIM2_DIER       (*(volatile unsigned long*)0x4000000C)

#define rTIM2_SR         (*(volatile unsigned long*)0x40000010)
#define rTIM2_EGR        (*(volatile unsigned long*)0x40000014)
#define rTIM2_CCMR1      (*(volatile unsigned long*)0x40000018)
#define rTIM2_CCMR2      (*(volatile unsigned long*)0x4000001C)

#define rTIM2_CCER       (*(volatile unsigned long*)0x40000020)
#define rTIM2_CNT        (*(volatile unsigned long*)0x40000024)
#define rTIM2_PSC        (*(volatile unsigned long*)0x40000028)
#define rTIM2_ARR        (*(volatile unsigned long*)0x4000002C)
#define rTIM2_CCR1       (*(volatile unsigned long*)0x40000034)
#define rTIM2_CCR2       (*(volatile unsigned long*)0x40000038)
#define rTIM2_CCR3       (*(volatile unsigned long*)0x4000003C)
#define rTIM2_CCR4       (*(volatile unsigned long*)0x40000040)

#define rTIM3_CR1        (*(volatile unsigned long*)0x40000400)
#define rTIM3_CR2        (*(volatile unsigned long*)0x40000404)
#define rTIM3_SMCR       (*(volatile unsigned long*)0x40000408)
#define rTIM3_DIER       (*(volatile unsigned long*)0x4000040C)
#define rTIM3_SR         (*(volatile unsigned long*)0x40000410)
#define rTIM3_EGR        (*(volatile unsigned long*)0x40000414)
#define rTIM3_CCMR1      (*(volatile unsigned long*)0x40000418)
#define rTIM3_CCMR2      (*(volatile unsigned long*)0x4000041C)
#define rTIM3_CCER       (*(volatile unsigned long*)0x40000420)
#define rTIM3_CNT        (*(volatile unsigned long*)0x40000424)
#define rTIM3_PSC        (*(volatile unsigned long*)0x40000428)
#define rTIM3_ARR        (*(volatile unsigned long*)0x4000042C)
#define rTIM3_CCR1       (*(volatile unsigned long*)0x40000434)
#define rTIM3_CCR2       (*(volatile unsigned long*)0x40000438)
#define rTIM3_CCR3       (*(volatile unsigned long*)0x4000043C)
#define rTIM3_CCR4       (*(volatile unsigned long*)0x40000440)

#define rTIM4_CR1        (*(volatile unsigned long*)0x40000800)
#define rTIM4_CR2        (*(volatile unsigned long*)0x40000804)
#define rTIM4_SMCR       (*(volatile unsigned long*)0x40000808)
#define rTIM4_DIER       (*(volatile unsigned long*)0x4000080C)
#define rTIM4_SR         (*(volatile unsigned long*)0x40000810)
#define rTIM4_EGR        (*(volatile unsigned long*)0x40000814)
#define rTIM4_CCMR1      (*(volatile unsigned long*)0x40000818)
#define rTIM4_CCMR2      (*(volatile unsigned long*)0x4000081C)
#define rTIM4_CCER       (*(volatile unsigned long*)0x40000820)
#define rTIM4_CNT        (*(volatile unsigned long*)0x40000824)
#define rTIM4_PSC        (*(volatile unsigned long*)0x40000828)
#define rTIM4_ARR        (*(volatile unsigned long*)0x4000082C)
#define rTIM4_CCR1       (*(volatile unsigned long*)0x40000834)
#define rTIM4_CCR2       (*(volatile unsigned long*)0x40000838)
#define rTIM4_CCR3       (*(volatile unsigned long*)0x4000083C)
#define rTIM4_CCR4       (*(volatile unsigned long*)0x40000840)


/*   ----------- ADC1 -----------  0x4001 2400 - 0x4001 27FF */
#define rADC1_SR         (*(volatile unsigned long*)0x40012400)
#define rADC1_CR1        (*(volatile unsigned long*)0x40012404)
#define rADC1_CR2        (*(volatile unsigned long*)0x40012408)
#define rADC1_SMPR1      (*(volatile unsigned long*)0x4001240C)
#define rADC1_SMPR2      (*(volatile unsigned long*)0x40012410)
#define rADC1_JOFR1      (*(volatile unsigned long*)0x40012414)
#define rADC1_JOFR2      (*(volatile unsigned long*)0x40012418)
#define rADC1_JOFR3      (*(volatile unsigned long*)0x4001241C)
#define rADC1_JOFR4      (*(volatile unsigned long*)0x40012420)
#define rADC1_HTR        (*(volatile unsigned long*)0x40012424)
#define rADC1_LTR        (*(volatile unsigned long*)0x40012428)
#define rADC1_SQR1       (*(volatile unsigned long*)0x4001242C)
#define rADC1_SQR2       (*(volatile unsigned long*)0x40012430)
#define rADC1_SQR3       (*(volatile unsigned long*)0x40012434)
#define rADC1_JSQR       (*(volatile unsigned long*)0x40012438)
#define rADC1_JDR1       (*(volatile unsigned long*)0x4001243C)
#define rADC1_JDR2       (*(volatile unsigned long*)0x40012440)
#define rADC1_JDR3       (*(volatile unsigned long*)0x40012444)
#define rADC1_JDR4       (*(volatile unsigned long*)0x40012448)
#define rADC1_DR         (*(volatile unsigned long*)0x4001244C)

/*   ----------- ADC2 -----------  0x4001 2800 - 0x4001 2BFF */
#define rADC2_SR         (*(volatile unsigned long*)0x40012800)
#define rADC2_CR1        (*(volatile unsigned long*)0x40012804)
#define rADC2_CR2        (*(volatile unsigned long*)0x40012808)
#define rADC2_SMPR1      (*(volatile unsigned long*)0x4001280C)
#define rADC2_SMPR2      (*(volatile unsigned long*)0x40012810)
#define rADC2_JOFR1      (*(volatile unsigned long*)0x40012814)
#define rADC2_JOFR2      (*(volatile unsigned long*)0x40012818)
#define rADC2_JOFR3      (*(volatile unsigned long*)0x4001281C)
#define rADC2_JOFR4      (*(volatile unsigned long*)0x40012820)
#define rADC2_HTR        (*(volatile unsigned long*)0x40012824)
#define rADC2_LTR        (*(volatile unsigned long*)0x40012828)
#define rADC2_SQR1       (*(volatile unsigned long*)0x4001282C)
#define rADC2_SQR2       (*(volatile unsigned long*)0x40012830)
#define rADC2_SQR3       (*(volatile unsigned long*)0x40012834)
#define rADC2_JSQR       (*(volatile unsigned long*)0x40012838)
#define rADC2_JDR1       (*(volatile unsigned long*)0x4001283C)
#define rADC2_JDR2       (*(volatile unsigned long*)0x40012840)
#define rADC2_JDR3       (*(volatile unsigned long*)0x40012844)
#define rADC2_JDR4       (*(volatile unsigned long*)0x40012848)
#define rADC2_DR         (*(volatile unsigned long*)0x4001284C)