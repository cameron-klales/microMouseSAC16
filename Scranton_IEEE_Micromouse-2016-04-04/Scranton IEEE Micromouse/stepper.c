#include "stepper.h"

void stepper_init(stepper_t *stepper, shield_t *shield, uint8_t num)
{
	stepper->num = num;
	stepper->current_step = 0;
	stepper->rev_steps = 0;

	stepper->shield = shield;

	if (stepper->num == 0)
	{
		stepper->pwma_pin = 8;
		stepper->ain2_pin = 9;
		stepper->ain1_pin = 10;
		stepper->pwmb_pin = 13;
		stepper->bin2_pin = 12;
		stepper->bin1_pin = 11;
	}
	else
	{
		stepper->pwma_pin = 2;
		stepper->ain2_pin = 3;
		stepper->ain1_pin = 4;
		stepper->pwmb_pin = 7;
		stepper->bin2_pin = 6;
		stepper->bin1_pin = 5;
	}
}

void stepper_release(stepper_t *stepper)
{
	shield_set_pin(stepper->shield, stepper->ain1_pin, 0x0);
	shield_set_pin(stepper->shield, stepper->ain2_pin, 0x0);
	shield_set_pin(stepper->shield, stepper->bin1_pin, 0x0);
	shield_set_pin(stepper->shield, stepper->bin2_pin, 0x0);
	shield_set_pwm(stepper->shield, stepper->pwma_pin, 0);
	shield_set_pwm(stepper->shield, stepper->pwmb_pin, 0);
}

uint8_t stepper_step(stepper_t *stepper, uint8_t dir)
{
	uint8_t ocrb, ocra;

	ocra = ocrb = 255;

	if ((stepper->current_step / (MICROSTEPS / 2)) % 2)
	{ // we're at an odd step, weird
		if (dir == FORWARD)
		{
			stepper->current_step += MICROSTEPS / 2;
		}
		else
		{
			stepper->current_step -= MICROSTEPS / 2;
		}
	}
	else
	{
		if (dir == FORWARD)
		{
			stepper->current_step += MICROSTEPS;
		}
		else
		{
			stepper->current_step -= MICROSTEPS;
		}
	}

	stepper->current_step += MICROSTEPS * 4;
	stepper->current_step %= MICROSTEPS * 4;

	shield_set_pwm(stepper->shield, stepper->pwma_pin, ocra * 16);
	shield_set_pwm(stepper->shield, stepper->pwmb_pin, ocrb * 16);

	// release all
	uint8_t latch_state = 0; // all motor pins to 0

	switch (stepper->current_step / (MICROSTEPS / 2))
	{
	case 0:
		latch_state |= 0x1; // energize coil 1 only
		break;
	case 1:
		latch_state |= 0x3; // energize coil 1+2
		break;
	case 2:
		latch_state |= 0x2; // energize coil 2 only
		break;
	case 3:
		latch_state |= 0x6; // energize coil 2+3
		break;
	case 4:
		latch_state |= 0x4; // energize coil 3 only
		break;
	case 5:
		latch_state |= 0xC; // energize coil 3+4
		break;
	case 6:
		latch_state |= 0x8; // energize coil 4 only
		break;
	case 7:
		latch_state |= 0x9; // energize coil 1+4
		break;
	}

	if (latch_state & 0x1)
	{
		shield_set_pin(stepper->shield, stepper->ain2_pin, 0x1);
	}
	else
	{
		shield_set_pin(stepper->shield, stepper->ain2_pin, 0x0);
	}
	if (latch_state & 0x2)
	{
		shield_set_pin(stepper->shield, stepper->bin1_pin, 0x1);
	}
	else
	{
		shield_set_pin(stepper->shield, stepper->bin1_pin, 0x0);
	}
	if (latch_state & 0x4)
	{
		shield_set_pin(stepper->shield, stepper->ain1_pin, 0x1);
	}
	else
	{
		shield_set_pin(stepper->shield, stepper->ain1_pin, 0x0);
	}
	if (latch_state & 0x8)
	{
		shield_set_pin(stepper->shield, stepper->bin2_pin, 0x1);
	}
	else
	{
		shield_set_pin(stepper->shield, stepper->bin2_pin, 0x0);
	}

	return stepper->current_step;
}
