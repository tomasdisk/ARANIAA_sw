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
 * Date: 2016-09-12
 */

/*==================[inclusions]=============================================*/

#include "servoController.h"        /* <= own header */

#include "pca9685Driver.h"          /* <= PCA9685 driver header */

#include "sAPI.h"         /* <= sAPI header */

/*==================[macros and definitions]=================================*/

#define SERVO_TOTALNUMBER   25 /* 16 from PCA9685 and 9 from sAPI_Servo */

/* PCA9586 info */
#define PCA9685_0_ADDR 0x40
#define PCA9685_0_FREQ 50
#define PCA9685_0_POS0 144
#define PCA9685_0_POS180 470

/*Servo local address in the device who control it.*/
/* From PCA9685 */
#define SERV0_ADDR 0
#define SERV1_ADDR 1
#define SERV2_ADDR 2
#define SERV3_ADDR 3
#define SERV4_ADDR 4
#define SERV5_ADDR 5
#define SERV6_ADDR 6
#define SERV7_ADDR 7
#define SERV8_ADDR 8
#define SERV9_ADDR 9
#define SERV10_ADDR 10
#define SERV11_ADDR 11
#define SERV12_ADDR 12
#define SERV13_ADDR 13
#define SERV14_ADDR 14
#define SERV15_ADDR 15
/* From sAPI_Servo */
#define SERV16_ADDR SERVO0
#define SERV17_ADDR SERVO1
#define SERV18_ADDR SERVO2
#define SERV19_ADDR SERVO3
#define SERV20_ADDR SERVO4
#define SERV21_ADDR SERVO5
#define SERV22_ADDR SERVO6
#define SERV23_ADDR SERVO7
#define SERV24_ADDR SERVO8

/*==================[internal data declaration]==============================*/

typedef void (*controllerFunctionPointer_t)(uint8_t);

typedef struct {
	uint8_t addr; /*Servo local address in the device who control it.*/
	bool_t attached; /*If it is attached*/
	uint8_t angle;
	controllerFunctionPointer_t associatedFunction; /*Function who control it*/
	bool_t refresh;
	uint8_t init_pos; /*Natural position angle*/
} attachedServo_t;

/*==================[internal functions declaration]=========================*/

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max);
void controller_PCA9685_0(uint8_t n_servo);
void controller_sAPI_servo(uint8_t n_servo);

/*==================[internal data definition]===============================*/

static uint8_t use_sAPI=0;
static uint8_t use_PCA9685_0=0;

static attachedServo_t AttachedServoList[SERVO_TOTALNUMBER] = {
/*	position	| servo address	| attached	| angle	| associatedFunction |	refresh	| initial position*/
		/*0*/	{ SERV0_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*1*/	{ SERV1_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*2*/	{ SERV2_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*3*/	{ SERV3_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*4*/	{ SERV4_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*5*/	{ SERV5_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*6*/	{ SERV6_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*7*/	{ SERV7_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*8*/	{ SERV8_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*9*/	{ SERV9_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*10*/	{ SERV10_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*11*/	{ SERV11_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*12*/	{ SERV12_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*13*/	{ SERV13_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*14*/	{ SERV14_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*15*/	{ SERV15_ADDR	, 0 		, 0 	, controller_PCA9685_0	, 0 	, 0 },
		/*16*/	{ SERV16_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*17*/	{ SERV17_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*18*/	{ SERV18_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*19*/	{ SERV19_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*20*/	{ SERV20_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*21*/	{ SERV21_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*22*/	{ SERV22_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*23*/	{ SERV23_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
		/*24*/	{ SERV24_ADDR	, 0 		, 0 	, controller_sAPI_servo	, 0 	, 0 },
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void controller_PCA9685_0(uint8_t n_servo) {
	int16_t duty;
	duty = (int16_t) map((int32_t) AttachedServoList[n_servo].angle, 0, 180,
			(int32_t) PCA9685_0_POS0, (int32_t) PCA9685_0_POS180);
	PCA9685_setPWM(AttachedServoList[n_servo].addr, 0, duty);
}

void controller_sAPI_servo(uint8_t n_servo) {
	servoWrite(AttachedServoList[n_servo].addr,
			AttachedServoList[n_servo].angle);
}

/*==================[external functions definition]==========================*/

void servoController_init(servo_t* servos, uint8_t num) {
	uint8_t i;

	/* attach servos and define their initial position */
	for(i=0; i<num;i++){
		AttachedServoList[servos[i].servo].attached=1;
		AttachedServoList[servos[i].servo].init_pos=servos[i].init_pos;
		if(servos[i].servo < SERV16){
			use_PCA9685_0=1;
		}else{
			use_sAPI=1;
		}
	}
	/* init servos del 0 al 15 -> controlados por PCA9685_0 */
	if(use_PCA9685_0){
		PCA9685_begin(PCA9685_0_ADDR);
		PCA9685_setPWMFreq(PCA9685_0_FREQ);
	}
	/* init servos del 16 al 24 -> controlados por sAPI_Servo */
	if(use_sAPI){
		servoConfig(0, ENABLE_SERVO_TIMERS);
		for(i=SERV16; i<=SERV24;i++){
			if(AttachedServoList[i].attached){
				servoConfig(AttachedServoList[i].addr, ENABLE_SERVO_OUTPUT);
			}
		}
	}
}

void servoController_initialPosition() {
	uint8_t n_servo;
	for (n_servo = 0; n_servo < SERVO_TOTALNUMBER; n_servo++) {
		if (AttachedServoList[n_servo].attached) {
			AttachedServoList[n_servo].angle = AttachedServoList[n_servo].init_pos;
			AttachedServoList[n_servo].associatedFunction(n_servo);
			AttachedServoList[n_servo].refresh = 0;
		}
	}
}

void servoController_setServo(uint8_t n_servo, uint8_t angle) {
	if (AttachedServoList[n_servo].attached) {
		AttachedServoList[n_servo].angle = angle;
		AttachedServoList[n_servo].refresh = 1;
	}
}

void servoController_moveServo(uint8_t n_servo, uint8_t angle) {
	if (AttachedServoList[n_servo].attached) {
		AttachedServoList[n_servo].angle = angle;
		AttachedServoList[n_servo].associatedFunction(n_servo);
		AttachedServoList[n_servo].refresh = 0;
	}
}
void servoController_refreshAll() {
	uint8_t n_servo;
	for (n_servo = 0; n_servo < SERVO_TOTALNUMBER; n_servo++) {
		if (AttachedServoList[n_servo].refresh) {
			AttachedServoList[n_servo].associatedFunction(n_servo);
			AttachedServoList[n_servo].refresh = 0;
		}
	}
}

void servoController_finalize() {
	uint8_t i;

	/* apagar servos del 0 al 15 -> controlados por PCA9685_0 */
	if(use_PCA9685_0){
		PCA9685_shutdown();
		use_PCA9685_0=0;
	}
	/* apagar servos del 16 al 24 -> controlados por sAPI_Servo */
	if(use_sAPI){
		for(i=SERV16; i<=SERV24;i++){
			if(AttachedServoList[i].attached){
				servoConfig(AttachedServoList[i].addr, DISABLE_SERVO_OUTPUT);
			}
		}
		servoConfig(0, DISABLE_SERVO_TIMERS);
		use_sAPI=0;
	}
	/* detach servos and reset their initial position */
	for(i=0; i<SERVO_TOTALNUMBER;i++){
		AttachedServoList[i].attached=0;
		AttachedServoList[i].init_pos=0;
	}
}

/*==================[end of file]============================================*/
