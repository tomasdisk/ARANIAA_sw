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

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {

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

	uint8_t servoAngle = 0; /* 0 a 180 grados */
	uint8_t servoAngleNow = 0; /* 0 a 180 grados */

	/* Configurar Servo */
	servoConfig(0, ENABLE_SERVO_TIMERS);

	servoConfig(SERVO0, ENABLE_SERVO_OUTPUT);

	/* Usar Servo */
	servoWrite(SERVO0, servoAngle);
	servoAngle = servoRead(SERVO0);

	digitalWrite(LED3, 1);

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while (1) {

		if (!digitalRead(TEC2)) {
			servoAngle += 30;
			digitalWrite(LEDB, 1);
			while (digitalRead(TEC1))
				;
			digitalWrite(LEDB, 0);
		}
		if (!digitalRead(TEC3)) {
			servoAngle -= 30;
			digitalWrite(LEDG, 1);
			while (digitalRead(TEC1))
				;
			digitalWrite(LEDG, 0);
		}
		if (servoAngle >= 180) {
			servoAngle = 180;
			digitalWrite(LED1, 1);
		} else {
			digitalWrite(LED1, 0);
		}
		if (servoAngle <= 0) {
			servoAngle = 0;
			digitalWrite(LED2, 1);
		} else {
			digitalWrite(LED2, 0);
		}

		delay(1000);
		if (servoAngle != servoAngleNow) {
			servoAngleNow = servoAngle;
			servoWrite(SERVO0, servoAngle);
		}

//	   servoWrite( SERVO0, 0 );		delay(1000);
//	   servoWrite( SERVO0, 10 );	delay(1000);
//	   servoWrite( SERVO0, 20 );	delay(1000);
//	   servoWrite( SERVO0, 30 );	delay(1000);
//	   servoWrite( SERVO0, 40 );	delay(1000);
//	   servoWrite( SERVO0, 50 );	delay(1000);
//	   servoWrite( SERVO0, 60 );	delay(1000);
//	   servoWrite( SERVO0, 70 );	delay(1000);
//	   servoWrite( SERVO0, 80 );	delay(1000);
//	   servoWrite( SERVO0, 90 );	delay(1000);
//	   servoWrite( SERVO0, 100 );	delay(1000);
//	   servoWrite( SERVO0, 110 );	delay(1000);
//	   servoWrite( SERVO0, 120 );	delay(1000);
//	   servoWrite( SERVO0, 130 );	delay(1000);
//	   servoWrite( SERVO0, 140 );	delay(1000);
//	   servoWrite( SERVO0, 150 );	delay(1000);
//	   servoWrite( SERVO0, 160 );	delay(1000);
//	   servoWrite( SERVO0, 170 );	delay(1000);
//	   servoWrite( SERVO0, 180 );	delay(1000);
//	   servoWrite( SERVO0, 170 );	delay(1000);
//	   servoWrite( SERVO0, 160 );	delay(1000);
//	   servoWrite( SERVO0, 150 );	delay(1000);
//	   servoWrite( SERVO0, 140 );	delay(1000);
//	   servoWrite( SERVO0, 130 );	delay(1000);
//	   servoWrite( SERVO0, 120 );	delay(1000);
//	   servoWrite( SERVO0, 110 );	delay(1000);
//	   servoWrite( SERVO0, 100 );	delay(1000);
//	   servoWrite( SERVO0, 90 );	delay(1000);
//	   servoWrite( SERVO0, 80 );	delay(1000);
//	   servoWrite( SERVO0, 70 );	delay(1000);
//	   servoWrite( SERVO0, 60 );	delay(1000);
//	   servoWrite( SERVO0, 50 );	delay(1000);
//	   servoWrite( SERVO0, 40 );	delay(1000);
//	   servoWrite( SERVO0, 30 );	delay(1000);
//	   servoWrite( SERVO0, 20 );	delay(1000);
//	   servoWrite( SERVO0, 10 );	delay(1000);

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
