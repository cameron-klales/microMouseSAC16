#include "detection.h"
#include "motor.h"
#include "Arduino.h"
#include "math.h"

int dectection_reading(int sensor)
{
	// Make sure the sensor is in bounds
	if (sensor < S0 || sensor > 5)
	{
		return -1;
	}

	// If it is the from sensors then just return
	if (sensor >= 0 && sensor < 3)
	{
		return analogRead(sensor);
	}

	// Return the value
	return digitalRead(sensor);
}

void dectection_timer_callback()
{
	if (motor_status != MOTOR_TURNING)
	{
		int s0 = dectection_reading(0);
		int s1 = dectection_reading(1);
		int s2 = dectection_reading(2);

		dectection_update_adj(s0, s1, s2);
	}
}

void dectection_force_update()
{
	int s0 = dectection_reading(0);
	int s1 = dectection_reading(1);
	int s2 = dectection_reading(2);

	dectection_update_adj(s0, s1, s2);
}

void dectection_update_adj(int s0, int s1, int s2)
{
	// Values to see if sensors are usable
	int s0_use = 0, s2_use = 0;

	// We are at the end of the road
	if (s1 >= 250)
	{
		motor_adj_status = MOTOR_WALL;
		return;
	}

	// Approaching a wall don't listen to s0 or s2
	// they are dirty lairs
	if (s1 >= 110)
	{
		motor_adj_status = MOTOR_NO_ADJ;
		return;
	}

	// See if the s0 is usable
	if (s0 >= 85)
	{
		s0_use = 1;
	}

	// See if the s2 is usable
	if (s2 >= 70)
	{
		s2_use = 1;
	}

	// If both sensors are usable
	if (s0_use == 1 && s2_use == 1)
	{
		dectection_centering_adj(s0, s2);
	}
	else if (s0_use == 1)
	{
		dectection_centering_adj(s0, S2_IDEAL);
	}
	else if (s2_use == 1)
	{
		dectection_centering_adj(S0_IDEAL, s2);
	}
	// Running blind
	else
	{
		motor_adj_status = MOTOR_NO_ADJ;
	}
}

void dectection_centering_adj(int s0, int s2)
{
	int diff = s0 - s2 - S_OFFSET;

	if (diff >= -S_TOL && diff <= S_TOL)
	{
		motor_adj_status = MOTOR_NO_ADJ;
	}
	else if (diff > S_TOL || diff < -S_TOL)
	{
		motor_adj_status = MOTOR_EXP_COR;
		motor_correction = (int) (125.1 * pow(fabs(diff), -0.94));

		if (diff > S_TOL)
		{
			motor_correction_dir = MOTOR_ADJ_LEFT;
		}
		else if (diff < -S_TOL)
		{
			motor_correction_dir = MOTOR_ADJ_RIGHT;
		}
	}
}

void detection_update_walls(struct nav_array *array, pos_t *current)
{
	int s3 = dectection_reading(S3);
	int s4 = dectection_reading(S4);

	if (s3 == 0)
	{
		nav_update_wall(array, current, left);
	}

	if (s4 == 0)
	{
		nav_update_wall(array, current, right);
	}
}

void detection_update_front_wall(struct nav_array *array, pos_t *current)
{
	int s1 = dectection_reading(1);

	if (s1 >= 200)
	{
		nav_update_wall(array, current, front);
	}
}
