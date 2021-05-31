#include <DHT11.h>

#include <IRremote.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


// Set the LCD address to 0x27 for a 16 chars and 2 line display

const int irReceiverPin = 6;
const int ledPin = 12;

const int motorPin = 8;
Servo servo;

// UP, DOWN, LEFT, RIGHT
char CMD[] = { '8', '2', '4', '6' };
String MSG[] = { "UP", "DOWN", "LEFT", "RIGHT" };
int DIR[] = { 10, 11, 12, 13 };

/**
 * lcd_state
 * 0 = main menu
 * 1 = displaying ON/OFF
 * 2 = setting date
 * 2 = setting alarm
 */
int lcd_state = 0;
int lcd_count = 0; // Used in LCD display duration
bool lcd_hasAlarm = false;


/**
 * lcd variables
 */
uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};

LiquidCrystal_I2C lcd(0x27, 16, 2); //안되면 주소를 0x3F 로 변경

int months[] = { 31, 29, 31, 30,
                       31, 30, 31, 31,
                       30, 31, 30, 31 };
int year = 2021;
int month = 5;
int day = 30;
int hour = 23;
int minute = 30;
int sec = 40;

int cursor_loc = 0;
int cursor_blink_mill = 1;


/**
 * sensor variables;
 */

DHT11 dht(2);
 
float temp = 1000;
int humi = 1000;

IRrecv irrecv(irReceiverPin);
decode_results decodedSignal;


void setup()
{
    Serial.begin(9600); 
    delay(100);
    pinMode(ledPin, OUTPUT);
    servo.attach(motorPin);
    irrecv.enableIRIn();
    setupLCD();
}

boolean on = false;
unsigned long last = millis();

void loop(){

  loop_lcd();
  detectRev();

}

void detectRev(){
  if (irrecv.decode(&decodedSignal) == true) 
  {
    if (millis() - last > 250/*250*/) 
    { 
      int sig = getSignal(decodedSignal.value);      
      if(sig != -1){
        remote(sig);
      }
      Serial.print("value=");
      Serial.println(decodedSignal.value);
      Serial.print("signal=");
      Serial.println(sig);
      
    }
    
    last = millis();
    irrecv.resume();       // watch out for another message
  }
}

void changeState(int state){
  switch(state){
    case 0:
      
    case 2:
      cursor_loc = 0;
      lcd.cursor();
      lcd.blink();
      break;
  }

  if(state != 2){
    lcd.noCursor();
    lcd.noBlink();
  }
}

// delayed 1 sec
void togglePower(boolean b){
  on = b;
  digitalWrite(ledPin, b);
  lcd_state = 1;
  display_toggle(!b);
  
  int i;
  int angle = 90;
  for(i=0;i<90;i++){
    angle = angle + (on ? 1 : -1);
    servo.write(angle);
    delay(5);
  }
  display_toggle(b);
  delay(300);
  servo.write(90);
  delay(250);
  addTime();
  lcd.clear();
  lcd_state = 0;
}
