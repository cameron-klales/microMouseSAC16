#include <Encoder.h>
#include "./Wheel.h"
#include "./RingBuffer.h"
#include "./Button.h"

const int motorRF = 23; // right forward motor pin
const int motorRR = 22; // right reverse
const int motorLR = 21; // left forward
const int motorLF = 20; // left reverse
const int encoderR1 = 19;
const int encoderR2 = 18; // both right encoder pins
const int encoderL1 = 11;
const int encoderL2 = 10;

const unsigned int loopFreq = 100; // Hz
const unsigned int loopPeriodUs = 1000000 / loopFreq;
const float dt = 1.f / loopFreq;
unsigned int lastMicros = 0;

// 3 and 4 are motor pins, 0 and 1 are encoder pins
// Change these to fit actual circuit
Wheel leftwheel(motorLR, motorLF, encoderL1, encoderL2, dt, 0.1f, 1200);
Wheel rightwheel(motorRR, motorRF, encoderR1, encoderR2, dt, 0.1f, 1200);

struct DataPoint
{
    float positionR;
    float velocityR;
    float controlR;
    float positionL;
    float velocityL;
    float controlL;
};

RingBuffer<DataPoint, 2048> dataLog;
Button logButton(0, LOW, true);


void setup()
{
    Serial.begin(115200);
    leftwheel.velocityLoop.setTuning(0.1f, 0.f, 0.1f);
    leftwheel.setVelocity(100.f);
    rightwheel.velocityLoop.setTuning(0.1f, 0.f, 0.1f);
    rightwheel.setVelocity(100.f);
}


void loop()
{
    leftwheel.update();
    rightwheel.update();
    leftwheel.setVelocity(1);
    rightwheel.setVelocity(1);
    
    if (logButton.pressEdge())
        dataLog.clear();
    
    if (logButton.pressed())
    {
        dataLog.push({rightwheel.getPosition(),
                      rightwheel.getVelocity(),
                      rightwheel.velocityControl,
                      leftwheel.getPosition(),
                      leftwheel.getVelocity(),
                      leftwheel.velocityControl});
    }
              
    if (Serial.available())
    {
        char buffer[256];
        
        for (int i = dataLog.size(); i > 0; --i)
        {
            snprintf(buffer, 256, "%e,%e,%e,%e,%e,%e",
                     dataLog[i].positionR,
                     dataLog[i].velocityR,
                     dataLog[i].controlR,
                     dataLog[i].positionL,
                     dataLog[i].velocityL,
                     dataLog[i].controlL);
            Serial.println(buffer);
        }
        
        while (Serial.available())
            Serial.read();
    }
    
    // Limit loop speed to a consistent value to make timing and integration simpler
    while (micros() - lastMicros < loopPeriodUs);
    lastMicros = micros();
}
