#ifndef DEFPRINCIPAIS_H_
#define DEFPRINCIPAIS_H_

//---------------------------------------------------------------------------------

#define FCPU 84000000				//! define a frequencia da CPU - 84 MHz - ALTERAR TB NA LINHA DE BAIXO!

asm (".equ fcpu, 84000000  \n\t");	//! diretiva para usar fcpu em assembly

#include <asf.h>	//! Include header files for all drivers that have been imported from Atmel Software Framework (ASF).
#include "atraso.h"
#include "NOKIA5110_fb.h"
#include "ADC.h"

// Definição para uso de atraso no FreeRTOS
#define ms(tempo)		( (tempo) / portTICK_RATE_MS )

// Definições de macros para o trabalho com bits

#define bit(x)			( 1 << (x) )

#define	set_bit(y,bit)	( (y) |= (1 << (bit)) )		//! coloca em 1 o bit x da variável Y
#define	clr_bit(y,bit)	( (y) &= ~(1 << (bit)) )	//! coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) 	( (y) ^= (1 << (bit)) )		//! troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) 	( (y) & (1 << (bit)) )		//! retorna 0 ou 1 conforme leitura do bit

//----------------------------------------------------------------------------------

#endif /* DEFPRINCIPAIS_H_ */