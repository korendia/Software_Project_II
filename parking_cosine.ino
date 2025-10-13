#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10
#define PIN_TRIG 12
#define PIN_ECHO 13

#define ANGLE_DOWN 0
#define ANGLE_UP 90
#define DISTANCE_THRESHOLD 25

Servo myServo;
unsigned long MOVING_TIME = 1500;
unsigned long moveStartTime = 0;

int startAngle = ANGLE_DOWN;
int stopAngle  = ANGLE_DOWN;

float USS_measure();
float cosine_ease(float t);
void setTargetAngle(int target);

void setup() {
  myServo.attach(PIN_SERVO);
  
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  myServo.write(ANGLE_DOWN);
  delay(500);
}

void loop() {
  float distance = USS_measure();
  
  int desiredAngle;
  if (distance > 0.0 && distance <= DISTANCE_THRESHOLD) {
    desiredAngle = ANGLE_UP;
  } else {
    desiredAngle = ANGLE_DOWN;
  }

  setTargetAngle(desiredAngle);

  unsigned long progress = millis() - moveStartTime;

  if (progress < MOVING_TIME) {
    float t = (float)progress / MOVING_TIME; 
    
    float ease_val = cosine_ease(t);

    long angle = startAngle + (stopAngle - startAngle) * ease_val;
    
    myServo.write(angle);
  } else {
    myServo.write(stopAngle);
  }
  
  delay(50);
}

void setTargetAngle(int target) {
    if (target != stopAngle) {
        startAngle = myServo.read();
        stopAngle = target;
        moveStartTime = millis();
    }
}

float USS_measure() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);

  float distance = (float)duration / 2.0 / 29.1;

  if (distance > 400.0 || distance < 0.0) return 0.0;
  
  return distance;
}

float cosine_ease(float t) {
  return 0.5 * (1.0 - cos(PI * t));
}
