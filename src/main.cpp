/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Arduino.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

// I2C Addresses
// RTC: 0x68
// LCD: 0x27

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int light_status = 0;
int light_dim_value = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

struct Button {
    const uint8_t PIN;
    bool pressed;
};


// Button Pins
Button button_set_alarm = {36,false};
Button button_set_time = {39,false};
Button button_hour = {34,false};
Button button_min = {35,false};
Button button_alarm_state = {32,false};


// LED pins
const int led1 = 23;
const int led2 = 19;

unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

int periode = 2000;

int currentMillis;
int previousMillis = 0;
int interval = 250;

// pin for RTC interrupt
const int rtc_int = 23;

struct Alarm {
  bool active = false;
  bool trigger = false;
  DateTime alarm_time;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;

};

DateTime startup_time;
DateTime now;
Alarm alarm1;
Alarm clock1;


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());

  for(;;){
    // Serial.println("TASK1");
    // Get the current time from the RTC
    now = rtc.now();  
    
    // Getting each time field in individual variables
    // And adding a leading zero when needed;
    String yearStr = String(now.year(), DEC);
    String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
    String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
    String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC); 
    String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
    String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
    String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];

    // Complete time string
    String formattedTime = dayOfWeek + ", " + yearStr + "-" + monthStr + "-" + dayStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;

    // Print the complete formatted time
    Serial.println(formattedTime);
    
    currentMillis = millis();

    // Activate the alarm
    if (alarm1.active & !alarm1.trigger){
      if (now.minute() == alarm1.min){
        Serial.print("ALARM!");
        if (light_dim_value < 240){
          if (currentMillis - previousMillis >= interval){
            previousMillis = currentMillis;
            light_dim_value = 128+127*cos(2*PI/periode*millis());
            analogWrite(led1, light_dim_value);
            
          }
        }
        alarm1.trigger = false;
      }

      if (digitalRead(button_hour.PIN)){
        light_dim_value = 0;
        analogWrite(led1, light_dim_value);
      }

    }

    // Check if it's time to trigger the alarm


    // Set the time on the clock
    if(button_set_time.pressed){
      Serial.println("Set Clock Time");

      if(!digitalRead(button_set_time.PIN)){
        button_set_time.pressed = false;
      }

      if(digitalRead(button_hour.PIN)){
        clock1.hour += 1;
        if (clock1.hour >=24){
          clock1.hour = 0;
        }
        Serial.println("CLOCK HOUR: "+String(clock1.hour));
      }

      if(digitalRead(button_min.PIN)){
        clock1.min += 1;
        if (clock1.min >=60){
          clock1.min = 0;
        }
        Serial.println("CLOCK MIN: "+String(clock1.min));
      }

      delay(10);
      // January 21, 2014 at 3am you would call:
      rtc.adjust(DateTime(2025, 1, 1, clock1.hour, clock1.min, 0));
    }

    // Set the time on the alarm
    if(button_set_alarm.pressed){
      Serial.println("Set Alarm Time");

      if(!digitalRead(button_set_alarm.PIN)){
        button_set_alarm.pressed = false;
      }

      if(digitalRead(button_hour.PIN)){
        alarm1.hour += 1;
        if (alarm1.hour >=24){
          alarm1.hour = 0;
        }
        Serial.println("ALARM HOUR: "+String(alarm1.hour));
      }

      if(digitalRead(button_min.PIN)){
        alarm1.min += 1;
        if (alarm1.min >=60){
          alarm1.min = 0;
        }
        Serial.println("ALARM MIN: "+String(alarm1.min));
      }


      delay(10);
    }
   
    delay(100);
  } 
}

//Task2code: 
// LCD Display control
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  String alarmTime;
  String alarmHourStr;
  String alarmMinuteStr;

  String clockTime;

  String yearStr;
  String monthStr;
  String dayStr;
  String hourStr;
  String minuteStr;
  String secondStr;
  String dayOfWeek;

  for(;;){
    Serial.println("TASK2");
  
    // Getting each time field in individual variables
    // And adding a leading zero when needed;
    yearStr = String(now.year(), DEC);
    monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
    dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
    hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC); 
    minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
    secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
    dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];


    alarmHourStr = (alarm1.hour < 10 ? "0" : "") + String(alarm1.hour); 
    alarmMinuteStr = (alarm1.min < 10 ? "0" : "") + String(alarm1.min);

    alarmTime = alarmHourStr + ":" + alarmMinuteStr;
    clockTime = hourStr + ":" + minuteStr + ":" + secondStr;


    if(button_set_alarm.pressed){
      // clear the screen
      lcd.clear();
      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      // print message
      lcd.print(alarmTime);
    } else if(button_set_time.pressed){
      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      // print message
      lcd.print(clockTime);
    } else {

      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      // print message
      lcd.print(clockTime);
      // set cursor to bottom corner
      lcd.setCursor(15,1);
      lcd.print(String(alarm1.active));

    }

    delay(250);

  }
}


void IRAM_ATTR isr_set_clock_time() {
  button_time = millis();
  if (button_time - last_button_time > 250){
    if(digitalRead(button_set_time.PIN)){      
      Serial.println("INTERRUPT SET CLOCK TIME");
      button_set_time.pressed = true;
      last_button_time = button_time;
    }
  } 
}

void IRAM_ATTR isr_set_alarm_time() {
  button_time = millis();
  if (button_time - last_button_time > 250){
    if(digitalRead(button_set_alarm.PIN)){
      Serial.println("INTERRUPT SET ALARM TIME");
      button_set_alarm.pressed = true;
      last_button_time = button_time;
    }
  } 
}

void IRAM_ATTR isr_alarm_state() {
  button_time = millis();
  if (button_time - last_button_time > 250){
    if(digitalRead(button_alarm_state.PIN)){
      alarm1.active = !alarm1.active;
      Serial.println("ALARM MODE: "+String(alarm1.active));
      last_button_time = button_time;
    }
  } 
}

void setup() {
  Serial.begin(115200); 
  delay(500);
  Serial.println("hello");
  
  // pinMode(led1, OUTPUT);
  pinMode(rtc_int, INPUT_PULLUP);

  // declare button inputs
  pinMode(button_set_alarm.PIN, INPUT);
  pinMode(button_set_time.PIN, INPUT);
  pinMode(button_hour.PIN, INPUT);
  pinMode(button_min.PIN, INPUT);
  pinMode(button_alarm_state.PIN, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  attachInterrupt(digitalPinToInterrupt(button_set_alarm.PIN), isr_set_alarm_time, RISING);
  attachInterrupt(digitalPinToInterrupt(button_set_time.PIN), isr_set_clock_time, RISING);
  attachInterrupt(digitalPinToInterrupt(button_alarm_state.PIN), isr_alarm_state, RISING);




  Serial.println("let's set the time!");
  // When time needs to be set on a new device, or after a power loss, the
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
  startup_time = rtc.now();

  clock1.hour = startup_time.hour();
  clock1.min = startup_time.minute();

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}



void loop() {
  
}

