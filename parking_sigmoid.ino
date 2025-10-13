//10C16 제출용 코드
//Sigmoid 함수 사용버전.

#include <Servo.h>

#define PIN_SERVO 10
#define PIN_TRIG 12
#define PIN_ECHO 13

#define ANGLE_DOWN 0
#define ANGLE_UP 90
#define DISTANCE_THRESHOLD 25

Servo myServo;
unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;
int startAngle = ANGLE_DOWN; 
int stopAngle  = ANGLE_DOWN; 

long measureDistance();
float sigmoid_ease(float t);
void setTargetAngle(int target);

void setup() {
  Serial.begin(9600);
  myServo.attach(PIN_SERVO);

  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);

  myServo.write(startAngle); // Set position
  delay(500);
}

void loop() {
  long distance = measureDistance();
  int desiredAngle;

  if (distance > 0 && distance <= DISTANCE_THRESHOLD) {
    desiredAngle = ANGLE_UP;//차단기 상승 
  } else {
    desiredAngle = ANGLE_DOWN;//차단기 하강 
  }

  setTargetAngle(desiredAngle);

  unsigned long progress = millis() - moveStartTime;//sigmoid 사용. 

  // 움직임이 '진행 중'인 경우 (Sigmoid 곡선을 따라 각도 계산)
  if (progress < MOVING_TIME) {
    float t = (float)progress / MOVING_TIME; 
    float sigmoid_val = sigmoid_ease(t);
    long angle = startAngle + (stopAngle - startAngle) * sigmoid_val;
    
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
        
        Serial.print("Target changed to: ");
        Serial.println(target);
    }
}

long measureDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);

  long distance = duration / 2 / 29.1;

  if (distance > 400 || distance < 0) return 0;
  
  return distance;
}

float sigmoid_ease(float t) {
  float x = 12.0 * t - 6.0; 
  return 1.0 / (1.0 + exp(-x));
}
