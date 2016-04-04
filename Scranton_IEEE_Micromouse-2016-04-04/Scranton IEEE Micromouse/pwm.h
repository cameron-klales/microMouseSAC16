#ifndef PWM_H_
#define PWM_H_

#include "Arduino.h"

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#ifdef __cplusplus
extern "C"
{
#endif

void pwm_begin(uint8_t i2c_address);
void pwm_reset(uint8_t i2c_address);
void pwm_set_freq(uint8_t i2c_address, float freq);
void pwm_set(uint8_t i2c_address, uint8_t num, uint16_t on, uint16_t off);
uint8_t pwm_read8(uint8_t i2c_address, uint8_t address);
void pwm_write8(uint8_t i2c_address, uint8_t addr, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* PWM_H_ */
