#include "UbidotsMicroESP8266.h"
#define TOKEN  "BBFF-JvuMffTpfYHMGh8tDQ3ixxA2dMHLWK"  
#define WIFISSID "Elektro"
#define PASSWORD "elektro17"

#include <SoftwareSerial.h> 
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(14,12);
LiquidCrystal_I2C lcd(0x27,16,2);

  float v_UB  =0;
  float i_UB  =0;
  float p_UB  =0;
  float e_UB  =0;

 Ubidots client(TOKEN);
 unsigned long lastMillis = 0;

 void setup(){
    client.wifiConnection(WIFISSID, PASSWORD);

    Serial.begin(115200);
    lcd.begin();
    lcd.clear();
    lcd.noCursor();
 }

 void loop(){

    float v = pzem.voltage();
    if(v != NAN)
    { v_UB = v;
      Serial.print("Voltage: ");
      Serial.print(v);
      Serial.println("V");
    }else {
        Serial.println("Error reading voltage");
    }

    float c = pzem.current();
    if(c != NAN)
    { i_UB = c; 
      Serial.print("Current: ");
      Serial.print(c);
      Serial.println("A");
    }else {
        Serial.println("Error reading current");
    }

    float p = pzem.power();
    if(p != NAN)
    { p_UB = p; 
      Serial.print("Power: ");
      Serial.print(p);
      Serial.println("W");
    }else {
        Serial.println("Error reading power");
    }

    float e = pzem.energy();
    if(e != NAN)
    { e_UB = e; 
      Serial.print("Energy: ");
      Serial.print(e);
      Serial.println("kWh");
    }else {
        Serial.println("Error reading energy");
    }

  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print(v);
  lcd.print("V");
  lcd.print("/");
  lcd.print(c);
  lcd.print("A");
  

  lcd.setCursor(0, 1);
  lcd.print(p);
  lcd.print("W");
  lcd.print("/");
  lcd.print(e);
  lcd.print("kWh");
  
  
  if (millis() - lastMillis > 5000){

    lastMillis = millis();

    client.add("voltage", v_UB);
    client.add("current", i_UB);
    client.add("power", p_UB);
    client.add("energy", e_UB);

    client.sendAll(true);
  }
 }
