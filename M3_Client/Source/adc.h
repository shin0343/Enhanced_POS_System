#ifndef __ADC_H_
#define __ADC_H_

    #include "M3_addr.h"
    #include "macro.h"
    #include "my_types.h"

    void ADC1_Init(void);
    void ADC1_ISR_Enable(int en,int ch);
    void ADC1_ISR_Start(void);
    int ADC1_Get_Data(int ch);
    void ADC1_ISR_Stop(void);
    
    
#endif


