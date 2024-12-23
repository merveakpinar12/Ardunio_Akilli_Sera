#include "config.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D5    
#define DHTTYPE DHT11   // DHT 11
#define fan_relay D1
DHT dht(DHTPIN, DHTTYPE);

const int toprak_nem = A0;
const int su_motor_relay = D7;
const int tNem = 650;
const int oSicakliği = 27;
const int oNemi = 50;
int toprakNemi;

AdafruitIO_Feed *ToprakNem = io.feed("ToprakNem");
AdafruitIO_Feed *Sicaklik = io.feed("Sicaklik");
AdafruitIO_Feed *Nem = io.feed("Nem");

void setup() {
  Serial.begin(115200);
  pinMode(fan_relay, OUTPUT);
  pinMode(toprak_nem,INPUT);
  pinMode(su_motor_relay,OUTPUT);
  dht.begin();

  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  io.run();

  float nem = dht.readHumidity();
  float sicaklik = dht.readTemperature();

  toprakNemi= analogRead(toprak_nem);
  Serial.print("Toprak Nemi : ");
  Serial.print(toprakNemi);
  ToprakNem->save(toprakNemi);

  Serial.print("      Ortam Sicakligi (Celcius): ");
  Serial.print(sicaklik);
  Sicaklik->save(sicaklik);

  Serial.print("      Ortam Nemi (%): ");
  Serial.println(nem);
  Nem->save(nem);

  if(nem > oNemi){
    if(sicaklik > oSicakliği){
      Serial.println("Ortam Sicakligi ve Nemi yuksek!!!");
      digitalWrite(fan_relay,LOW);
      delay(3000);
    }
    else{
      Serial.println("Nemi yuksek!!!");
      digitalWrite(fan_relay,HIGH);
      delay(3000);
    }
  }
  else{
    if(sicaklik > oSicakliği){
      Serial.println("Ortam Sicakligi yuksek!!!");
      digitalWrite(fan_relay,LOW);
      delay(3000);
    }
    else{
      Serial.println("Ortam Sicakligi ve Nemi ideal.");
      digitalWrite(fan_relay,HIGH);
      delay(3000);
    }
  }
  
  if(toprakNemi > tNem){
    Serial.println("Toprak nemi dusuk!!!");
    digitalWrite(su_motor_relay, LOW);
    Serial.println("Motor calisti.");
    Serial.println("");
    delay(3000);
  }
  else{
    Serial.println("Toprak nemi yuksek!");
    digitalWrite(su_motor_relay, HIGH);
    Serial.println("Motor kapatildi.");
    Serial.println("");
    delay(3000);
  } 
}