#include "LedControl.h"
#include "binary.h"
#include "IRremote.h"
#include <LiquidCrystal.h>
#include <Servo.h>

const int WIND = 0;
const int RAIN = 1;
const int SUN = 2;
const int CLOUD = 3;

byte DRAWINGS[4][8] = {
  {B00001100,B00010010,B00100001, B00100101, B00011001, B00000010, B11111100,B00000000},
  {B00011100, B01100010, B10000001, B01111110, B00000000, B01010100, B00000000, B00101010},
  {B10010001, B01000010, B00011000, B00111101, B10111100, B00011000, B01000010, B10001001},
  {B00000000, B00001100, B00010010, B01100001, B10010001, B10000001, B01111110, B00000000}
};

const String WeatherTypeMatrix[10][4] {
  {"Sunny", "Cloudy", "Precipitation", "Windy"},
  {"Windy", "Precipitation", "Precipitation", "Cloudy"},
  {"Windy", "Precipitation", "Precipitation", "Precipitation"},
  {"Cloudy", "Cloudy", "Windy", "Precipitation"},
  {"Precipitation", "Sunny", "Windy", "Precipitation"},
  {"Cloudy", "Sunny", "Windy", "Sunny"},
  {"Windy", "Cloudy", "Windy", "Precipitation"},
  {"Cloudy", "Cloudy", "Windy", "Cloudy"},
  {"Cloudy", "Precipitation", "Precipitation", "Cloudy"},
  {"Cloudy", "Precipitation", "Precipitation", "Precipitation"}
};

const int TemperatureMatrix[10][4] {
  {2, 22, 10, 11},
  {6, 18, 8, 5},
  {8, 20, 2, 9},
  {4, 21, 4, 8},
  { -1, 23, 6, 10},
  {1, 22, 7, 10},
  {3, 21, 6, 12},
  {0, 21, 5, 10},
  {2, 19, 7, 10},
  {5, 20, 7, 11}
};

const String CitiesArray[4] = {
  "Calgary, AB",
  "Los Angeles, CA",
  "Toronto, ON",
  "Chicago, IL"
};

const String DatesArray[10] = {
  "Mon, March 9",
  "Tues, March 10",
  "Wed, March 11",
  "Thurs, March 12",
  "Fri, March 13",
  "Sat, March 14",
  "Sun, March 15",
  "Mon, March 16",
  "Tues, March 17",
  "Wed, March 18"
};

int currentCity = 0;
int currentDay = 0;

/*
 DIN connects to pin 12
 CLK connects to pin 11
 CS connects to pin 10 
*/
LedControl lc= LedControl(12,11,10,1);

/*
 * D7: 34
 * D6: 35
 * D5: 32
 * D4: 33
 * 
 * E: 51
 * RS: 50
 */
 LiquidCrystal lcd(50, 51, 33, 32, 35, 34);

Servo temperatureDial;

// Delcaration values for IR Receiver
const int RECV_PIN = 53;
IRrecv irrecv(RECV_PIN);
decode_results results;

int currentDrawingIndex = 0;

void setup(){
  Serial.begin(9600);
  // Set up the IR Receiver
  irrecv.enableIRIn();
  irrecv.blink13(true);
  
  // Set up the LED matrix
  lc.shutdown(0,false);
  // Set brightness to a medium value
  lc.setIntensity(0, 4);
  // Clear the display
  lc.clearDisplay(0);

  // Setup the LCD Screen
  lcd.begin(10, 2);
  lcd.setCursor(0, 0);

  temperatureDial.attach(26);
  int servoPosition = GetServoPosition(TemperatureMatrix[currentDay][currentCity]);
  temperatureDial.write(servoPosition);

  // Get the index value for the Weather
  int index = GetWeatherIndexValue(WeatherTypeMatrix[currentDay][currentCity]);
  // Set the current drawing to the first day of the first city
  draw(DRAWINGS[index]);
  // Print the name of the first city to the lcd screen
  lcd.print(CitiesArray[0]);
  lcd.setCursor(0, 1);
  lcd.print(DatesArray[0]);
  delay(5.00);
}

