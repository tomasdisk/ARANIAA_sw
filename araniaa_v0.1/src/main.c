/* Copyright 2017, ARANIAA_Team.
 *
 */

/*==================[inclusions]=============================================*/

#include "main.h"         /* <= own header */

#include "servoController.h"		/* <= servoController header */
#include "sAPI.h"					/* <= sAPI header */
#include "hc06_driver.h"
#include "secuencer.h"				/* <= secuencer header */

/*==================[macros and definitions]=================================*/

#define USED_SERVO 12

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void initIO();
void initServos();
void decodeOrder(uint8_t order);
void nextSecuence();

/*==================[internal data definition]===============================*/

servo_t servos[USED_SERVO];
bool_t refresh_end = 0;
bool_t flag_sleep = 1;
bool_t flag_move = 0;
bool_t flag_vervose = 0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void initIO() {

	/* Inicializar DigitalIO */
	digitalConfig(0, ENABLE_DIGITAL_IO);

	/* Configuración de pines de entrada para Teclas de la CIAA-NXP */
	digitalConfig(TEC1, INPUT);
	digitalConfig(TEC2, INPUT);
	digitalConfig(TEC3, INPUT);
	digitalConfig(TEC4, INPUT);

	/* Configuración de pines de salida para Leds de la CIAA-NXP */
	digitalConfig(LEDR, OUTPUT);
	digitalConfig(LEDG, OUTPUT);
	digitalConfig(LEDB, OUTPUT);
	digitalConfig(LED1, OUTPUT);
	digitalConfig(LED2, OUTPUT);
	digitalConfig(LED3, OUTPUT);

}

void initServos() {

	/* Definir 12 Servos */
	//PATA 1 --------------------------------------------------------
	//LONG
	servos[0].servo = SERV0;
	servos[0].init_pos = 80;
	servos[0].type = LON;
	servos[0].reverse = 0;
	//MID
	servos[1].servo = SERV1;
	servos[1].init_pos = 20;
	servos[1].type = MID;
	servos[1].reverse = 1;
	//SHORT
	servos[2].servo = SERV2;
	servos[2].init_pos = 84;
	servos[2].type = SHORT;
	servos[2].reverse = 1;
	//PATA 2 --------------------------------------------------------
	//LONG
	servos[3].servo = SERV4;
	servos[3].init_pos = 84;
	servos[3].type = LON;
	servos[3].reverse = 1;
	//MID
	servos[4].servo = SERV5;
	servos[4].init_pos = 20;
	servos[4].type = MID;
	servos[5].reverse = 0;
	//SHORT
	servos[5].servo = SERV6;
	servos[5].init_pos = 90;
	servos[5].type = SHORT;
	servos[5].reverse = 0;
	//PATA 3 --------------------------------------------------------
	//LONG
	servos[6].servo = SERV8;
	servos[6].init_pos = 80;
	servos[6].type = LON;
	servos[5].reverse = 0;
	//MID
	servos[7].servo = SERV9;
	servos[7].init_pos = 20;
	servos[7].type = MID;
	servos[7].reverse = 1;
	//SHORT
	servos[8].servo = SERV10;
	servos[8].init_pos = 80;
	servos[8].reverse = 1;
	//PATA 4 --------------------------------------------------------
	//LONG
	servos[9].servo = SERV12;
	servos[9].init_pos = 80;
	servos[9].type = LON;
	servos[9].reverse = 1;
	//MID
	servos[10].servo = SERV13;
	servos[10].init_pos = 20;
	servos[10].type = MID;
	servos[5].reverse = 0;
	//SHORT
	servos[11].servo = SERV14;
	servos[11].init_pos = 85;
	servos[11].type = SHORT;
	servos[11].reverse = 0;

	/* Attach Servos */
	servoController_init(servos, USED_SERVO);

	/* Mover Servos a la posicion inicial */
	servoController_initialPosition();

}

void decodeOrder(uint8_t order) {

	switch (order) {
	case 'a':
		if (!flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_ABAJO);
			flag_sleep = 1;
			flag_move = 1;
		}
		break;
	case 'b':
		if (flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_ARRIBA);
			flag_sleep = 0;
			flag_move = 1;
		}
		break;
	case 'c':
		if (!flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_ADELANTE);
			flag_move = 1;
		}
		break;
	case 'd':
		if (!flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_ATRAS);
			flag_move = 1;
		}
		break;
	case 'e':
		if (!flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_GIRAR_DER);
			flag_move = 1;
		}
		break;
	case 'f':
		if (!flag_sleep && !flag_move) {
			secuencer_setSeciencia(SEC_GIRAR_IZQ);
			flag_move = 1;
		}
		break;
	case 'v':
		flag_vervose = !flag_vervose;
		break;
	default:;
		//TODO completar ordenes

	}
}

void nextSecuence() {
	uint8_t i;
	int16_t angle;
	TypeSecuencia sec = secuencer_getSecuencia();

	if (sec.servos == 0xFFFF) {
		flag_move = !flag_move;
	} else if (flag_move) {
		for (i = 0; i < 16; i++) {
			if ((i + 1) % 4 != 0 && (sec.servos >> i) % 2 == 1) {
				angle = servoController_getServoPos(i);
				if ((sec.dir >> i) % 2)
					angle += sec.mov;
				else
					angle -= sec.mov;
				//TODO verificar cotas que siguen
				if (angle > 100) //Como los angulos son int8_t el valor maximo es 127 y no puede ajustarse dentro de servoController
					angle = 100; //No es necesario hacer la comprobacion angle < 0, se resuelve dentro de servoController

				servoController_setServo(i, (int8_t) angle, 2);
			}
		}
	}
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {
	/* ------------- INICIALIZACIONES ------------- */

	/* Inicializar la placa */
	boardConfig();
	/* Inicializar IO */
	initIO();
	/*Iniciar terminal*/
	uartConfig(UART_USB, 115200);
	/* Inicializar bluetooth */
	HC06_init(115200); //TODO
	/*Iniciar servos*/
	initServos();

	digitalWrite(LEDB, ON);
	digitalWrite(LED3, 1);
	delay(1000);

	/* ----------- FIN INICIALIZACIONES ----------- */

	while (1) {
		/* ------- Nueva orden ------- */
		decodeOrder(HC06_ReadByte());

		/* ------- */
		/* ------- Ejecutar ordenes ------- */

		/* ------- */
		/* ------- Ejecutar tareas ------- */
		if (refresh_end) {
			nextSecuence();
		}
		refresh_end = servoController_refreshAll();

		/* ------- */

	}
	/* ------------- FINALIZO  SCHEDULER ------------- */

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;

}
/*==================[end of file]============================================*/
