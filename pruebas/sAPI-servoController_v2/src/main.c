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

#include "hc06_driver.h"		/* <= hc06 bluetooth driver */

/*==================[macros and definitions]=================================*/

#define USED_SERVO 3

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

servo_t servos[USED_SERVO];

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

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {
	static uint8_t uartBuff[10];

	/* ------------- INICIALIZACIONES ------------- */

	/* Inicializar la placa */
	boardConfig();

	/* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
	tickConfig(1, 0);

	/*Iniciar terminal*/
	uartConfig(UART_USB, 115200);
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

	/* iniciar hc06 bluetooth */
	HC06_init(115200);

	/* Definir 12 Servos */
	servos[0].servo = SERV4;
	servos[0].init_pos = 90;
	servos[1].servo = SERV5;
	servos[1].init_pos = 90;
	servos[2].servo = SERV6;
	servos[2].init_pos = 90;

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
	uartWriteString(UART_USB, "inicio..");
	int16_t dato = 90, angle, angle_leg = 90, angle_mid = 90, angle_sho = 90;

	/* -------------  INICIAR SCHEDULER  ------------- */

	while (1) {

		dato = HC06_ReadByte();
		if (dato) {
			switch (dato) {

			case 'q':
				angle_leg++;
				if (angle_leg > 180)
					angle_leg = 180;
				angle = angle_leg;
				servoController_moveServo(SERV4, (uint8_t)angle_leg);
				uartWriteString(UART_USB, "leg angle change:");
				HC06_WriteString("leg angle change:");
				break;

			case 'w':
				angle_leg--;
				if (angle_leg < 0)
					angle_leg = 0;
				angle = angle_leg;
				servoController_moveServo(SERV4, (uint8_t)angle_leg);
				uartWriteString(UART_USB, "leg angle change:");
				HC06_WriteString("leg angle change:");
				break;

			case 'r':
				angle_mid++;
				if (angle_mid > 180)
					angle_mid = 180;
				angle = angle_mid;
				servoController_moveServo(SERV5, (uint8_t)angle_mid);
				uartWriteString(UART_USB, "mid angle change:");
				HC06_WriteString("mid angle change:");
				break;

			case 't':
				angle_mid--;
				if (angle_mid < 0)
					angle_mid = 0;
				angle = angle_mid;
				servoController_moveServo(SERV5, (uint8_t)angle_mid);
				uartWriteString(UART_USB, "mid angle change:");
				HC06_WriteString("mid angle change:");
				break;

			case 'u':
				angle_sho++;
				if (angle_sho > 180)
					angle_sho = 180;
				angle = angle_sho;
				servoController_moveServo(SERV6, (uint8_t)angle_sho);
				uartWriteString(UART_USB, "sho angle change:");
				HC06_WriteString("sho angle change:");
				break;

			case 'i':
				angle_sho--;
				if (angle_sho < 0)
					angle_sho = 0;
				angle = angle_sho;
				servoController_moveServo(SERV6, (uint8_t)angle_sho);
				uartWriteString(UART_USB, "sho angle change:");
				HC06_WriteString("sho angle change:");
				break;

			default:
				dato = 0;
				break;
			}

			itoa(angle, uartBuff, 10); /* base 10 significa decimal */
			uartWriteString(UART_USB, uartBuff);
			HC06_WriteString(uartBuff);
			uartWriteString(UART_USB, "\r\n");
			HC06_WriteString("\r\n");
			delay(200);
		}
	}

	/* ------------- FINALIZO  SCHEDULER ------------- */

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
