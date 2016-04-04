#include "shield.h"
#include "pwm.h"

void shield_init(shield_t *shield, uint8_t address)
{
	shield->address = address;
}

void shield_begin(shield_t *shield, uint16_t freq)
{
	pwm_begin(shield->address);
	shield->freq = freq;

	pwm_set_freq(shield->address, freq);

	uint8_t i = 0;

	for (i = 0; i < 16; i++)
	{
		pwm_set(shield->address, i, 0, 0);
	}
}

void shield_set_pwm(shield_t *shield, uint8_t pin, uint16_t value)
{
	if (value > 4095)
	{
		pwm_set(shield->address, pin, 4096, 0);
	}
	else
	{
		pwm_set(shield->address, pin, 0, value);
	}
}

void shield_set_pin(shield_t *shield, uint8_t pin, uint8_t value)
{
	if (value == LOW)
	{
		pwm_set(shield->address, pin, 0, 0);
	}
	else
	{
		pwm_set(shield->address, pin, 4096, 0);
	}
}
