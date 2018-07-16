
/*
  Wiring:

  HM-10 Pinout
  RX to pin 9 (Sofware TX)
  TX to pin 8 (Sofware RX)
  Vcc to 5 V
  Gnd to Gnd

*/
#include <SoftwareSerial.h>
SoftwareSerial BTserial(8,9); 
char c = ' ';
int flag = 0;
int S = 7;
int flag1 = 0;

int state = LOW;
void setup()
{
  pinMode(  S, INPUT);
  pinMode(11, OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(11,HIGH);
  Serial.begin(9600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");
  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");
  delay(100);
  BTserial.println("AT");
  delay(1000);
  BTserial.println("AT+SLEEP");
  delay(1000);
  cli();
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 62499;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  sei();  
}
 ISR(TIMER1_COMPA_vect){
Serial.println("i am alive");
digitalWrite(11, LOW);
delay(100);
digitalWrite(11,HIGH);
delay(500);
  
 }
// Arduino Execution Loop
void loop()
{                         
 
 if(digitalRead(S) == HIGH && flag == 1)
 {Serial.println("connected");
 flag = 0;
 TIMSK1 |= (1 << OCIE1A);
 }
 if(digitalRead(S)== LOW && flag == 0)
{ Serial.println("not connected");
flag = 1;
TIMSK1 &=~ (1 << OCIE1A);
TCNT1  = 0;
delay(1000);
BTserial.println("AT+SLEEP");
}
 
   
  
  if (BTserial.available())
  {
    c = BTserial.read();
    Serial.write(c);
    
    }
                                
  if (Serial.available())
  {
    c = Serial.read();
    BTserial.write(c);
    Serial.write(c);
  }

}
