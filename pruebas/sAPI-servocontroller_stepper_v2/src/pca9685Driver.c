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
 * Date: 2016-09-11
 */

/*==================[inclusions]=============================================*/

#include "pca9685Driver.h"         /* <= own header */

#include "sAPI.h"         /* <= sAPI header */

#include "math.h"         /* <= math header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

uint8_t _i2caddr;

/*==================[internal functions declaration]=========================*/

uint8_t read8(uint8_t addr);
void write8(uint8_t addr, uint8_t d);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

uint8_t read8(uint8_t addr) {
	uint8_t buf;
	i2cRead(I2C0, _i2caddr, addr, &buf, 1);
	return buf;
}

void write8(uint8_t addr, uint8_t d) {
	i2cWrite(I2C0, _i2caddr, addr, &d, 1);
}

/*==================[external functions definition]==========================*/

void PCA9685_begin(uint8_t addr) {
	i2cConfig(I2C0, 100000);
	_i2caddr = addr;
	PCA9685_reset();
}

void PCA9685_reset(void) {
	write8(PCA9685_MODE1, 0x0);
}

void PCA9685_setPWMFreq(float freq) {
	freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;

	uint8_t prescale = floor(prescaleval + 0.5);

	uint8_t oldmode = read8(PCA9685_MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	write8(PCA9685_MODE1, newmode); // go to sleep
	write8(PCA9685_PRESCALE, prescale); // set the prescaler
	write8(PCA9685_MODE1, oldmode);
	delay(5);
	write8(PCA9685_MODE1, oldmode | 0xa1); //  This sets the MODE1 register to turn on auto increment.
}

/* num tiene que ser entre 0 y 15 */
void PCA9685_setPWM(uint8_t num, uint16_t on, uint16_t off) {
	if (num >= 0 && num <= 15) {
		uint8_t buf[4];
		buf[0] = on;
		buf[1] = on >> 8;
		buf[2] = off;
		buf[3] = off >> 8;
		i2cWrite(I2C0, _i2caddr, LED0_ON_L + 4 * num, buf, 4);
	}
}

// Sets pin without having to deal with on/off tick placement and properly handles
// a zero value as completely off.  Optional invert parameter supports inverting
// the pulse for sinking to ground.  Val should be a value from 0 to 4095 inclusive.
void PCA9685_setPin(uint8_t num, uint16_t val, bool_t invert) {
	// Clamp value between 0 and 4095 inclusive.
	if (val > 4095)
		val = 4095;
	if (invert) {
		if (val == 0) {
			// Special value for signal fully on.
			PCA9685_setPWM(num, 4096, 0);
		} else if (val == 4095) {
			// Special value for signal fully off.
			PCA9685_setPWM(num, 0, 4096);
		} else {
			PCA9685_setPWM(num, 0, 4095 - val);
		}
	} else {
		if (val == 4095) {
			// Special value for signal fully on.
			PCA9685_setPWM(num, 4096, 0);
		} else if (val == 0) {
			// Special value for signal fully off.
			PCA9685_setPWM(num, 0, 4096);
		} else {
			PCA9685_setPWM(num, 0, val);
		}
	}
}

void PCA9685_shutdown() {
	/* sleep */
	//uint8_t oldmode = read8(PCA9685_MODE1);
	//uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	//write8(PCA9685_MODE1, newmode); // go to sleep

	/* reset */
	uint8_t addr = 0x00;
	uint8_t buff = 0x06;
	i2cWrite(I2C0, addr, buff, &buff, 0);
}

/*==================[end of file]============================================*/
