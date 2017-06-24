/*
 * keys.h
 *
 * Created: 15/11/2016 19:23:00
 *  Author: winarchmkr
 */ 


#ifndef KEYS_H_
#define KEYS_H_

#include "defPrincipais.h"

// private:
static int32_t key = 0;			//! 

static int32_t media_eixoX = 0, media_eixoY = 0;
static uint32_t ADC_x[32], ADC_y[32], k = 0, j = 0;

// public:

enum keys {
	NONE = 0x0000,
	STICK_UP,
	STICK_DOWN,
	STICK_RIGHT,
	STICK_LEFT,
	STICK_CLICK
	};

int32_t getKey(void);

void stickHandler(void);

#endif /* KEYS_H_ */