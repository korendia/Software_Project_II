#include <Servo.h>
#define SERVO_PIN 10

Servo myServo;

#define _SERVO_SPEED 3.0

float timeDelay_ms = 0;

void slowMove(float speed, int startAngle, int endAngle) {
    
    timeDelay_ms = (1.0 / speed) * 1000.0;
    
    int step = (endAngle > startAngle) ? 1 : -1;

    for (int i = startAngle; i != endAngle; i += step) {
        myServo.write(i);
        delay(timeDelay_ms);
    }
    myServo.write(endAngle);
}


void setup() {
    Serial.begin(9600);
    myServo.attach(SERVO_PIN);
    myServo.write(0);
    delay(1000);
}

void loop() {
    
    // 도전과제 1에 사용 
    slowMove(_SERVO_SPEED, 0, 180);
    
    // 도전과제 2에 사용 
    //slowMove(_SERVO_SPEED, 0, 90);


    while(1);
}