// Draws the given array of draws on the led matrix
void draw(byte* drawing){
  lc.setRow(0,0,drawing[0]);
  lc.setRow(0,1,drawing[1]);
  lc.setRow(0,2,drawing[2]);
  lc.setRow(0,3,drawing[3]);
  lc.setRow(0,4,drawing[4]);
  lc.setRow(0,5,drawing[5]);
  lc.setRow(0,6,drawing[6]);
  lc.setRow(0,7,drawing[7]);
}

int GetWeatherIndexValue(String weatherType) {
  if (weatherType == "Sunny") {
    return SUN;
  } else if (weatherType == "Cloudy") {
    return CLOUD;
  } else if (weatherType == "Precipitation") {
    return RAIN;
  } else {
    return WIND;
  }
}

int GetServoPosition(int temperature) {
  
  int pos = 0;
  if (temperature <= 0) {
    pos = ((temperature * -2.4) + 95);
  } else if (temperature > 0 && temperature <= 10) {
    pos = ((temperature * -2.8) + 95);
  } else if (temperature > 10 && temperature <= 15) {
    pos = ((temperature * -3) + 97);
  } else if (temperature > 15 && temperature <= 20) {
    pos = ((temperature * -2.8) + 94);
  } else if (temperature > 20 && temperature <= 25) {
    pos = ((temperature * -2.2) + 82);
  } else {
    pos = ((temperature * -2.63) + 96);
  }

  return pos;
}

void loop(){
  // If the IR Reciever receives a message
  if (irrecv.decode(&results)){
    // Print the hex value received
    Serial.println(results.value, HEX);
    // If the >>| button was pressed
    if (results.value == 0xFFC23D) {
      // Increment the index of which value is currently shown
      currentCity++;
      currentDay = 0;
      // If the index is greater than the maximum possible value, reset it to 0
      if (currentCity > 3) {
        currentCity = 0;
      }
    }
    // If the |<< button was pressed
    else if (results.value == 0xFF22DD) {
      // Decrement the index of which value is currently shown
      currentCity--;
      currentDay = 0;
      if (currentCity < 0) {
        // If the index is less than the minimum possible value, reset it to the maximum
        currentCity = 3;
      }
    }
    // If the '0' button was pressed
    else if (results.value == 0xFF6897) {
      currentDay = 0;
    }
    else if (results.value == 0xFF30CF) {
      currentDay = 1;
    }
    else if (results.value == 0xFF18E7) {
      currentDay = 2;
    }
    else if (results.value == 0xFF7A85) {
      currentDay = 3;
    }
    else if (results.value == 0xFF10EF) {
      currentDay = 4;
    }
    else if (results.value == 0xFF38C7) {
      currentDay = 5;
    }
    else if (results.value == 0xFF5AA5) {
      currentDay = 6;
    }
    else if (results.value == 0xFF42BD) {
      currentDay = 7;
    }
    else if (results.value == 0xFF4AB5) {
      currentDay = 8;
    }
    else if (results.value == 0xFF52AD) {
      currentDay = 9;
    } else if (results.value == 0xFFA25D) {
      currentDay = 0;
      currentCity = 0;
    }

    int drawing = GetWeatherIndexValue(WeatherTypeMatrix[currentDay][currentCity]);
    
    // Draw the relevant image in the LED Matrix
    draw(DRAWINGS[drawing]);
    // Clear the lcd screen
    lcd.clear();
    lcd.setCursor(0, 0);
    // Print the city name to the lcd screen
    lcd.print(CitiesArray[currentCity]);
    lcd.setCursor(0, 1);
    lcd.print(DatesArray[currentDay]);

    int servoPosition = GetServoPosition(TemperatureMatrix[currentDay][currentCity]);
    temperatureDial.write(servoPosition);

    delay(500);
    irrecv.resume();
  }
}
