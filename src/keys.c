/*
 * keys.c
 *
 * Created: 15/11/2016 21:20:58
 *  Author: winarchmkr
 */ 

#include "keys.h"

int32_t getKey(void)
{
	return key;
}

/*
 * @brief reads the joystick and treat it as some key
 *		actually it reads ADC, apply a filter and interprets its meaning translating it to some key
 *		code. To get the key, it is needed to use the getKey() function.
 */
void stickHandler(void)
{
	// gets the stick position
	ADC_Handler();		// le o ADC, atualiza x e y
	
	ADC_x[k] = get_ADC_VRx();  //valor do ADC entra no filtro, na amostra mais antiga
	ADC_y[k] = get_ADC_VRy();

	if(++k == 32) k = 0;

	media_eixoX = media_eixoY = 0; // zera antes de entrar no somatorio
	for(j = 0; j < 32; j++){
		media_eixoX += ADC_x[j];  // somatório das amostras e média
		media_eixoY += ADC_y[j];
	}

	media_eixoX = 512 -( media_eixoX >> 5 );
	media_eixoY = 512 -( media_eixoY >> 5 );
	
	// X axis
	if(media_eixoX < -50 ){
		set_bit(key, STICK_LEFT);
		clr_bit(key, STICK_RIGHT);
	}else if(media_eixoX > 50){
		set_bit(key, STICK_RIGHT);
		clr_bit(key, STICK_LEFT);
	}else{
		clr_bit(key, STICK_LEFT);
		clr_bit(key, STICK_RIGHT);
	}
	
	// Y axis
	if(media_eixoY < -50 ){
		set_bit(key, STICK_DOWN);
		clr_bit(key, STICK_UP);
	}else if(media_eixoY > 50){
		set_bit(key, STICK_UP);
		clr_bit(key, STICK_DOWN);
	}else{
		clr_bit(key, STICK_UP);
		clr_bit(key, STICK_DOWN);
	}
	
	
	// gets the button click
	if(PIOA->PIO_PDSR & PIO_PA23)	clr_bit(key, STICK_CLICK);
	else							set_bit(key, STICK_CLICK);
	
}
