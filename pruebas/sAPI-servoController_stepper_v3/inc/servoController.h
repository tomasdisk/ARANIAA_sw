/* Copyright 2015-2016, Eric Pernia.
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

#ifndef _SERVOCONTROLLER_H_
#define _SERVOCONTROLLER_H_

/*==================[inclusions]=============================================*/

#include "sAPI_DataTypes.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define SERVO_TOTAL_NUMBER   16 /* 16 from PCA9685 and 9 from sAPI_Servo */

/*==================[typedef]================================================*/

/*Defined for servoController.h*/
typedef enum {
	SERV0,
	SERV1,
	SERV2,
	SERV3,
	SERV4,
	SERV5,
	SERV6,
	SERV7,
	SERV8,
	SERV9,
	SERV10,
	SERV11,
	SERV12,
	SERV13,
	SERV14,
	SERV15,
} ServoControllerMap_t;

typedef enum {
	NORMAL,
	LON,
	MID,
	SHORT,
} ServoType;

typedef struct {
	ServoControllerMap_t servo; /*Servo to use. From ServoControllerMap_t*/
	int8_t init_pos; /*Natural position angle*/
	ServoType type;
	bool_t reverse;
} servo_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void servoController_init(servo_t* servos,  uint8_t num);
void servoController_initialPosition();

void servoController_moveInstantServo(uint8_t n_servo, int8_t angle) ;
void servoController_setServo(uint8_t n_servo, int8_t angle,uint8_t step);
void servoController_stop();
void servoController_start();
bool_t servoController_refreshAll();
bool_t servoController_ifEnd();

uint8_t servoController_getServoPos(uint8_t n_servo);
uint16_t servoController_getServoDuty(uint8_t n_servo);
ServoType servoController_getType(uint8_t n_servo);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SERVOCONTROLLER_H_ */
