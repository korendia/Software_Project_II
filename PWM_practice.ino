#define LED 7

int period_us = 1000; // 기본 주기 1ms
int duty = 0;         

void set_period(int p){
  if(p < 100) p = 100;
  if(p > 10000) p = 10000;
  period_us = p;
}

void set_duty(int d){
  if(d < 0) d = 0;
  if(d > 100) d = 100;
  duty = d;
}

// pwm 한 주기 출력
void pwm(){
  int ontime = (period_us * duty) / 100;
  int offtime = period_us - ontime;

  if(ontime > 0){
    digitalWrite(LED, LOW);   // on
    delayMicroseconds(ontime);
  }
  if(offtime > 0){
    digitalWrite(LED, HIGH);  // off
    delayMicroseconds(offtime);
  }
}

void triangle_1s(){
  unsigned long start = micros();
  while(micros() - start < 1000000UL){
    float t = (micros() - start) / 1000000.0;
    int d;
    
    if(t < 0.5){
      d = (int)(t * 200);
    }
    else{
      d = (int)((1.0 - t) * 200);
    }
    
    set_duty(d);
    pwm();
  }
}

void setup(){
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); 
}

void loop(){
  set_period(100); // 10000(10ms),1000(1ms),100(0.1ms)
  triangle_1s();
  while(1);
}
