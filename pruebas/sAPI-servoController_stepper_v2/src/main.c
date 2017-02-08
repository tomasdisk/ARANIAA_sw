/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-07-03
 */

/*==================[inclusions]=============================================*/

#include "main.h"         /* <= own header */

#include "servoController.h"        /* <= servoController header */
#include "sAPI.h"         /* <= sAPI header */
#include "secuencer.h"


/*==================[macros and definitions]=================================*/

#define USED_SERVO 12

/*==================[internal data declaration]==============================*/
uint8_t count = 0;
bool_t end = 0;

/*==================[internal functions declaration]=========================*/
char* itoa(int value, char* result, int base) ;
void initIO();
void printS(uint8_t n_servo);
void printS_c(uint8_t n_servo);
void printS_for();
void moveServos();


void initServos();

/*==================[internal data definition]===============================*/

servo_t servos[USED_SERVO];
bool_t FLAG1 = 0;
uint16_t auxR = 0;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ =
				"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
						+ (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void initIO(){

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
/* FUNCION que se ejecuta cada vezque ocurre un Tick. */
bool_t myTickHook(void *ptr){
   FLAG1 = 1;
   count++;
   return 1;
}

void TASK1(){
	if(count == 50)
		uartWriteString(UART_USB, "Paso un segundo");
	end = servoController_refreshAll();
}

void printS(uint8_t n_servo){
	static uint8_t uartBuff[10];

	itoa(servos[n_servo].servo, uartBuff,10);
	uartWriteString(UART_USB, "Servo ");
	uartWriteString(UART_USB, uartBuff);
	uartWriteString(UART_USB, " pos: ");
	itoa(servoController_getServoPos(servos[n_servo].servo), uartBuff,10);
	uartWriteString(UART_USB, uartBuff);
	uartWriteString(UART_USB, "\r\n");


}

void printS_c(uint8_t n_servo){

	static uint8_t uartBuff[10];

	itoa(servos[n_servo].servo, uartBuff,10);
	uartWriteString(UART_USB, "Servo  ");
	uartWriteString(UART_USB, uartBuff);
	uartWriteString(UART_USB, "\r\n");
	uartWriteString(UART_USB, "pos: ");
	itoa(servoController_getServoPos(servos[n_servo].servo), uartBuff,10);
	uartWriteString(UART_USB, uartBuff);
	uartWriteString(UART_USB, "\r\n");
	itoa(servoController_getServoDuty(servos[n_servo].servo), uartBuff,10);
	uartWriteString(UART_USB, "duty: ");
	uartWriteString(UART_USB, uartBuff);
	uartWriteString(UART_USB, "\r\n");


}
void printS_for(){
	for(uint8_t i=0; i<USED_SERVO;i++){
			printS_c(i);
	}
}

void initServos(){

	/* Definir 12 Servos */
		//PATA 1
			//LONG
		servos[0].servo = SERV0;
		servos[0].init_pos = 50;
		servos[0].type = LON;
		servos[0].reverse = 0;
			//MID
		servos[1].servo = SERV1;
		servos[1].init_pos = 50;
		servos[1].type = MID;
		servos[1].reverse = 1;
			//SHORT
		servos[2].servo = SERV2;
		servos[2].init_pos = 50;
		servos[2].type = SHORT;
		//PATA 2
			//LONG
		servos[3].servo = SERV4;
		servos[3].init_pos = 50;
		servos[3].type = LON;
		servos[3].reverse = 1;
			//MID
		servos[4].servo = SERV5;
		servos[4].init_pos = 50;
		servos[4].type = MID;
			//SHORT
		servos[5].servo = SERV6;
		servos[5].init_pos = 50;
		servos[5].type = SHORT;
		servos[5].reverse = 1;
		//PATA 3
			//LONG
		servos[6].servo = SERV8;
		servos[6].init_pos = 50;
		servos[6].type = LON;
			//MID
		servos[7].servo = SERV9;
		servos[7].init_pos = 50;
		servos[7].type = MID;
		servos[7].reverse = 1;
			//SHORT
		servos[8].servo = SERV10;
		servos[8].init_pos = 50;
		servos[8].type = SHORT;
		servos[8].reverse = 1;
		//PATA 4
			//LONG
		servos[9].servo = SERV12;
		servos[9].init_pos = 50;
		servos[9].type = LON;
		servos[9].reverse = 1;
			//MID
		servos[10].servo = SERV13;
		servos[10].init_pos = 50;
		servos[10].type = MID;
			//SHORT
		servos[11].servo = SERV14;
		servos[11].init_pos = 50;
		servos[11].type = SHORT;

	/* Attach Servos */
	servoController_init(servos, USED_SERVO);

	/* Mover Servos a la posicion inicial */
	servoController_initialPosition();

}

void moveServos(){
	static uint8_t uartBuff[10];
	int8_t servoMover;
	int16_t angle;

	TypeSecuencia sec = secuencer_getSecuencia();

	for(uint8_t i = 0;i<4;i++){
		uint8_t servos = (sec.servos >> i*4) & 0x000F ;
		switch(servos){
		case 2:
			servoMover = SERVO0 + 4*i;
			break;
		case 4:
			servoMover = SERVO1 + 4*i;
			break;
		case 8:
			servoMover = SERVO2 + 4*i;
			break;
		}
		itoa(servoMover, uartBuff,10);
		uartWriteString(UART_USB, "La secuencia es ");
		uartWriteString(UART_USB, uartBuff);
		uartWriteString(UART_USB, "\r\n");
		angle = servoController_getServoPos(servoMover);
		if(sec.dir)
			angle += sec.mov;
		else
			angle -= sec.mov;
		if(angle > 100)   //Como los angulos son int8_t el valor maximo es 127 y no puede ajustarse dentro de servoController
			angle = 100; //No es necesario hacer la comprobacion angle < 0, se resuelve dentro de servoController
		servoController_setServo(servoMover,(int8_t)angle,2);
	}
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {
	uint8_t i;
	static uint8_t uartBuff[10];

	/* ------------- INICIALIZACIONES ------------- */

	/* Inicializar la placa */
	boardConfig();
	initIO();

	tickConfig( 1, 0 );
	//tickConfig(40, myTickHook );
	//delay(3000);
	//digitalWrite( LEDB, ON );
	/*Iniciar terminal*/
	uartConfig(UART_USB, 115200);
	initServos();
	//delay(2000);
	digitalWrite( LEDB, ON );
	digitalWrite(LED3, 1);
	delay(1000);
	uartWriteString(UART_USB, "inicio..");
	uartWriteString(UART_USB, "\r\n");
	printS_for();

	//for(i=0; i<USED_SERVO;i++){
	//	servoController_setServo(servos[i].servo,128,2);
	//		}
	moveServos();

	while (1) {

		delay(20);

		if(!servoController_ifEnd()){
			end = servoController_refreshAll();

			printS(SERV0);

			if(end){
				uartWriteString(UART_USB, "Termino");
				uartWriteString(UART_USB, "\r\n");
				printS_for();
				delay(1000);
				moveServos();
				}
		}
}
	/* ------------- FINALIZO  SCHEDULER ------------- */

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;

}
/*==================[end of file]============================================*/
