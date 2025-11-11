#include <Servo.h>

// Arduino pin assignment

#define PIN_IR    A0         // IR sensor at Pin A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 544  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1480  // servo neutral position (90 degree)
#define _DUTY_MAX 2690  // servo full counter-clockwise position (180 degree)

#define _DIST_MIN  100.0   // minimum distance 100mm
#define _DIST_MAX  250.0   // maximum distance 250mm

#define EMA_ALPHA  0.1      // for EMA Filter

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time;   // unit: msec

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9)) - 4.0) * 10.0 -60.0;

  if (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX)
  { 
    digitalWrite(PIN_LED, LOW); // 범위 내이면 LED 켜기
    dist_prev = dist_raw;        // 유효한 거리 값으로 업데이트
  }
  else
  {
    digitalWrite(PIN_LED, HIGH);  // 범위 밖이면 LED 끄기
  }
  dist_ema = (EMA_ALPHA * dist_prev) + ((1.0 - EMA_ALPHA) * dist_ema);
      
  //duty = map(dist_ema, _DIST_MIN, _DIST_MAX, _DUTY_MIN, _DUTY_MAX);
  duty = (int)((long)(dist_ema - _DIST_MIN) * (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN) + _DUTY_MIN);

  if (duty < _DUTY_MIN) duty = _DUTY_MIN;
  if (duty > _DUTY_MAX) duty = _DUTY_MAX;
  
  myservo.writeMicroseconds(duty);

  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
