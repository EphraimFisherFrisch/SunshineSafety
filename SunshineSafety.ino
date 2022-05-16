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


  // Storing time in variables to use later
long start_sunscreen = millis();
long start_drink = millis();

long sunscreen_time = 45000;
long drink_time = 30000;

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



void loop() {
  float uv_index = uv.readUV() / 100.0;
  Serial.print("UV Index is ");
  Serial.println(uv_index);
  delay(1);
  if (uv_index > 0.02)
  {
    sunscreen_time = sunscreen_time - 1000; // Changing sunscreen time according to the UV index
  }


  Serial.print("Temperature is ");
  int currentTemp = analogRead(temp);
  Serial.println(currentTemp);
  if (currentTemp > 400) {
    drink_time -= 1000; // Changing drink time according to the temperature
    Serial.print("GSR is ");
    Serial.println(analogRead(gsr));
    if (analogRead(gsr) < 470)
    {
      drink_time = drink_time - 1500; // Changing drink time according to the GSR readings - sweat
    }
  }
  
  Serial.print("Sunscreen time is ");
  Serial.println(sunscreen_time);
  Serial.print("Drink time is ");
  Serial.println(drink_time);

  delay(1000);


// Sunscreen reminder:
  if (millis() - start_sunscreen > sunscreen_time)
  {
    Serial.print("Sunscreen millis = ");
    Serial.print(millis());
    while (digitalRead(button) == LOW)
    {
      lcd.backlight();
      digitalWrite(vibrate, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("It's sunny!");
      lcd.setCursor(0, 1);
      lcd.print("Put on Sunscreen!");
    }
    lcd.noBacklight();
    lcd.clear();
    digitalWrite(vibrate, LOW);
    start_sunscreen = millis();
    sunscreen_time = start_sunscreen + 45000;
    Serial.print("New sunscreen time is ");
    Serial.println(sunscreen_time);
    delay(200);
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
      lcd.setCursor(0, 0);
      lcd.print("It's hot!");
      lcd.setCursor(0, 1);
      lcd.print("Go take a drink!");
    }
    lcd.noBacklight();
    lcd.clear();
    digitalWrite(vibrate, LOW);
    start_drink = millis();
    drink_time = start_drink + 30000;
    delay(200);
  }
}
