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
#define SEC_CANT 14
/*==================[internal data declaration]==============================*/

static uint8_t count = 0;

/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/


static const TypeSecuencia secList[SEC_CANT] = {
/*	TYPE		| Servos	|Mov |dir*/
/*0*/

				{ 0x0003, 20, 1},
				{ 0x0004, 40, 1},
				{ 0x0003, 20, 0},

				{ 0x0030, 20, 1},
				{ 0x0040, 40, 0},
				{ 0x0030, 20, 0},

				{ 0x0300, 20, 1},
				{ 0x0400, 40, 1},
				{ 0x0300, 20, 0},

				{ 0x3000, 20, 1},
				{ 0x4000, 40, 0},
				{ 0x3000, 20, 0},

				{ 0x0404, 40, 0},
				{ 0x4040, 40, 1},

				//{ 0x0400, 40, 0},
				//{ 0x0040, 40, 0}



				/*{ 0x0030, 30, 1},
				{ 0x0040, 40, 0},
				{ 0x0030, 30, 0},

				{ 0x0300, 30, 1},
				{ 0x0400, 40, 1},
				{ 0x0300, 30, 0},

				{ 0x3000, 30, 1},
				{ 0x4000, 40, 1},
				{ 0x3000, 30, 0},*/
				//{ 0x0040, 50, 0},
				//{ 0x8888, 20, 0},
/*1*/	        //{ 0x4444, 20, 1},
				//{ 0x2222, 80, 1},
				//{ 0x4444, 20, 0},
/*2*/	        //{ 0x8888, 90, 1},
/*3*/	        //{ 0x2222, 90, 0},
/*3*/	        //{ 0x4444, 100, 0},
/*3*/	        //{ 0x8888, 90, 0},
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
