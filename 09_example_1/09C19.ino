//09C19 제출용 코드 작성본    
// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)
#define MEDIAN_SAMPLES 30
                          
// global variables
unsigned long last_sampling_time;   // unit: msec
float dist_ema;                     // EMA distance
float dist_median;                  //중위수 필터결과 저장할 변수 추가함.
float sample_buffer[MEDIAN_SAMPLES];//중위수 버퍼배열   
int buffer_index=0;                 //순환 인덱스 추가


  //배열 정리하기   
void sort_array(float arr[],int n){
  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-i-1;j++){
      if(arr[j]>arr[j+1]){
        float temp = arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
}

  //중위수 필터함
float median_filter(float new_sample){
  sample_buffer[buffer_index]=new_sample;
  buffer_index=(buffer_index+1)%MEDIAN_SAMPLES;

  float sorted_buffer[MEDIAN_SAMPLES];
  for(int i=0;i<MEDIAN_SAMPLES;i++){
    sorted_buffer[i]=sample_buffer[i];
  }

  sort_array(sorted_buffer,MEDIAN_SAMPLES);

  return sorted_buffer[MEDIAN_SAMPLES/2];
}


void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // initialize serial port
  Serial.begin(57600);

  //중위수 필터 버퍼 초기화하기 
  for(int i=0;i<MEDIAN_SAMPLES;i++){
    sample_buffer[i]=_DIST_MAX;         
  }
}

void loop() {
  float dist_raw;
  
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  dist_median=median_filter(dist_raw);

  // Modify the below line to implement the EMA equation
  //dist_ema = _EMA_ALPHA* dist_filtered+(1.0 - _EMA_ALPHA)* dist_ema;
  //필터함수 미사용으로인한 EMA주석처리   

  // output the distance to the serial port
  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",median:"); Serial.print(dist_median);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  // do something here
  if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
    digitalWrite(PIN_LED, 1);       // LED OFF
  else
    digitalWrite(PIN_LED, 0);       // LED ON

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
