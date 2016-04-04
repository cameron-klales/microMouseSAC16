#include "nav.h"
#include "queue.h"
#include "position.h"
#include "motor.h"
#include "timer.h"
#include "stepper.h"
#include "detection.h"
#include "inttypes.h"

volatile unsigned long start_time = 0;

// Motor shield and stepper motors
shield_t shield;
stepper_t motor0, motor1;

// Our empty maze
struct nav_cell cells[8 * 8];
struct nav_array array;

// Current position
pos_t current;
pos_t target;

uint8_t calibrated = 0;

void print_pos(pos_t *position) {
	Serial.print("(");
	Serial.print(position->row);
	Serial.print(", ");
	Serial.print(position->column);
	Serial.print(")");
	Serial.println("");
}

uint8_t calibrate(struct nav_array *nav, pos_t *pos) {
	uint8_t wall = 0;
	struct nav_cell *cell = nav_get_cell_pos(nav, pos);

	// See if we can calibrate
	if (pos->direction == north && nav_north_wall(cell)) {
		wall = 1;
	} else if (pos->direction == east && nav_east_wall(cell)) {
		wall = 1;
	} else if (pos->direction == south && nav_south_wall(cell)) {
		wall = 1;
	} else if (pos->direction == west && nav_west_wall(cell)) {
		wall = 1;
	}
	
	if (wall == 1){
		int reading = dectection_reading(1);
		
		while (reading > 205 || reading < 195)
		{
			if (reading > 205)
			{
				stepper_step(&motor0, BACKWARD);
				stepper_step(&motor1, BACKWARD);
			}
			else
			{
				stepper_step(&motor0, FORWARD);
				stepper_step(&motor1, FORWARD);
			}
			
			delay(5);
			reading = dectection_reading(1);
		}
		
		return 1;
	}
	
	return 0;
}

void print_maze(struct nav_array *array) {
	int i, j;
	for (i = 0; i < array->rows; i++) {
		for (j = 0; j < array->columns; j++) {
			struct nav_cell *cell = nav_get_cell(array, i, j);

			Serial.print("(");
			Serial.print(cell->row);
			Serial.print(",");
			Serial.print(cell->column);
			Serial.print(") ");
		}

		Serial.println("");
	}
}

void print_flood(struct nav_array *array) {
	int i, j;
	for (i = 0; i < array->rows; i++) {
		for (j = 0; j < array->columns; j++) {
			struct nav_cell *cell = nav_get_cell(array, i, j);

			if (cell->flood_num < 10) {
				Serial.print("0");
			}

			Serial.print(cell->flood_num);
			Serial.print(" ");
		}

		Serial.println("");
	}
}

void callback(void) {
	dectection_timer_callback();

	if (motor_adj_status == MOTOR_EXP_COR) {
		Serial.println("");
		Serial.print(motor_correction);
		if (motor_correction_dir == MOTOR_ADJ_LEFT) {
			Serial.println(" left");
		} else if (motor_correction_dir == MOTOR_ADJ_RIGHT) {
			Serial.println(" right");
		}

		Serial.println("");
	}
}

// Micromouse.ino
void setup() {
	delay(5000);

	Serial.println("setup() called");

	// Triggers for relays
	pinMode(S3, INPUT);
	pinMode(S4, INPUT);

	// Start a serial with 115200 baud rate
	Serial.begin(115200);

	// Motor shield is at i2c address of 0x60
	shield_init(&shield, 0x60);

	// Begin the pwm at a frequency of 1600
	shield_begin(&shield, 1600);

	// Init our steppers
	stepper_init(&motor0, &shield, 0);
	stepper_init(&motor1, &shield, 1);

	// Give the stepper to our motor functions
	motor_init(&motor0, &motor1);

	// Setup our position
	current.row = 0;
	current.column = 0;
	current.direction = east;

	target.row = 7;
	target.column = 7;

	// Start a callback to the sensors
	timer2_init_ms(150, dectection_timer_callback);
	start_time = millis();

	Serial.println("setup() complete");
}

void loop() {
	Serial.println("loop() called");

	// Init our maze
	nav_init(&array, cells, 8, 8);

	Serial.print("Current is ");
	print_pos(&current);
	Serial.print("Target is ");
	print_pos(&target);

	// Init shared motor values
	motor_status = MOTOR_STANDBY;
	motor_adj_status = MOTOR_NO_ADJ;

	// See if there are any walls in front
	detection_update_walls(&array, &current);
	detection_update_front_wall(&array, &current);

	// Right walls
	motor_turn_to_direction(&current,
			position_right_adj_direction(current.direction));
	current.direction = position_right_adj_direction(current.direction);
	detection_update_walls(&array, &current);
	detection_update_front_wall(&array, &current);

	// Left walls
	motor_turn_to_direction(&current,
			position_invert_direction(current.direction));
	current.direction = position_invert_direction(current.direction);
	detection_update_walls(&array, &current);
	detection_update_front_wall(&array, &current);

	// Explore the maze
	nav_explore(&array, &current);

	// Turn 180
	motor_turn_180();
	current.direction = position_invert_direction(current.direction);

	// Force an update
	dectection_force_update();

	// Run flood algo
	nav_flood(&array, &target);
	print_flood(&array);
	delay(100);

	// Drive to target
	nav_drive_to_target(&array, &current, &target);

	//Do a dance
	motor_turn_180();
	motor_turn_180();
	motor_turn_180();
	current.direction = position_invert_direction(current.direction);

	delay(10000);

	Serial.println("loop() complete");
	
}
