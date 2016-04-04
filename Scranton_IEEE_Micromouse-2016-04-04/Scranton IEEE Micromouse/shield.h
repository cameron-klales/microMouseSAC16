/*
 * shield.h
 *
 *  Created on: Feb 23, 2014
 *      Author: stefan
 */

#ifndef SHIELD_H_
#define SHIELD_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include <inttypes.h>

typedef struct
{
	uint8_t address;
	uint16_t freq;
} shield_t;

void shield_init(shield_t *shield, uint8_t address);
void shield_begin(shield_t *shield, uint16_t freq);
void shield_set_pwm(shield_t *shield, uint8_t pin, uint16_t val);
void shield_set_pin(shield_t *shield, uint8_t pin, uint8_t val);


#ifdef __cplusplus
}
#endif

#endif /* SHIELD_H_ */
