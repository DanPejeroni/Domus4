/*
Domus Blynk 1.1 [Arduino Uno - Blynk]
Dan Pejeroni (C) Copyright 2016

Input                                       Morsettiera
A0 -
A1 - 
D3 - 
D2 - BUS Sensori temperatura DS18B20        [8(-)9(+)10(s)]
A2 - 
D9 - 
A3 - 
D8 - 
D10,11,12,13 riservati a shield Ethernet W5100
D4-7 - Relay Board
*/
#define BLYNK_PRINT Serial // Enables Serial Monitor
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>
#define ONE_WIRE_BUS 2                                 // Data wire della sonda collegata al pin 2 digitale
OneWire oneWire(ONE_WIRE_BUS);                         // Setup dell'istanza oneWire per comunicare con dispositivi OneWire
DallasTemperature sensors(&oneWire);                   // Passa il reference di oneWire a Dallas Temperature. 

char auth[] = "dab631f913ec4e85a972d08587fcf290";      //Blynk auth code

SimpleTimer timer;                                  

void setup()
{
  Serial.begin(9600);                                   // See the connection status in Serial Monitor
  Blynk.begin(auth);                                    // Here  Arduino connects to the Blynk Cloud
  sensors.begin();                                      // Inizializzo le sonde DS18b20

  timer.setInterval(1000, myTimerEvents);               // Setup function to be called each 1000 milliseconds
}

bool isFirstConnect = true;                             // Keep this flag not to re-sync on every reconnection

BLYNK_CONNECTED() {                                     // This function will run every time Blynk connection is established
  if (isFirstConnect) {   
    Blynk.syncAll();                                    // Request Blynk server to re-send latest values for all pins
    isFirstConnect = false;
  }
} 

void myTimerEvents()
{
  Blynk.virtualWrite(V9, millis() / 1000);             // Sends Arduino's up time every second to Virtual Pin (V9)

  Blynk.virtualWrite(V4, digitalRead(4)* 255);         // Su V4 scrive stato del relay01
  Blynk.virtualWrite(V5, digitalRead(5)* 255);         // Su V5 scrive stato del relay02
  Blynk.virtualWrite(V6, digitalRead(6)* 255);         // Su V6 scrive stato del relay02
  Blynk.virtualWrite(V7, digitalRead(7)* 255);         // Su V7 scrive stato del relay02  
     
  sensors.requestTemperatures();                       // Send the command to get temperatures
  Blynk.virtualWrite(V0, sensors.getTempCByIndex(0));  // Su V0 scrive la temperatura della prima sonda
  Blynk.virtualWrite(V1, sensors.getTempCByIndex(1));  // Su V1 scrive la temperatura della seconda sonda
//Blynk.virtualWrite(V0, 10.7);                        // Test widget temperatura
//Blynk.virtualWrite(V1, 21.5);                        // Test widget temperatura
  Blynk.virtualWrite(V10, 0);                          // Test Alarm 1
  Blynk.virtualWrite(V11, 0);                          // Test Alatm 2
}

void loop()
{
  Blynk.run(); // All the Blynk Magic happens here...
  timer.run(); // Initiates SimpleTimer
}

