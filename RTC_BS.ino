/*
* Name: RTC-projekt
* Author: Márton Lendvai
* Date: 2025-10-20
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231, displaying a mapped value to a 9g-servo-motor, 
* As well as turning on a fan if the temperature excceeds a threshold.
*/

// Include Libraries for Real-Time-Clock:"RTC", oled display and servo. 
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include "Servo.h"



// Init constants
const int servoPin = 9 ; 
const int fanPin = 10;
const int thresh = 27 ; // Temperature threshold for fan activation

// Construct objects: Including servo, digital display as well as an RTC
Servo myservo;
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	

void setup() {
  // init communication with serial monitor as well as servo
  Serial.begin(9600);
  Wire.begin();
  myservo.attach(servoPin);
  pinMode(fanPin, OUTPUT);
  
  // Init RTC
  rtc.begin();

  // Set time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  oledWrite( getTime() ); // Write time on digital display
  servoWrite( getTemp() ); // Give angle to corresponding tempreature degree
  fanWrite( getTemp()); // Initiate fan depending on temperature
  
  Serial.println(getTemp()); // Print temperature in serial monitor
  Serial.println(getTime()); // Print time in serial monitor

  delay(1000);
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text ) {
  u8g.setFont(u8g_font_unifont);
  u8g.firstPage();  
  do {
    u8g.drawStr(0,20, getTime().c_str());
  } while( u8g.nextPage() );
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
myservo.write(getTemp());
  if (getTemp() > 180) {
  myservo.write(180);
  }
  if (getTemp() < 0) {
  myservo.write(0);
  }
}

/*
* starts fan if temperature value goes over threshold
*Paramteres: - value: temperature
*Returns: void
*/
void fanWrite(float value) {

  if (value >= thresh) {
    digitalWrite(fanPin, HIGH);
  }
  else {
    digitalWrite(fanPin, LOW);
  }
}