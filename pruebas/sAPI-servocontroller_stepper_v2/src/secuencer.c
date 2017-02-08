/*==================[inclusions]=============================================*/

#include "servoController.h"        /* <= own header */
#include "pca9685Driver.h"          /* <= PCA9685 driver header */
#include "secuencer.h"

#include "sAPI.h"         /* <= sAPI header */

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


/*==================[macros and definitions]=================================*/
#define SEC_CANT 4
/*==================[internal data declaration]==============================*/

static uint8_t count = 0;

/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/


static const TypeSecuencia secList[SEC_CANT] = {
/*	TYPE		| Servos	|Mov |dir*/
/*0*/	        { 0x4444, 100, 0},
/*1*/	        { 0x4444, 100, 1},
/*2*/	        { 0x8888, 100, 1},
/*3*/	        { 0x8888, 100, 0},
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
TypeSecuencia secuencer_getSecuencia(){
	TypeSecuencia aux = secList[count];
	count++;
	if(count == SEC_CANT)
		count = 0;
	return aux;
}
void secuencer_reset(){
	count = 0;
}

uint8_t secuencer_getCount(){
	return count;
}
/*==================[end of file]============================================*/
