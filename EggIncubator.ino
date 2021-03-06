#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN A0 // pin where we connect the dht sensor
#define DHTTYPE DHT11 // type of sensor
#define RELAYPIN A2 // pin where we connect the relay module
#define FANPIN A1 // pin where we connect the fan

LiquidCrystal lcd(13, 12, 5, 4, 3, 2); // initiate lcd screen constructor
DHT dht(DHTPIN, DHTTYPE); // inititate dht11 sensor constructor

int tempC = 0;
int humdC = 0;

int RelayState =  0;
int lastRelayState = 0;

int FanState =  300;
int lastFanState = 0;

void setup() {
  pinMode(RELAYPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  Serial.begin(9600);
  delay(500);
  Serial.println("Humidity and Temperature Sensor");
  delay(500);
  display_at_start();
}

void display_at_start(){
  lcd.begin(16,2);
  dht.begin();
  lcd.print("Egg Incubator");
  delay(1000);
  lcd.clear();
  lcd.print("Temp:  Humidity:");
}

void display_data(float temp, float humd, int tempC, int humdC){
  lcd.setCursor(0, 0);
  lcd.print("Tp:");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print("H:");
  lcd.print(humd);
  lcd.setCursor(0, 1);
  lcd.print("Tc:");
  lcd.print(tempC);
  lcd.print(" ");
  lcd.setCursor(9, 1);
  lcd.print("Hc:");
  lcd.print(humdC);
}


void loop() {
  delay(500);
  lcd.setCursor(0, 1);
  float humidity = dht.readHumidity();
  float celcius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  RelayState = digitalRead(RELAYPIN);
  delay(50);

  if (RelayState != lastRelayState) {
    if (RelayState == HIGH) {
      tempC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    } else {
      tempC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastRelayState = RelayState;

  FanState = analogRead(FANPIN);
  delay(50);

  if (FanState > 300){
    if (humidity > 60 && FanState > 300) {
      humdC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    } else if (humidity > 60 && FanState < 300) {
      humdC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    }
  }

  else if(FanState < 300){
    if (humidity > 60 && FanState > 300) {
      humdC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    }  else if (humidity < 60 && FanState < 300) {
      humdC++;
      lcd.clear();
      display_data(celcius, humidity, tempC, humdC);
    }
  }

//  if (FanState != lastFanState) {
//    if (RelayState > 0) {
//      humdC++;
//      lcd.clear();
//      display_data(celcius, humidity, tempC, humdC);
//    } else{
//      humdC++;
//      lcd.clear();
//      display_data(celcius, humidity, tempC, humdC);
//    }
//    // Delay a little bit to avoid bouncing
//    delay(50);
//  }
//  lastFanState = FanState;

  Serial.print("RelayState: ");
  Serial.println(RelayState);
  Serial.print("LastRelayState: ");
  Serial.println(lastRelayState);
  Serial.print("FanState: ");
  Serial.println(FanState);
  Serial.print("LastRelayState: ");
  Serial.println(lastFanState);
   

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.print("% ");
  Serial.print("temperature = ");
  Serial.print(celcius);
  Serial.println("C ");
  Serial.print("No. of Changes in Temp = ");
  Serial.println(tempC);
  Serial.print("No. of Changes in Humd = ");
  Serial.println(humdC);
  

  if(isnan (humidity) || isnan(celcius))
  {
    humidity = 0;
    celcius = 0;
    return;
  }
  display_data(celcius, humidity, tempC, humdC);
  delay(300);
  manageTemp(celcius, humidity);
  manageHumd(celcius, humidity);
}

void manageTemp(float temp, float humd){
  if (temp > 37.5) {
    digitalWrite(RELAYPIN, LOW);
    lcd.clear();
    display_data(temp, humd, tempC, humdC);
  } else if (temp < 37.5) {
    digitalWrite(RELAYPIN, HIGH);
    lcd.clear();
    display_data(temp, humd, tempC, humdC);
  }   
}

void manageHumd(float temp, float humd){
  if (humd > 60) {
    digitalWrite(FANPIN, LOW);
    lcd.clear();
    display_data(temp, humd, tempC, humdC);
  } else if (humd < 60) {
    digitalWrite(FANPIN, HIGH);
    lcd.clear();
    display_data(temp, humd, tempC, humdC);
  } 
} 
