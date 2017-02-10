/* Copyright 2017, ARANIAA_Team.
 *
 */

/*==================[inclusions]=============================================*/

#include "secuencer.h"			/* <= own header */

#include "sAPI.h"				/* <= sAPI header */

/*==================[macros and definitions]=================================*/
#define ADELANTE_CANT 1
#define ATRAS_CANT 1
#define GIRAR_DER_CANT 15
#define GIRAR_IZQ_CANT 1
#define ARRIBA_CANT 1
#define ABAJO_CANT 1

/*==================[internal data declaration]==============================*/
static uint8_t actual_sec_count = 0;
static uint8_t actual_count = 0;
static const TypeSecuencia *actual_sec;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*    LEG3  LEG2  LEG1  LEG0
 * 0b(0SML)(0SML)(0SML)(0SML) --> 0x(7)(7)(7)(7)
 *
 * 0b0SML0SML0SML0SML --> 0x7777
 * 0b0SML0SML0SML0SM0 --> 0x7776
 * 0b0SML0SML0SML0S0L --> 0x7775
 * 0b0SML0SML0SML0S00 --> 0x7774
 * 0b0SML0SML0SML00ML --> 0x7773
 * 0b0SML0SML0SML00M0 --> 0x7772
 * 0b0SML0SML0SML000L --> 0x7771
 * 0b0SML0SML0SML0000 --> 0x7770
 *
 * 0xFFFF --> secuencia inicio/fin
 * */

static const TypeSecuencia sec_girar_der[GIRAR_DER_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};
//TODO
static const TypeSecuencia sec_girar_izq[GIRAR_IZQ_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};
//TODO
static const TypeSecuencia sec_arriba[ARRIBA_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};
//TODO
static const TypeSecuencia sec_abajo[ABAJO_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};
//TODO
static const TypeSecuencia sec_adelante[ADELANTE_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};
//TODO
static const TypeSecuencia sec_atras[ATRAS_CANT] = {
/*01*/{ 0xFFFF, 00, 0x0000 },
/*02*/{ 0x0002, 20, 0x0002 },
/*03*/{ 0x0004, 40, 0x0004 },
/*04*/{ 0x0002, 20, 0x0000 },
/*05*/{ 0x0020, 20, 0x0020 },
/*06*/{ 0x0040, 40, 0x0000 },
/*07*/{ 0x0020, 20, 0x0000 },
/*08*/{ 0x0200, 20, 0x0200 },
/*09*/{ 0x0400, 40, 0x0400 },
/*10*/{ 0x0200, 20, 0x0000 },
/*11*/{ 0x2000, 20, 0x2000 },
/*12*/{ 0x4000, 40, 0x0000 },
/*13*/{ 0x2000, 20, 0x0000 },
/*14*/{ 0x4444, 40, 0x4040 },
/*15*/{ 0xFFFF, 00, 0xFFFF },
/**/};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void secuencer_setSeciencia(uint8_t sec) {

	switch (sec) {
	case SEC_ABAJO:
		actual_sec = sec_abajo;
		actual_sec_count = ABAJO_CANT;
		break;
	case SEC_ADELANTE:
		actual_sec = sec_adelante;
		actual_sec_count = ADELANTE_CANT;
		break;
	case SEC_ARRIBA:
		actual_sec = sec_arriba;
		actual_sec_count = ARRIBA_CANT;
		break;
	case SEC_ATRAS:
		actual_sec = sec_atras;
		actual_sec_count = ATRAS_CANT;
		break;
	case SEC_GIRAR_DER:
		actual_sec = sec_girar_der;
		actual_sec_count = GIRAR_DER_CANT;
		break;
	case SEC_GIRAR_IZQ:
		actual_sec = sec_girar_izq;
		actual_sec_count = GIRAR_IZQ_CANT;
		break;
		;
	}

}

TypeSecuencia secuencer_getSecuencia() {
	TypeSecuencia aux = actual_sec[actual_count];
	actual_count++;
	if (actual_count == actual_sec_count)
		actual_count = 0;
	return aux;
}

void secuencer_reset() {
	actual_count = 0;
}

uint8_t secuencer_getCount() {
	return actual_count;
}
/*==================[end of file]============================================*/
