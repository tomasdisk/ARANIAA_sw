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

/* PCA9586 info */
#define PCA9685_ADDR 0x40
#define PCA9685_FREQ 50
#define PCA9685_POS0 125 //limit ok
#define PCA9685_POS180 460 //limit ok

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


/*==================[internal data declaration]==============================*/

//typedef void (*controllerFunctionPointer_t)(uint8_t);

typedef struct {
	uint8_t addr; /*Servo local address in the device who control it.*/
	bool_t attached; /*If it is attached*/
	int16_t angle; /*Servo angle in duty circle. From 0º to 180º*/
	int16_t angle_end; /*Servo angle_end in duty circle. From 0º to 180º*/
	uint8_t dir; /*Direction of move*/
	//controllerFunctionPointer_t associatedFunction; /*Function who control it*/
	bool_t refresh;
	uint16_t init_pos; /*Natural position angle in duty circle*/
	uint8_t step;
	ServoType type;
	bool_t reverse;
} attachedServo_t;

typedef struct {
	uint16_t duty_min;
	uint16_t duty_max;
	//ServoType type;
} TypeLimits;



/*==================[internal functions declaration]=========================*/

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min,int16_t out_max);
void validateLimits(uint16_t n_servo);
void validateStep(uint8_t n_servo);
uint16_t setDuty(uint8_t n_servo,int8_t angle);
void servoController_moveServo(uint8_t n_servo);

/*==================[internal data definition]===============================*/

//static char end;

