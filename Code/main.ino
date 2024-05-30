#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define RS_PIN 8
#define EN_PIN 9
#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7
#define BL_PIN 10
#define COLD_MIN 15
#define COLD_MAX 20
#define MODERATE_MIN 21
#define MODERATE_MAX 24
#define WARM_MIN 25
#define WARM_MAX 29

bool showAmbientTemp = true;
int brightnessLevel = 175;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
  pinMode(BL_PIN, OUTPUT);
  lcd.begin(16, 2);
  mlx.begin();
  analogWrite(BL_PIN, brightnessLevel);
}

void loop() {
  
  float ambientTemp = mlx.readAmbientTempC();
  float objectTemp = mlx.readObjectTempC();

  if (analogRead(0) > 600 && analogRead(0) < 800) {
    // Toggle the display mode and delay to avoid multiple detections
    showAmbientTemp = !showAmbientTemp;
    while (analogRead(0) > 600 && analogRead(0) < 800){
      delay(10);
    }
  }
  while (analogRead(0) < 60 && brightnessLevel < 255){
    brightnessLevel += 2;
    analogWrite(BL_PIN, brightnessLevel);
    delay(23); 
  }
  while (analogRead(0) > 400 && analogRead(0) < 600 && brightnessLevel > 10){
    brightnessLevel -= 2;
    analogWrite(BL_PIN, brightnessLevel);
    delay(23);
  }

  if (showAmbientTemp) {
    lcd.setCursor(0, 0);
    lcd.print("Ambient: ");
    lcd.print(ambientTemp);
    lcd.print(" C");

    // Display customizable text based on temperature ranges
    lcd.setCursor(0, 1);
    if (ambientTemp < COLD_MIN) {
      lcd.print("hladnichko :) ");
    } else if (ambientTemp >= COLD_MIN && ambientTemp <= COLD_MAX) {
      lcd.print("hubavo e :)   ");
    } else if (ambientTemp >= MODERATE_MIN && ambientTemp <= MODERATE_MAX) {
      lcd.print("hubavo e :)   ");
    } else if (ambientTemp >= WARM_MIN && ambientTemp <= WARM_MAX) {
      lcd.print("hubavo e :)   ");
      
    } else {
      lcd.print(":(            ");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Object: ");
    lcd.print(objectTemp);
    lcd.print(" C ");

    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the top line
  }

  delay(100);
}
