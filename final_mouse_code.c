
#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const int wprox = 200;
const int dspeed = 30;


const char welcome_line1[] PROGMEM = "Revision";
const char welcome_line2[] PROGMEM = "1";
const char name_line1[] PROGMEM = "Tillman";
const char name_line2[] PROGMEM = "Klales";
// A couple of simple tunes, stored in program space.
//const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
// Refresh the LCD display every tenth of a second.
const int display_interval_ms = 100;
#define MS_ELAPSED_IS(n) (get_ms() % n == 0)
#define TIME_TO_DISPLAY (MS_ELAPSED_IS(display_interval_ms))
void initialize()
{
	// Set PC5 as an input with internal pull-up disabled
	DDRC &= ~(1<< PORTC5); //port 5 is an input
	PORTC &= ~(1<< PORTC5);

	DDRC &= ~(1<< PORTC6); //port 1 is an input
	PORTC &= ~(1<< PORTC6);

	DDRC &= ~(1<< PORTD7); //port 7 is an input
	PORTC &= ~(1<< PORTD7);
	// Play welcome music and display a message
	print_from_program_space(welcome_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(welcome_line2);
	//play_from_program_space(welcome);
	delay_ms(1000);
	clear();
	print_from_program_space(name_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(name_line2);
	delay_ms(1000);
	// Display battery voltage and wait for button press
	while(!button_is_pressed(BUTTON_B))
	{
		clear();
		print_long(read_battery_millivolts());
		print("mV");
		lcd_goto_xy(0,1);
		print("Press B");
		delay_ms(100);
	}

	// Always wait for the button to be released so that 3pi doesn't
	// start moving until your hand is away from it.
	wait_for_button_release(BUTTON_B);
	clear();
	print("Go!");
	// Play music and wait for it to finish before we start driving.
	play_from_program_space(go);
	while(is_playing());
}
void back_up()
{
	if (TIME_TO_DISPLAY)
	{
		clear();
		lcd_goto_xy(0,0);
		print("Backing");
		lcd_goto_xy(0,1);
		print("Up");
	}
	// Back up slightly to the left
	set_motors(-50,-90);
}
void turn_in_place() {
	if (TIME_TO_DISPLAY) {
		clear();
		lcd_goto_xy(0,0);
		print("Front");
		lcd_goto_xy(0,1);
		print("Obstacle");
	}
	// Turn to the right in place
	set_motors(50, -50);
}

void motortest(){
	int i;
	int mospeed;
	mospeed = 0;
	for (i = 9 ,i!=0,i--){
		motortest(mospeed,mospeed);
		mospeed += 10;
		delay(5000);	
	}
}

void turn(int deg,){
	int k;
	int j; 
	
	if (deg = 90){
		k = analog_read(/*unkown pin*/1);
		j = analog_read(/*unkown pin*/1);
		}
		set_motors(0,0);
		return();
	}else if (deg = -90){
		k = analog_read(/*unkown pin*/1)
		while(k<200){
			set_motors(20,-20);
		}
		set_motors(0,0);
		return();
	}else if (deg = 180){
		k = analog_read(/*unkown pin*/1)
		while(k<200){
			set_motors(20,-20);
		}
		set_motors(0,0);
		return();
	}
}

void edgelogic(int r, int l, int f){
	if (f > /*proximity*/ wprox){
		if(r>wprox && l>wprox){
			//turn 180 degrees
		}
		if(r>wprox){
			//turn 90 degrees
		}
		if(l>wprox){
			//turn -90 degrees
		}
	}
	
}
int main()
{
	// set up the 3pi
	initialize();
		
	motortest();
	int r,l,f;
	
	while(1){
		edgelogic(r,l,f);
		delay(5);
		set_motors(dspeed,dspeed);
	}
	/*int last_proximity = 0;
	const int base_speed = 200;
	const int set_point = 100; // what is the set point for?
	// This is the "main loop" - it will run forever.
	while(1)
	{
		// In case it gets stuck: for 1 second every 15 seconds back up
		if (get_ms() % 15000 > 14000) {
			back_up();
			continue;
		}
		// If something is directly in front turn to the right in place
		int front_proximity = analog_read(7);
		if (front_proximity > 200) {
			turn_in_place();
			continue;
		}

		int proximity = analog_read(1); // 0 (far away) - 650 (close)
		int proportional = proximity - set_point;
		int derivative = proximity - last_proximity;
		// Proportional-Derivative Control Signal
		int pd = proportional / 3 + derivative * 20;
		int left_set = base_speed + pd;
		int right_set = base_speed - pd;
		set_motors(left_set, right_set);
		if (TIME_TO_DISPLAY) {
			clear();
			lcd_goto_xy(0,0);
			print_long(proximity);
			lcd_goto_xy(5,0);
			print_long(pd);
			lcd_goto_xy(0,1);
			print_long(left_set);
			lcd_goto_xy(4,1);
			print_long(right_set);
		}
		last_proximity = proximity; // remember last proximity for derivative
	}
	// This part of the code is never reached.
	while(1)
	{
set_motors(0,0);
	}*/
}