static attachedServo_t ServoList[SERVO_TOTAL_NUMBER] = {
/*	position	| servo address	| attached	| angle  		     | angle_end     |dir	   |refresh	    | init pos|step |ServoType| reverse*/
/*0*/	        { SERV0_ADDR	  , 0 		    , PCA9685_POS0 	 , 0	 	      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*1*/	        { SERV1_ADDR	  , 0 		    , PCA9685_POS0 	 , 0	 	      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*2*/	        { SERV2_ADDR	  , 0 		    , PCA9685_POS0	 , 0 		      , 0      , 0 	  	    , 0		  , 1   ,NORMAL   ,0},
/*3*/	        { SERV3_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*4*/	        { SERV4_ADDR	  , 0 		    , PCA9685_POS0	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*5*/	        { SERV5_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*6*/	        { SERV6_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*7*/	        { SERV7_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*8*/	        { SERV8_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	    	, 0 	  , 1   ,NORMAL   ,0},
/*9*/	        { SERV9_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0      , 0 	  	    , 0		  , 1   ,NORMAL   ,0},
/*10*/	        { SERV10_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0      , 0 			, 0       , 1   ,NORMAL   ,0},
/*11*/	        { SERV11_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	  		, 0 	  , 1   ,NORMAL   ,0},
/*12*/	        { SERV12_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	  		, 0 	  , 1   ,NORMAL   ,0},
/*13*/	        { SERV13_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	  		, 0	      , 1   ,NORMAL   ,0},
/*14*/	        { SERV14_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	  		, 0	      , 1   ,NORMAL   ,0},
/*15*/	        { SERV15_ADDR	  , 0 		    , PCA9685_POS0 	 , 0		      , 0	   , 0 	  		, 0 	  , 1   ,NORMAL   ,0},
};
static TypeLimits LimitsList[4] = {
/*	TYPE	| Duty min	| Duty max */
/*0*/	        { 130	  , 460	},
/*L*/	        { 190	  , 460 },
/*M*/	        { 214	  , 450 },
/*S*/	        { 230	  , 340 },
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min,int16_t out_max) {
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void validateLimits(uint16_t n_servo){

	if(ServoList[n_servo].dir && ServoList[n_servo].angle >= ServoList[n_servo].angle_end){
		ServoList[n_servo].angle = ServoList[n_servo].angle_end;
		ServoList[n_servo].refresh = 0;

	}else
		if(!ServoList[n_servo].dir && ServoList[n_servo].angle <= ServoList[n_servo].angle_end){
				ServoList[n_servo].angle = ServoList[n_servo].angle_end;
				ServoList[n_servo].refresh = 0;
			}

}

void validateStep(uint8_t n_servo){

	if(ServoList[n_servo].step < 1)
		ServoList[n_servo].step = 1;
	else
		if(ServoList[n_servo].step > 8)
			ServoList[n_servo].step = 8;

}

uint16_t setDuty(uint8_t n_servo,int8_t angle){
	uint16_t max;
	uint16_t min;
	int16_t duty;

	if(angle<0) angle = 0;
	if(angle>100) angle = 100;

	max = LimitsList[ServoList[n_servo].type].duty_max;
	min = LimitsList[ServoList[n_servo].type].duty_min;
	if(!ServoList[n_servo].reverse)
		duty = map((int16_t) angle, 0, 100,min, max);
	else
		duty = map((int16_t) angle, 0, 100,max, min);
	return duty;
}

void servoController_moveServo(uint8_t n_servo) {
	if (ServoList[n_servo].attached && ServoList[n_servo].refresh) {
		if(ServoList[n_servo].dir){
			ServoList[n_servo].angle += ServoList[n_servo].step;
		}else{
			ServoList[n_servo].angle -= ServoList[n_servo].step;
		}
		validateLimits(n_servo);
		PCA9685_setPWM(ServoList[n_servo].addr, 0, ServoList[n_servo].angle);
	}
}

/*==================[external functions definition]==========================*/

void servoController_init(servo_t* servos, uint8_t num) {
	uint8_t i;

	/* attach servos and define their initial position */
	for(i=0; i<num;i++){
		ServoList[servos[i].servo].attached=1;
		ServoList[servos[i].servo].type = servos[i].type;
		if(servos[i].reverse)
			ServoList[servos[i].servo].reverse = 1;
		
		ServoList[servos[i].servo].init_pos =  setDuty(servos[i].servo,servos[i].init_pos);

	}
	/* init servos del 0 al 15 -> controlados por PCA9685_0 */

		PCA9685_begin(PCA9685_ADDR);
		PCA9685_setPWMFreq(PCA9685_FREQ);

}

void servoController_initialPosition() {
	uint8_t n_servo;
	for (n_servo = 0; n_servo < SERVO_TOTAL_NUMBER; n_servo++) {
		if (ServoList[n_servo].attached) {
			ServoList[n_servo].angle = ServoList[n_servo].init_pos;
			ServoList[n_servo].refresh = 0;
			PCA9685_setPWM(ServoList[n_servo].addr, 0, ServoList[n_servo].angle);
		}
	}
}

void servoController_moveInstantServo(uint8_t n_servo, int8_t angle) {
	if(ServoList[n_servo].attached){
		int16_t duty = setDuty(n_servo,angle);
		ServoList[n_servo].angle = duty;
		PCA9685_setPWM(ServoList[n_servo].addr, 0, duty);

	}
}

void servoController_setServo(uint8_t n_servo, int8_t angle,uint8_t step) {
	int16_t angle_duty;
	if (ServoList[n_servo].attached) {
		angle_duty = setDuty(n_servo,angle);
		if(angle_duty != ServoList[n_servo].angle){
			ServoList[n_servo].angle_end = angle_duty;
			ServoList[n_servo].refresh = 1;
			ServoList[n_servo].step = step;
			validateStep(n_servo);
			if(ServoList[n_servo].angle < angle_duty)
				ServoList[n_servo].dir=1;
			else
				ServoList[n_servo].dir=0;
		}
	}
}

void servoController_stop() {
	uint8_t i;

	for(i=0; i<SERVO_TOTAL_NUMBER;i++){
		ServoList[i].refresh=0;
	}
}

void servoController_start() {
	uint8_t i;

	for(i=0; i<SERVO_TOTAL_NUMBER;i++){
		if(ServoList[i].attached && ServoList[i].angle != ServoList[i].angle_end)
			ServoList[i].refresh=1;
	}
}

bool_t servoController_refreshAll() {
	uint8_t n_servo;
	bool_t notEnd = 0;
	for (n_servo = 0; n_servo < SERVO_TOTAL_NUMBER; n_servo++) {
		if (ServoList[n_servo].attached && ServoList[n_servo].refresh) {
			servoController_moveServo(n_servo);
			if (ServoList[n_servo].refresh)
				notEnd  = 1;				//Si el servo no termino asigna 1 a notEnd
		}
	}
	return !notEnd; //Devuelve 1 si terminaron todos
}

bool_t servoController_ifEnd(){

	bool_t end = 1;
	uint8_t i;

	for(i=0; i<SERVO_TOTAL_NUMBER;i++){
		if(ServoList[i].attached && ServoList[i].refresh)
			end = 0;
		}

	return end;
}

uint8_t servoController_getServoPos(uint8_t n_servo){
	uint16_t max;
	uint16_t min;
	int16_t angle;

	max = LimitsList[ServoList[n_servo].type].duty_max;
	min = LimitsList[ServoList[n_servo].type].duty_min;

	uint16_t duty_angle = ServoList[n_servo].angle;

	if(!ServoList[n_servo].reverse)
		angle = map(duty_angle, min, max,0, 100);
	else
		angle = map(duty_angle, min, max,100,0);

	return angle;
}

uint16_t servoController_getServoDuty(uint8_t n_servo){

	return ServoList[n_servo].angle;
}

ServoType servoController_getType(uint8_t n_servo){

	return ServoList[n_servo].type;

}

/*==================[end of file]============================================*/
