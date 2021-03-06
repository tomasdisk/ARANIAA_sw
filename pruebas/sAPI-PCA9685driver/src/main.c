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

#include "pca9685Driver.h"         /* <= PCA9685 driver header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

uint16_t pos0 = 144; // ancho de pulso en cuentas para pocicion 0°
uint16_t pos180 = 470; // ancho de pulso en cuentas para la pocicion 180°

uint16_t posActual = 144;

/*==================[internal functions declaration]=========================*/

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max);

void setServo(uint8_t n_servo, int16_t angulo);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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

void setServo(uint8_t n_servo, int16_t angulo) {
	int16_t duty;
	duty = (int16_t) map((int32_t) angulo, 0, 180, (int32_t) pos0,
			(int32_t) pos180);
	PCA9685_setPWM(n_servo, 0, duty);
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

	/* Configurar Servo */
	PCA9685_begin(PCA9685_ADDR);
	PCA9685_setPWMFreq(50);

	/* Usar Servo */

	/* Usar Output */
	digitalWrite(LED3, 1);

	int16_t n;
	int16_t duty;
	uint8_t dato = 0;

	itoa( posActual, uartBuff, 10 ); /* base 10 significa decimal */
	uartWriteString( UART_USB, uartBuff );

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while (1) {
		//for (duty = pos0; duty < pos180; duty = duty + 10) {
		dato = uartReadByte(UART_USB);
	    if (dato) {
	    			//uartWriteByte( UART_232, dato );
	    			if (dato == '1') {
	    				posActual = posActual + 1;
	    			}
	    			if (dato == '0') {
	    				posActual = posActual - 1;
	    			}
	    			itoa( posActual, uartBuff, 10 ); /* base 10 significa decimal */
	    			uartWriteString( UART_USB, uartBuff );
	    			uartWriteString( UART_USB, "\r\n" );
	    			PCA9685_setPWM(0, 0, posActual);
	    		}

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
