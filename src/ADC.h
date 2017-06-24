#ifndef ADC_H_
#define ADC_H_


#include "defPrincipais.h"

void inic_Interrupt_ADC_EOC();
void inic_ADC();
void ADC_Handler();
uint32_t get_ADC_VRx();
uint32_t get_ADC_VRy();


#endif /* ADC_H_ */