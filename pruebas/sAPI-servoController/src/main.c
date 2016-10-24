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

#include "sAPI.h"         /* <= sAPI header */

#include "servoController.h"        /* <= servoController header */

/*==================[macros and definitions]=================================*/

#define USED_SERVO 16

/*==================[internal data declaration]==============================*/

typedef void (*taskPointer_t)(void);

/*==================[internal functions declaration]=========================*/

void TASK_A(void);
void TASK_B(void);

/*==================[internal data definition]===============================*/

servo_t servos[USED_SERVO];
taskPointer_t task_A = TASK_A;
taskPointer_t task_B = TASK_B;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void TASK_A() {
	uint8_t i = 0;
	digitalWrite(LEDR, 0);
	digitalWrite(LEDB, 0);

	while (digitalRead(TEC1)) {
		digitalWrite(LEDG, 1);
		if (i == 0)
			i = 180;
		else
			i = 0;
		//for (i = 0; i < USED_SERVO; i++) {
		servoController_setServo(SERV15, i);
		//}
		delay(1000);
		servoController_refreshAll();
		digitalWrite(LEDG, 0);
		delay(5000);
	}

	//task_B();
}

void TASK_B() {
	digitalWrite(LEDG, 0);
	digitalWrite(LEDR, 0);

	while (digitalRead(TEC1)) {
		digitalWrite(LEDB, 1);
		delay(1000);
		digitalWrite(LEDB, 0);
		delay(1000);
	}

	//task_A();
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {
	uint8_t i;

	/* ------------- INICIALIZACIONES ------------- */

	/* Inicializar la placa */
	boardConfig();

	/* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
	tickConfig(1, 0);

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

	/* Definir 12 Servos */
	servos[0].servo = SERV0;
	servos[0].init_pos = 0;
	for (i = 1; i < USED_SERVO; i++) {
		servos[i].servo = i;
		servos[i].init_pos = 0;
	}

	/* Attach Servos */
	servoController_init(servos, USED_SERVO);

	/* Mover Servos a la posicion inicial */
	servoController_initialPosition();
	delay(2000);

	/* Usar Servo */
	//servoController_moveServo(SERV15, 180);
	/* Usar Output */
	digitalWrite(LED3, 1);

	delay(1000);
	int16_t n;

	/* -------------  INICIAR SCHEDULER  ------------- */

	while (digitalRead(TEC4)) {
		//task_A();
		//task_B();
		for (n = 0; n < USED_SERVO; n++) {
			servoController_moveServo(n, 0);
			delay(100);
		}
		delay(5000);
		for (n = 0; n < USED_SERVO; n++) {
			servoController_moveServo(n, 180);
			delay(100);
		}
		delay(5000);

	}

	/* ------------- FINALIZO  SCHEDULER ------------- */

	digitalWrite(LED3, 0);
	digitalWrite(LEDG, 0);
	digitalWrite(LEDB, 0);
	digitalWrite(LEDR, 1);
	while (1)
		;

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
