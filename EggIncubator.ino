#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN A0 // pin where we connect the dht sensor
#define DHTTYPE DHT11 // type of sensor
#define RELAYPIN 3 // pin where we connect the relay module

LiquidCrystal lcd(13, 12, 7, 6, 5, 4); // initiate lcd screen constructor
DHT dht(DHTPIN, DHTTYPE); // inititate dht11 sensor constructor

void setup() {
  pinMode(RELAYPIN, OUTPUT);
  Serial.begin(9600);
  delay(500);
  Serial.println("Humidity and Temperature Sensor");
  delay(1000);
  display_at_start();
}

void display_at_start(){
  lcd.begin(16,2);
  dht.begin();
  lcd.print("Egg Incubator");
  delay(500);
  lcd.clear();
  lcd.print("Temp:  Humidity:");
}


void loop() {
  delay(500);
  lcd.setCursor(0, 1);
  float humdity = dht.readHumidity();
  float celcius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if(isnan(humdity) || isnan(celcius)){
    lcd.print("ERROR");
    return;
  }

  lcd.print(celcius);
  lcd.setCursor(7,1);
  lcd.print(humdity);

  Serial.print("Humidity = ");
  Serial.print(humdity);
  Serial.print("% ");
  Serial.print("temperature = ");
  Serial.print(celcius);
  Serial.println("C ");

  if(isnan (humdity) || isnan(celcius))
  {
    humdity = 0;
    celcius = 0;
    Serial.println("ERROR");
    return;
  }
  delay(1000);

  manageTemp(celcius);

}

void manageTemp(float temp){
  if (temp < 100) {
    digitalWrite(RELAYPIN, HIGH);
  } else if (temp > 101) {
    digitalWrite(RELAYPIN, LOW);
  }   
}
