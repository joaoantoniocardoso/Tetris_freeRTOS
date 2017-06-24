/*
 * main.c
 *
 * Created: 15/11/2016 21:20:58
 *  Author: winarchmkr
 */ 

#include "defPrincipais.h"
#include "figuras.h"
#include "keys.h"


static int32_t score = 0;					//! game score
static int32_t level = 0;					//! game level
static int32_t game = 0;					//! game status: 0 for start screen, 1 for run, 2 for gameover
static int32_t frame = 0;					//! frame tick
const static int32_t frameLayout[21][12] =
{	
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1},
	{0,1,0,0,0,0,0,0,0,0,0,1}
};			//! 
static int32_t wall[21][12];				//! 


struct xy_t{
	int32_t x;
	int32_t y;
};
static struct xy_t brick0xy = {19, 6}; 

struct block_t{
	int32_t x;					//! x position of the object
	int32_t y;					//! y position of the object
	int32_t bricks[4][4];		//! 
};

#define I_BLOCK {	1, 1, 1, 1, \
					0, 0, 0, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define J_BLOCK {	1, 0, 0, 0, \
					1, 1, 1, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define L_BLOCK {	1, 1, 1, 0, \
					1, 0, 0, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define O_BLOCK {	1, 1, 0, 0, \
					1, 1, 0, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define S_BLOCK {	1, 1, 0, 0, \
					0, 1, 1, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define Z_BLOCK {	0, 1, 1, 0, \
					1, 1, 0, 0, \
					0, 0, 0, 0, \
					0, 0, 0, 0 }
#define T_BLOCK {	0, 1, 0, 0, \
					1, 1, 0, 0, \
					0, 1, 0, 0, \
					0, 0, 0, 0 }
static struct block_t block = { 21, 6, I_BLOCK };

/*
 * @brief 
 */
int32_t getRandomNumber(int32_t min, int32_t max){
	return (TRNG->TRNG_ODATA % max) +min;
}

/*
 * @brief just draws the wall
 */
void drawWall(void)
{
	int32_t x, y;
	
	for(y = 0; y < 12; y++){
		for(x = 0; x < 21; x++){
			struct pontos_t p = {x*4,y*4,0,0,0,0};
			if(wall[x][y])	desenha_fig(&p, &solid_brick0f);
			else			desenha_fig(&p, &empty_brick0f);
		}
	}
}

/*
 * @brief just cleans the wall
 */
void cleanWall(void)
{
	int32_t x, y;
	
	for(y = 0; y < 12; y++){
		for(x = 0; x < 21; x++){
			wall[x][y] = 0;
		}
	}
}

/*
 * @brief just remove a column of bricks from the wall and make it falls to ground
 */
void rmBrickColumn(int32_t x)
{
	int32_t y;
	for(y = 0; y < 12; y++){
		wall[x][y] = 0;
	}
	
	for(x; x < 21; x++){
		for(y = 0; y < 12; y++){
			if(x+1 < 21) 
				wall[x][y] = wall[x+1][y];
		}
	}
	
	score += 100*level;
	
}

/*
 * @brief just adds a brick to the wall (called after a colision)
 */
void addBrick(int32_t x, int32_t y)
{
	if(x >= 19){
		game = 2; // game over
	}
	
	int32_t i, j;
	
	// adds each brick from the block to the wall
	for(j = 0; j < 4; j++){
		for(i = 0; i < 4; i++){
			if(block.bricks[i][j]) wall[x+i][y+j] = 1;
		}
	}
	
	int32_t b = 1;
	for(y = 2; y < 11; y++){
		b &= wall[x][y];
	}
	if(b)	rmBrickColumn(x);
}

/*
 * @brief randomyly creates a new falling brick
 */
void newBrick(void)
{
	brick0xy.x = 19;
	brick0xy.y = 6;
	
	int32_t random = getRandomNumber(1, 7);
	
	int32_t i, j;
	
	if(random == 1){
		int32_t mblock[4][4] = I_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 2){
		int32_t mblock[4][4] = J_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 3){
		int32_t mblock[4][4] = L_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 4){
		int32_t mblock[4][4] = O_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 5){
		int32_t mblock[4][4] = S_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 6){
		int32_t mblock[4][4] = Z_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else
	if(random == 7){
		int32_t mblock[4][4] = T_BLOCK;
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				block.bricks[i][j] = mblock[i][j];
			}
		}
	}else{
		;
	}
	
	score += 10*level;
}

/*
 * @brief just moves left if collision isn't detected, otherwise handles it
 */
void moveLeft()
{
	int32_t x = brick0xy.x -1;
	int32_t y = brick0xy.y;

	int32_t i, j, b;
	
	if(x <= 0){
		// end of screen
		x = 0;
		addBrick(x, y);
		newBrick();	
	}else{
		b = 0;
		
		for(j = 0; j < 4; j++){
			for(i = 0; i < 4; i++){
				if(block.bricks[i][j]){
					if(wall[x+i][y+j]){
						// collision with wall detected!
						b = 1;
					}
				}
			}
		}
			
		if(!b) brick0xy.x--;	// move
		else{
			addBrick(brick0xy.x, brick0xy.y);
			newBrick();
		}
	}

}

/*
 * @brief just moves down if collision isn't detected, otherwise handles it
 */
void moveDown()
{
	int32_t y = brick0xy.y +1;
	int32_t x = brick0xy.x;
	if(y > 12) y = 12;
	
	int32_t i, j, b;
	
	b = 0;
	
	for(j = 0; j < 4; j++){
		for(i = 0; i < 4; i++){
			if(block.bricks[i][j]){
				if(wall[x+i][y+j]){
					// collision with wall detected!
					b = 1;
				}else if(frameLayout[x+i][y+j]){
					// collision with frameLayout detected!
					b = 1;
				}
			}
		}
	}
	
	if(!b)	brick0xy.y++;
}

/*
 * @brief just moves up if collision isn't detected, otherwise handles it
 */
void moveUp()
{
	int32_t y = brick0xy.y -1;
	int32_t x = brick0xy.x;
	if(y < 0) y = 0;
	
	int32_t i, j, b;
	
	b = 0;
	
	for(j = 0; j < 4; j++){
		for(i = 0; i < 4; i++){
			if(block.bricks[i][j]){
				if(wall[x+i][y+j]){
					// collision with wall detected!
					b = 1;
				}else if(frameLayout[x+i][y+j]){
					// collision with frameLayout detected!
					b = 1;
				}
			}
		}
	}

	if(!b)	brick0xy.y--;	
}

/*
 * @brief 
 */
void rotate()
{
	int32_t i, j;
	int32_t bricks[4][4];
	for(j = 0; j < 4; j++){
		for(i = 0; i < 4; i++){
			bricks[j][i] = block.bricks[i][j];
		}
	}
	for(j = 0; j < 4; j++){
		for(i = 0; i < 4; i++){
			block.bricks[i][j] = bricks[i][j];
		}
	}
}

/*
 * @brief this debug that shows some variables in the screen
 */
void debugKeys(void)
{
	
	int32_t key = getKey();
	
	// keys debug:
	if(tst_bit(key, STICK_UP))			escreve_Nr_Peq(0, 0, 1, 1);
	else								escreve_Nr_Peq(0, 0, 0, 1);
	if(tst_bit(key, STICK_DOWN))		escreve_Nr_Peq(5, 0, 1, 1);
	else								escreve_Nr_Peq(5, 0, 0, 1);
	if(tst_bit(key, STICK_RIGHT))		escreve_Nr_Peq(10, 0, 1, 1);
	else								escreve_Nr_Peq(10, 0, 0, 1);
	if(tst_bit(key, STICK_LEFT))		escreve_Nr_Peq(15, 0, 1, 1);
	else								escreve_Nr_Peq(15, 0, 0, 1);
	if(tst_bit(key, STICK_CLICK))		escreve_Nr_Peq(20, 0, 1, 1);
	else								escreve_Nr_Peq(20, 0, 0, 1);
}

/*
 * @brief just draws the frame layout
 */
void drawFrameLayout(void)
{
	int32_t x, y;
	
	for(y = 0; y < 12; y++){
		for(x = 0; x < 21; x++){
			struct pontos_t p = {x*4,y*4,0,0,0,0};
			if(frameLayout[x][y]){
				if(y<3)		desenha_fig(&p, &topFrame_brick0f);
				else		desenha_fig(&p, &botFrame_brick0f);
			}//else			desenha_fig(&p, &empty_brick0f);
		}
	}
}

/*
 * @brief just draws the frame layout
 */
void drawBrick(void)
{
	int32_t x, y;
	
	block.x = brick0xy.x;
	block.y = brick0xy.y;
	
	for(y = 0; y < 4; y++){
		for(x = 0; x < 4; x++){
			if(block.bricks[x][y]){
				struct pontos_t p = {(block.x +x)*4, (block.y +y)*4, 0, 0, 0, 0};
				desenha_fig(&p, &solid_brick0f);
			}
		}
	}
	vTaskDelay(ms(500));
}

/*
 * @brief tetris basics
 */
void run(void)
{
	if(frame){
		drawBrick();
		
		drawWall();		
		
		drawFrameLayout();

		moveLeft();
	
		frame = 0;
	}else taskYIELD();	
}

/*
 * @brief start screen
 */
void startScreen(void)
{
	limpa_LCD();
	imprime_LCD();

	atraso_ms(500);
	
	goto_XY(0, 2);
	string_LCD("BRICKS TO WALL");
	goto_XY(0, 5);
	string_LCD("click to start");

	inverte_pixels();
	
	imprime_LCD();

	while(!tst_bit(getKey(), STICK_CLICK));
	
	goto_XY(0,0);
	normaliza_pixels();
	limpa_LCD();
	imprime_LCD();
	
	// cleans the wall
	cleanWall();

	game = level = 1;
}

/*
 * @brief game over screen
 */
void gameOver(void)
{
	limpa_LCD();
	imprime_LCD();

	atraso_ms(500);
	
	goto_XY(15, 2);
	string_LCD("GAME OVER");
	escreve_Nr_Peq(20, 30, score, 6);
	
	imprime_LCD();

	while(!tst_bit(getKey(), STICK_CLICK)){
		inverte_pixels();
		atraso_ms(150);
		normaliza_pixels();
		atraso_ms(150);
	}
	
	goto_XY(0,0);
	normaliza_pixels();
	limpa_LCD();
	imprime_LCD();

	game = level = score = 0;
}

/*
 * @brief just makes the brick to fall
 */
void vTask_game(void *pvParameters)
{
	game = 0;
	while(1){
		switch (game){
			case 0:
				startScreen();
			break;
			case 1:
				run();
			break;
			case 2:
			default:
				gameOver();
			break;
		} 
		//vTaskDelay(ms(500));
		taskYIELD();
	}
}

/*
 * @brief handles the keys
 */
void vTask_keys(void *pvParameters)
{
	while(1){
		
		stickHandler();

		if(!frame){
			int32_t key = getKey();
		
			if(tst_bit(key, STICK_LEFT))		moveLeft();
			//else if(tst_bit(key, STICK_RIGHT))	moveRight();
			
			if(tst_bit(key, STICK_DOWN))		moveDown();
			else if(tst_bit(key, STICK_UP))		moveUp();
			
			if(tst_bit(key, STICK_CLICK))		rotate();

			frame = 1;
		}else taskYIELD();
		
		//vTaskDelay(ms(100));
			
	}
}

/*
 * @brief just shows the score on top right and level on top left
 */
void vTask_score(void *pvParameters)
{
	while(1){
		if(game == 1){
			escreve_Nr_Peq(60, 0, score, 6); // mostra até 99999
			escreve_Nr_Peq(0, 0, level, 3);
			//debugKeys();
		}
		vTaskDelay(ms(200));
	}
}

/*
 * @brief just updates LCD screen
 */
void vTask_screen(void *pvParameters)
{
	while(1){
		if(game == 1)	imprime_LCD();
		vTaskDelay(ms(100));
	}
}

/*
 * @brief Gerador de Números Aleatórios do ATSAM
 */
void randomGeneratorEnable(void)
{
	pmc_enable_periph_clk(ID_TRNG);		// Ativa o CLK para o TRNG
	TRNG->TRNG_CR = 0x524E4701;			// Habilita o TRNG
}

/*
 * @brief general configuration
 */
void config(void)
{
	sysclk_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;		//desabilitar o watchdog!
	
	// --------------------------------------------------------------------------------------
	// Botao Joystik  pino PA23 - A2 (Arduino)
	PIOA->PIO_PER =  PIO_PA23;			// PIO Enable Register
	PIOA->PIO_PUER = PIO_PA23;			// Pull-up Enable Register (DEFAULT É HABILITADO)
	PIOA->PIO_ODR =  PIO_PA23;			// Output Disable Register (DEFAULT É DESABILITADO)

	// Habilita o Debouncing por hardware
	PIOA->PIO_DIFSR= PIO_PA23;	// Debouncing Input Filter Select Register
	PIOA->PIO_IFER = PIO_PA23;	// Glitch Input Filter Enable Register 
	PIOA->PIO_SCDR = 163;		/* ajuste do divisor de clock para o debouncing filter
									DIV = (T/(2.Tslow)) - 1
									[Slow Clock = 32768 Hz] clock para o sistema
									[T = 10 ms] período do debauncing - validade garantida para pulso > T
								*/
	//Reading the I/O line levels requires the clock of the PIO controller to be enabled
	PMC->PMC_PCER0 = 0x01 << ID_PIOA;
	
	// Gerador de Números Aleatórios do ATSAM
	pmc_enable_periph_clk(ID_TRNG);	// liga o ckl para o TRNG
	TRNG->TRNG_CR = 0x524E4701;		// habilita o TRNG
	
	// --------------------------------------------------------------------------------------
	inic_ADC();
	//inic_Interrupt_ADC_EOC();
	// --------------------------------------------------------------------------------------
	// inicializa LCD 5110   
	inic_IO_LCD();					// CLK (13 - PB27), DIN (12 - PD8), DC (11 - PD7), CE (10 - PC29), RST (9 - PC21)
	inic_LCD();
	limpa_LCD();
	
	randomGeneratorEnable();
}


/*
 * @brief main
 */
int main (void)
{
	config();

	xTaskCreate(vTask_game, "Game", 100, NULL, 1, NULL);
	xTaskCreate(vTask_screen, "Screen", 100, NULL, 1, NULL);
	xTaskCreate(vTask_keys, "Keys", 100, NULL, 1, NULL);
	xTaskCreate(vTask_score, "Score", 100, NULL, 1, NULL);

	vTaskStartScheduler();	// apos este comando o RTOS passa a executar as tarefas

	while(1);
}
