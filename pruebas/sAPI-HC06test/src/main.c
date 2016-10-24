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
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "main.h"         /* <= own header */

#include "sAPI.h"         /* <= sAPI header */

#include "pca9685Driver.h"         /* <= PCA9685 driver header */

/*==================[macros and definitions]=================================*/

#define PCA9685_ADDR 0x40

/*==================[internal data declaration]==============================*/

uint16_t pos0 = 144; // ancho de pulso en cuentas para pocicion 0°
uint16_t pos180 = 470; // ancho de pulso en cuentas para la pocicion 180°

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
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

	/* Inicializar UART_232 a 115200 baudios */
	uartConfig(UART_232, 9600);

	/* Configurar Servo */
	PCA9685_begin(PCA9685_ADDR);
	PCA9685_setPWMFreq(50);

	uint8_t dato = 0;
//	uint8_t dato1 = 1;
//	uint8_t dato2 = 78;
//	int32_t dato3 = 1234;

	/* Buffer */
	static uint8_t uartBuff[10];

//   uartWriteByte( UART_232, 'h' - 32 );   /* Envía 'H' */
//   uartWriteByte( UART_232, 'A' + 32 );   /* Envía 'a' */
//
//   /* Enviar un Enter */
//   uartWriteByte( UART_232, '\r' ); /* Envía '\r', retorno de carro */
//   uartWriteByte( UART_232, '\n' ); /* Envía '\n', nueva línea      */
//
//   uartWriteByte( UART_232, dato1 + 48 ); /* Envía '1' */
//   uartWriteByte( UART_232, ' ' );        /* Envía ' ' */
//   uartWriteByte( UART_232, '1' );        /* Envía '1' */
//   uartWriteByte( UART_232, 32 );         /* Envía ' ' */
//
//   /* Convertir un número entero de 2 dígitos ASCII y enviar */
//   uartWriteByte( UART_232, (dato2/10) + 48 ); /* Envía '7' */
//   uartWriteByte( UART_232, (dato2%10) + 48 ); /* Envía '8' */
//
//   uartWriteString( UART_232, "\r\n" ); /* Enviar un Enter */
//
//   uartWriteByte( UART_232, 'H' );  /* Envía 'H' */
//   uartWriteByte( UART_232, 'o' );  /* Envía 'o' */
//   uartWriteByte( UART_232, 'l' );  /* Envía 'l' */
//   uartWriteByte( UART_232, 'a' );  /* Envía 'a' */
//   uartWriteByte( UART_232, '\r' ); /* Envía '\r', retorno de carro */
//   uartWriteByte( UART_232, '\n' ); /* Envía '\n', nueva línea      */
//
//   uartWriteString( UART_232, "Chau\r\n" ); /* Envía "Chau\r\n" */
//
//   uint8_t miTexto[] = "Hola de nuevo\r\n";
//
//   uartWriteString( UART_232, miTexto ); /* Envía "Hola de nuevo\r\n" */
//
//   miTexto[0] = 'h';
//   uartWriteString( UART_232, miTexto ); /* Envía "hola de nuevo\r\n" */
//
//   /* Conversión de muestra entera a ascii con base decimal usando itoa() */
//   itoa( dato3, uartBuff, 10 ); /* base 10 significa decimal */
//   uartWriteString( UART_232, uartBuff );
//
//   uartWriteString( UART_232, "\r\n" ); /* Enviar un Enter */

	/* ------------- REPETIR POR SIEMPRE ------------- */
	while (1) {

		/* Recibir byte de la UART_232 y guardarlo en la variable dato */
		dato = uartReadByte(UART_232);

		/* Si el byte recibido es distinto de 0 (caracter NULL) se reenvía
		 a la UART_232 realizando un eco de lo que llega */
		if (dato) {
			//uartWriteByte( UART_232, dato );
			if (dato == '1') {
				PCA9685_setPWM(8, 0, pos180);
			}
			if (dato == '0') {
				PCA9685_setPWM(8, 0, pos0);
			}
		}

	}

	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
