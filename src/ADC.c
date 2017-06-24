#include "ADC.h"

static uint32_t eixo_x, eixo_y;

//-----------------------------------------------------------------------------------------------------
void inic_Interrupt_ADC_EOC()
{
	NVIC_EnableIRQ (ADC_IRQn);						// enable ADC interrupt vector
	ADC->ADC_IER = ADC_IER_EOC7|ADC_IER_EOC6;		// enable AD6/7 End-Of-Conv interrupt (Arduino pin A0 e A1) 
	
	ADC->ADC_CR = ADC_CR_START;					// inicia conversao
}
//-----------------------------------------------------------------------------------------------------
void inic_ADC()
{
	PIOA->PIO_PDR = PIO_PA16|PIO_PA24;				// disable PIO control
	PMC->PMC_PCER1 = 1 << (ID_ADC-32);				// the ADC clock enable 
	
	ADC->ADC_CHER = ADC_CHDR_CH7| ADC_CHDR_CH6;		// enable CH7 / AD7 e CH6  / AD6

	ADC->ADC_CGR = 0x00000000;						// ganho 1x para o CH6/7 (cuidado, ADC_MR_ANACH_ALLOWED para ganho individual)
	ADC->ADC_COR = 0x00000000;						// all offsets off and single ended mode (DIFFx)	


	ADC->ADC_MR = (0xFF<<8)|ADC_MR_FREERUN_ON|ADC_MR_LOWRES_BITS_10;	/* PRESCAL = 255, 10 bits resolution, starting conversion by software
																	   ADC_clk = FCPU/((PRESCAL+1)*2)	-> FCPU = 84MHz -> 164kHz		*/
	//ADC->ADC_ACR = ADC_ACR_TSON;					// temperature sensor ON
}
//-----------------------------------------------------------------------------------------------------
void ADC_Handler()
{
	if(ADC->ADC_ISR & ADC_ISR_EOC7)							// ensure there was an End-of-Conversion and we read the ISR reg
	{
		eixo_y = ADC->ADC_CDR[7];													
	}
	if(ADC->ADC_ISR & ADC_ISR_EOC6)															
	{
		eixo_x =  ADC->ADC_CDR[6];			
	}															
}
//-----------------------------------------------------------------------------------------------------
uint32_t get_ADC_VRx()
{
	return eixo_x;
}
//-----------------------------------------------------------------------------------------------------
uint32_t get_ADC_VRy()
{
	return eixo_y;
}
//-----------------------------------------------------------------------------------------------------
