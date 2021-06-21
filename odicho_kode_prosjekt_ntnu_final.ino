// daniel odicho
#include <gp20u7.h>
#include <MKRNB.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "SparkFunBME280.h"
BME280 mySensor;
const int oneWireBus = 5;    
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
    
const char PINNUMBER[] = "8722";
const char PHONENUMBER[] = "97312627";

NB nbAccess;
NB_SMS sms;
GP20U7 gps = GP20U7(Serial1);
Geolocation currentLocation;


void setup() {
  Serial.begin(9600);  //9600 baud
  gps.begin();   //starter opp dei diverse komponentane
  sensors.begin();
  Wire.begin();
  

    if (mySensor.beginI2C() == false) // Starter kommunikasjonen over I2C
  {
    Serial.println("Sensoren svarte ikke. Kontroller ledningene.");
    while(1); // Stopper loopen om det ikkje er kopla til skikkelig, eller evt andre problem
  }
  
  bool connected = false;

 while (!connected) {
   if (nbAccess.begin(PINNUMBER) == NB_READY) {
     connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
 }

  Serial.println("NB initialized");
    

  
}

void loop() {
  
  sensors.requestTemperatures();
   //Serial.print("Hav Temperatur: ");                  // Temperatur i vatnet, 4 desimal
   //Serial.println(sensors.getTempCByIndex(0),4);   
  
  if(gps.read()){      //porsessen starter når det er oppretta eit signal med gps
    currentLocation = gps.getGeolocation();
    //Serial.println("Your current location is:");  //desse 5 linjene med serial er bare for debugging, og trengs berre når ein testar komponentane med pcen
    //Serial.print("    Latitude: ");
    //Serial.println(currentLocation.latitude,5);
   // Serial.print("    Longitude: ");
    //Serial.println(currentLocation.longitude,5);
    sms.beginSMS(PHONENUMBER);
 
    sms.print("Your current location is:");
    sms.print("                 Latitude: ");
    sms.print(currentLocation.latitude,6);  //breddegrad
    sms.print("                Longitude: ");
    sms.print(currentLocation.longitude,6);     //lengdegrad
    sms.print("                Hav Temperatur:");
    sms.print(String(sensors.getTempCByIndex(0),4));  //temperaturen til havet
    sms.print("                Fuktighet:");
    sms.print(String(mySensor.readFloatHumidity(),0)); //fuktighet
    sms.print("                 Trykk:");
    sms.print(String(mySensor.readFloatPressure(),0)); //trykk
    sms.print("                  Luft Temperatur:");
    sms.print(String(mySensor.readTempC(),4));  //luft temperatur
    sms.endSMS();

    delay(5000);  //forsinkelse på 5000ms per loop.   Om ein vil ha bort visse variabler kan ein skrive to "//" teikn forran den koden ein vil sette i pause
  }
}
