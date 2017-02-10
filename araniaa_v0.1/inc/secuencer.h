/* Copyright 2017, ARANIAA_Team.
 *
 */

#ifndef _SECUENCER_H_
#define _SECUENCER_H_

/*==================[inclusions]=============================================*/

#include "sAPI_DataTypes.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define SEC_ADELANTE 1
#define SEC_ATRAS 2
#define SEC_GIRAR_DER 3
#define SEC_GIRAR_IZQ 4
#define SEC_ARRIBA 5
#define SEC_ABAJO 6

/*==================[typedef]================================================*/
typedef struct {
	uint16_t servos;
	uint8_t mov;
	uint16_t dir;
} TypeSecuencia;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void secuencer_setSeciencia(uint8_t sec);
TypeSecuencia secuencer_getSecuencia();
void secuencer_reset();
uint8_t secuencer_getCount();


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SECUENCER_ */
