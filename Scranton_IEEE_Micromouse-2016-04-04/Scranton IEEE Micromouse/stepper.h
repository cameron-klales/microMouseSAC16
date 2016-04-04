/*
 * stepper.h
 *
 *  Created on: Feb 23, 2014
 *      Author: stefan
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "shield.h"

#define MICROSTEPS 16

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	shield_t *shield;
	uint8_t num;
	uint8_t pwma_pin, ain1_pin, ain2_pin;
	uint8_t pwmb_pin, bin1_pin, bin2_pin;
	uint16_t rev_steps; // # steps per revolution
	uint8_t current_step;
} stepper_t;

void stepper_init(stepper_t *stepper, shield_t *shield, uint8_t num);
void stepper_release(stepper_t *stepper);
uint8_t stepper_step(stepper_t *stepper, uint8_t dir);

#ifdef __cplusplus
}
#endif


#endif /* STEPPER_H_ */
