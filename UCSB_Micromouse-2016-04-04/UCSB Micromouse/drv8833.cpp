#include "drv8833.h"
#include <WProgram.h>
#include <cmath>


Motor::Motor(int pin1, int pin2) : pin1(pin1), pin2(pin2)
{
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    
    // Set up PWM
    analogWriteFrequency(pin1, 20000);
    analogWriteFrequency(pin2, 20000);
    analogWriteResolution(16);
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
}


void Motor::write(float value)
{
    if (value < 0.f)
    {
        analogWrite(pin1, 65535 * fabs(value));
        digitalWrite(pin2, 0);
    }
    else
    {
        digitalWrite(pin1, 0);
        analogWrite(pin2, 65535 * fabs(value));
    }
}


Motor& Motor::operator=(float value)
{
    write(value);
    return *this;
}


void Motor::brake()
{
    digitalWrite(pin1, 1);
    digitalWrite(pin2, 1);
}
