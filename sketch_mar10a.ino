#include<Servo.h>
const int servopin=9;
const int steppin_x = 6, steppin_y = 5;
const int dirpin_x = 8, dirpin_y = 7;
const int enable = 12;
const int x_interrupt = 2, y_interrupt = 3;
const int interval = 1500;
volatile int pulse_cycle;
volatile int curr_x, curr_y;
volatile int pos = 0;
volatile int a[4];
//const int sleep=6;
Servo myservo;

void rise(bool dir){
  if(dir){
    for(pos = 0; pos < 180; pos += 10)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    }
  }
  else{
    for(pos = 180; pos>=1; pos-=5)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    }
  }
} 

void resetx() {
  if(pos!=0)  rise(0);//deraise
  digitalWrite(dirpin_x, HIGH);
  while (1) {
    if (digitalRead(x_interrupt) == HIGH) break;
    ;
    digitalWrite(steppin_x, HIGH);
    delayMicroseconds(1000);
    digitalWrite(steppin_x, LOW);
    delayMicroseconds(1000);
  }

}

void resety() {
  if(pos!=0)  rise(0);//deraise
  digitalWrite(dirpin_y, LOW);
  while (1) {
    if (digitalRead(y_interrupt) == HIGH) return;
    digitalWrite(steppin_y, HIGH);
    delayMicroseconds(1000);
    digitalWrite(steppin_y, LOW);
    delayMicroseconds(1000);
  }
}

void go(int pulse_cycle, int steppin) {
  int dirpin = (steppin == 6) ? 8 : 7;
  if (pulse_cycle == 0) return ;
  else if (pulse_cycle > 0) {
    digitalWrite(dirpin, LOW);
    for (int i = 0; i < pulse_cycle; i++) {
      digitalWrite(steppin, HIGH);
      delayMicroseconds(interval);
      digitalWrite(steppin, LOW);
      delayMicroseconds(interval);
    }
  }
  else if (pulse_cycle < 0) {
    digitalWrite(dirpin, HIGH);
    for (int i = pulse_cycle; i < 0; i++) {
      digitalWrite(steppin, HIGH);
      delayMicroseconds(interval);
      digitalWrite(steppin, LOW);
      delayMicroseconds(interval);
    }
  }

}

void moveit(int xi, int yi, int xf, int yf) {
  go((xi - curr_x) * 50, steppin_x); 
  curr_x = xi;
  go((yi - curr_y) * 50, steppin_y); 
  curr_y = yi;
  rise(1);  //rise
  go((xf - xi) * 50, steppin_x); 
  curr_x = xf;
  go((yf - yi) * 50, steppin_y); 
  curr_y = yf;
  rise(0);  //deraise;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(steppin_x, OUTPUT);
  pinMode(steppin_y, OUTPUT);
  pinMode(dirpin_x, OUTPUT); 
  pinMode(dirpin_y, OUTPUT);
  pinMode(servopin,OUTPUT);
  pinMode(enable, OUTPUT);//digitalWrite(enable,HIGH);//enable on
  //pinMode(x_interrupt, INPUT);  pinMode(
  //pinMode(Y_home,INPUT);
  //pinMode(sleep,OUTPUT);
  digitalWrite(x_interrupt, LOW);  
  digitalWrite(y_interrupt, LOW);
  attachInterrupt(digitalPinToInterrupt(3), resetx , RISING);
  attachInterrupt(digitalPinToInterrupt(2), resety , RISING);
  Serial.begin(300);
  myservo.attach(servopin);  myservo.write(0);
  //resetx();
  //resety();
}
void loop() {
  while (Serial.available() < 4);
  for (int i = 0; i < 4; i++)a[i] = Serial.read();
  Serial.println("\n");
  moveit(a[0],a[1],a[2],a[3]);
}



