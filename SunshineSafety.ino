// Declaring global variables
int button = 7;
int gsr = A3;
int temp = A6;
int vibrate = 8;

// Including libraries
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include "Adafruit_SI1145.h"
Adafruit_SI1145 uv = Adafruit_SI1145();

// Declaring variables
float sunscreen_time = 45000;
float drink_time = 30000;


void setup() {
  pinMode(button, INPUT);
  pinMode(vibrate, OUTPUT);
  lcd.init();
  lcd.init();
  Serial.begin(9600);
  Serial.println("Adafruit SI1145 test");

  // Setting up the UV sensor
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }
}

// Storing time in variables to use later
float start_sunscreen = millis();
float start_drink = millis();


void loop() {
  float uv_index = uv.readUV() / 100.0;
  Serial.print("UV Index is ");
  Serial.println(uv_index);
  delay(1);
  if (uv_index > 2)
  {
    sunscreen_time = sunscreen_time - 1000; // Changing sunscreen time according to the UV index
  }


  Serial.print("Temperature is ");
  Serial.println(temp);
  if (temp > 400) {
    drink_time -= 1000; // Changing drink time according to the temperature
    if (analogRead(gsr) < 470)
    {
      drink_time = drink_time - 1500; // Changing drink time according to the GSR readings - sweat
    }
  }


  Serial.println(sunscreen_time);
  Serial.println(drink_time);

  delay(1000); 


// Sunscreen reminder:
  if (millis() - start_sunscreen > sunscreen_time)
  {
    Serial.print("Sunscreen millis = ");
    Serial.print(millis());
    while (digitalRead(button) == LOW);
    {
      lcd.backlight();
      digitalWrite(vibrate, HIGH);
      lcd.setCursor(0, 3);
      lcd.print("It's sunny!");
      lcd.setCursor(1, 0);
      lcd.print("Put on Sunscreen!");
    }
    lcd.backlight();
    digitalWrite(vibrate, LOW);
    long start_sunscreen = millis();
    delay(1000);
    long sunscreen_time = start_sunscreen + 45000;
  }

// Drinking reminder
  if (millis() - start_drink > drink_time)
  {
    Serial.print("Drink millis = ");
    Serial.print(millis());
    while (digitalRead(button) == LOW)
    {
      lcd.backlight();
      digitalWrite(vibrate, HIGH);
      lcd.setCursor(0, 4);
      lcd.print("It's hot!");
      lcd.setCursor(1, 0);
      lcd.print("Go take a drink!");
    }
    lcd.backlight();
    digitalWrite(vibrate, LOW);
    long start_sunscreen = millis();
    delay(1000);
    long sunscreen_time = start_sunscreen + 30000;
  }
}
