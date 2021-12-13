#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>
#include <Wire.h>

#define SLAVE 1
#define TRIG 9
#define ECHO 8
#define RST 2
#define SS 7
#define bt 2
#define led 3

MFRC522 mfrc(SS, RST);

float duration, distance;
boolean prev = false;
boolean turn = false;
  
void setup() {
  Wire.begin(SLAVE);
  Wire.onReceive(fromMaster);
  Wire.onRequest(sendToMaster);
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  SPI.begin();
  mfrc.PCD_Init();
  pinMode(bt, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = ((float)(duration * 340) / 10000) / 2;
//
//  if(digitalRead(bt)){ // 버튼이 눌러진 경우
//   
//    Serial.println("Button is pressed..");
//   
//  }
//  digitalWrite(led, turn);
  
//  Serial.println(int(distance));
  
}
void fromMaster( int bytes){
  for (int i=0; i<bytes; i++){
    char ch = Wire.read();
    Serial.print(ch);
  }
}
void sendToMaster(){
  Wire.write(byte(int(distance)));
//  Serial.println(int(distance));
}
