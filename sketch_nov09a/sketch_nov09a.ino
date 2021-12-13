#include <Servo.h>
#include <Wire.h>
//#include <Stepper.h>
#include <LiquidCrystal_I2C_Hangul.h>
//#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
//#include <Stepper.h>
#include <Keypad.h>

#define SLAVE 1
#define MODEMAIN 0
#define MODEENTER 1
#define MODEADD 2
#define MODECHANGE 3
#define MODEADMIN 4

LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2); 

//전역
int mode = 0;
boolean isUser = false;
boolean isSelected = false;

//사용자 입력 저장
String password2[3] = {"1111","2222","3333"};
String userInput = "";
String userSelect = "";

// 키패드
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//스텝모터
//int steps = 2048;
//Stepper stepper(steps, 13, 12, 11, 10);

//서보 모터
Servo servo;
int value = 0;


void modeMain(){
  userInput = "";
  lcd.clear();
  lcd.print("Main Mode");
  lcd.setCursor(0,1);
  lcd.print("Ent:A Hm:B Chg:C");
}

void enter(char key){
  userInput += key;
  lcd.clear();
  lcd.print("Enter Mode");
  lcd.setCursor(0,1);
  if(userInput.startsWith("A")){
    userInput = userInput.substring(1);
  }
  if(userInput.endsWith("A")||userInput.endsWith("B")||userInput.endsWith("C")){
    userInput = "";
  }
  if(key == 'D'){
    del();
  }
  lcd.print(userInput);
  if(key == '*'){
    userInput = userInput.substring(0,userInput.length() -1);
    for (int i=0; i<sizeof(password2);i++){
      if( userInput == password2[i] ){
        isUser = true;
        //userSelect = String(i+1);
      }
    }
    if(isUser){
      lcd.clear();
      lcd.print("Come on in");
      value = 90;
      servo.write(value);
      
//      stepper.step(steps);
      delay(1000);
      
      value = 0;
      servo.write(value);
      lcd.clear();
      userInput = "";
      userSelect = "";
      isUser = false;
      mode = MODEMAIN;
      modeMain();
    }
    else {
      lcd.clear();
      lcd.print("Incorrect!");
      delay(3000);
      lcd.clear();
      lcd.print("Enter Mode");
      userInput = "";
    }
  }
}
void changePwd(char key){
  userInput += key;
  lcd.clear();
  lcd.print("Change Mode "+userSelect);
  lcd.setCursor(0,1);
  if(userInput.startsWith("C")){
    userInput = userInput.substring(1);
  }
  if(userInput.endsWith("A")||userInput.endsWith("B")){
    userInput = "";
  }
  if(key == 'D'){
    del();
  }
  lcd.print(userInput);
  if(key == '*'){
    lcd.clear();
    userSelect = userInput.substring(0, userInput.length() -1);
    if (userSelect.toInt() < sizeof(password2)+1){
      lcd.print(userSelect+" Selected");
      lcd.setCursor(0,1);
      lcd.print("Input prior pwd");
      isSelected = true;
      userInput = "";
    }
    else{
      lcd.clear();
      lcd.print("Select again");
      delay(1000);
      lcd.clear();
      lcd.print("Change Mode");
      userInput = "";
      userSelect = "";
    }    
  }
  if(key == '#'){    
    if (isSelected){
      userInput = userInput.substring(0,userInput.length() -1);
      if (userInput == password2[userSelect.toInt() -1]){
        isUser = true;
        lcd.clear();
        lcd.print("Certified");
        lcd.setCursor(0,1);
        lcd.print("Input new pwd");
        userInput = "";
      }
      else {
        lcd.clear();
        lcd.print("Incorrect!");
        lcd.setCursor(0,1);
        lcd.print("Try again");
        delay(1000);
        lcd.clear();
        lcd.print("Change Mode "+userSelect);
        userInput = "";
      }
    }
  }

  if (key == 'C'){
    if (isSelected && isUser){
      lcd.clear();
      userInput = userInput.substring(0,userInput.length() -1);
      password2[userSelect.toInt()-1] = userInput;
      lcd.print("Password is now");
      lcd.setCursor(0,1);
      lcd.print("changed!");
      delay(3000);
      lcd.clear();
      isUser = false;
      isSelected = false;
      userInput = "";
      userSelect = "";
      mode = MODEMAIN;
      modeMain();
    }  
  }  
}
void del(){
  userInput = userInput.substring(0,userInput.length() -2);
}

void setup(){
  servo.attach(13);
  Serial.begin(9600);
  lcd.init();
//  stepper.setSpeed(10);
  lcd.noBacklight();
  modeMain();
  Wire.begin();
  
}
  
void loop(){
  char key = keypad.getKey();
  
  Wire.requestFrom(SLAVE, 1);
  int c = Wire.read();
  Serial.println(c);
  
  if(String(c,DEC).toInt()<30){
    lcd.backlight();
  }
  else if(String(c,DEC).toInt()>100){
    lcd.noBacklight();
  }
  Serial.println(String(c,DEC));
    
  if (key){

    if(key == 'A'){
      mode = MODEENTER; 
      delay(200);
    }
    else if(key == 'B'){
      mode = MODEMAIN;
      delay(200);
    }
    else if(key == 'C'){
      mode = MODECHANGE;
      delay(200);
    }
    
    switch(mode){
      case MODEMAIN:
        modeMain();
        break;
      case MODEENTER:
        enter(key);
        break;
      case MODECHANGE:
        changePwd(key);
        break;
    }
  }
}
