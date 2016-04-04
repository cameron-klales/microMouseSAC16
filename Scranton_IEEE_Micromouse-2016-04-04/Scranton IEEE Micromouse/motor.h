#ifndef MOTOR_H
#define	MOTOR_H

#include "position.h"
#include "detection.h"
#include "stepper.h"
#include "nav.h"

// Hardik told me to do this
/*#define TURN_ADJ 79
#define TURN_180 160
#define BLOCK 230*/

#define TURN_ADJ 81
#define TURN_180 163
#define BLOCK 215

#ifdef __cplusplus
extern "C"
{
#endif

// Status of the motor
#define MOTOR_STANDBY 0
#define MOTOR_MOVING 1
#define MOTOR_TURNING 2

// Adjustment status
#define MOTOR_NO_ADJ 0
#define MOTOR_EXP_COR 1
#define MOTOR_WALL 2

// Adjustment direction
#define MOTOR_ADJ_LEFT 0
#define MOTOR_ADJ_RIGHT 1

// Shared values
extern volatile int motor_status;
extern volatile int motor_adj_status;
extern volatile int motor_correction;
extern volatile int motor_correction_dir;

void motor_init(stepper_t *left, stepper_t *right);
void motor_turn_left();
void motor_turn_right();
void motor_turn_180();
void motor_move_forward();
void motor_turn_to_direction(pos_t *current, dir_t dir);

#ifdef __cplusplus
}
#endif

#endif	/* MOTOR_H */
