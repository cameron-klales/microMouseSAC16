#include "pwm.h"
#include "Wire.h"

extern "C"
{

void pwm_begin(uint8_t i2c_address)
{
	Wire.begin();

	pwm_reset(i2c_address);
}

void pwm_reset(uint8_t i2c_address)
{
	pwm_write8(i2c_address, PCA9685_MODE1, 0x0);
}

void pwm_set_freq(uint8_t i2c_address, float freq)
{
	  float prescaleval = 25000000;
	  prescaleval /= 4096;
	  prescaleval /= freq;
	  prescaleval -= 1;
	  uint8_t prescale = floor(prescaleval + 0.5);

	  uint8_t oldmode = pwm_read8(i2c_address, PCA9685_MODE1);
	  uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	  pwm_write8(i2c_address, PCA9685_MODE1, newmode); // go to sleep
	  pwm_write8(i2c_address, PCA9685_PRESCALE, prescale); // set the prescaler
	  pwm_write8(i2c_address, PCA9685_MODE1, oldmode);
	  delay(5);
	  pwm_write8(i2c_address, PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
	                                          // This is why the beginTransmission below was not working.
	  //  Serial.print("Mode now 0x"); Serial.println(read8(PCA9685_MODE1), HEX);
}

void pwm_set(uint8_t i2c_address, uint8_t num, uint16_t on, uint16_t off)
{
	Wire.beginTransmission(i2c_address);

	Wire.write(LED0_ON_L + 4*num);
	Wire.write(on);
	Wire.write(on >> 8);
	Wire.write(off);
	Wire.write(off >> 8);

	Wire.endTransmission();
}

uint8_t pwm_read8(uint8_t i2c_address, uint8_t address)
{
	Wire.beginTransmission(i2c_address);

	Wire.write(address);

	Wire.endTransmission();

	Wire.requestFrom(i2c_address, (uint8_t) 1);

	return Wire.read();
}

void pwm_write8(uint8_t i2c_address, uint8_t addr, uint8_t data)
{
	Wire.beginTransmission(i2c_address);

	Wire.write(addr);
	Wire.write(data);

	Wire.endTransmission();
}

}
