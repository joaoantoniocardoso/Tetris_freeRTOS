#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include "defPrincipais.h"
#include "atraso.h"

//-------------------------------------------------------------------------------------
// RST (9 - PC21)
#define SET_RST		PIOC->PIO_SODR
#define CLR_RST		PIOC->PIO_CODR	
#define RST			PIO_PC21	

// CE (10 - PC29)
#define SET_CE		PIOC->PIO_SODR
#define CLR_CE		PIOC->PIO_CODR
#define CE			PIO_PC29

// DC (11 - PD7)
#define SET_DC		PIOD->PIO_SODR		
#define CLR_DC		PIOD->PIO_CODR		
#define DC			PIO_PD7				

// DIN (12 - PD8)
#define SET_DIN		PIOD->PIO_SODR
#define CLR_DIN		PIOD->PIO_CODR
#define DIN			PIO_PD8

// CLK (13 - PB27)
#define SET_CLK		PIOB->PIO_SODR
#define CLR_CLK		PIOB->PIO_CODR
#define CLK			PIO_PB27
//-------------------------------------------------------------------------------------
#define pulso_CLK() atraso_us(1); SET_CLK=CLK; atraso_us(1); CLR_CLK=CLK

#define LCD_X     84
#define LCD_Y     48

#define inverte_pixels()		cmd_LCD(0x0D)
#define normaliza_pixels()		cmd_LCD(0x0C)	
//-------------------------------------------------------------------------------------

struct pontos_t{
	uint32_t x1;
	uint32_t y1;
	uint32_t x2;
	uint32_t y2;
	uint32_t x3;
	uint32_t y3;	
};

struct figura_t
{
	unsigned char largura;
	unsigned char altura;
	unsigned char pixels[];
};

void inic_IO_LCD();		// Editar quando alterar os pinos de IO!
void cmd_LCD(uint32_t data);
void data_LCD(uint32_t data);
void inic_LCD(); 
void goto_XY(uint32_t x, uint32_t y);
void bitmap_LCD(const unsigned char my_array[]);
void caractere_LCD(char character);
void string_LCD(char *msg);
void limpa_LCD(); 

// Funções do Borges
void string_LCD_Nr(char * msg, uint32_t valor, uint32_t quant2Print);

void desenha_pixel(uint32_t x, uint32_t y, uint32_t propriedade);
void desenha_linha(struct pontos_t *coord, uint32_t prop);
void desenha_circulo(int32_t x0, int32_t y0, int32_t radius, uint32_t prop);
void desenha_retangulo(struct  pontos_t *t, uint32_t prop);
void desenha_triangulo(struct  pontos_t *p, uint32_t prop);
void desenha_fig(struct  pontos_t *p, const struct figura_t *figura);


//-------------------------------------------------------------------------------------
#endif /* NOKIA5110_H_ */