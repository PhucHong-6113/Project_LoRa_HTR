#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
#define DHTPIN A0     
#define DHTTYPE DHT11  

#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349

DHT dht(DHTPIN, DHTTYPE);
int hum;
float temp; //Stores temperature value
int buzzerPin = 3;
int int_p = A1; 
// notes in the melody:
int melody1[] = {
    NOTE_F4, 0
}; 

int melody2[] = {
    NOTE_F4, NOTE_E4, NOTE_DS4, NOTE_D4, 0
}; 

float noteDurations1[] = 
{
    8, 16
    
};
float noteDurations2[] = 
{
    2, 2, 2, 1, 16
    
};

int size1 = sizeof(melody1) / sizeof(int);
int size2 = sizeof(melody2) / sizeof(int);

void setup() {    
  Serial.begin(9600);
  dht.begin();
  while (!Serial);
  Serial.println("LoRa Sender");  
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  }
void loop() 
{
  
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  if(hum >= 80){
    LoRa.beginPacket();
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");
    Serial.print("Temp:");
    Serial.print(temp);
    Serial.println("C");
    Serial.println(""); 
    LoRa.print("Humidity is over 80%!");
    LoRa.endPacket();
    song2();
    delay(2000);
  }
  else if(hum <= 40){
      LoRa.beginPacket();
      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print("%");
      Serial.print("Temp:");
      Serial.print(temp);
      Serial.println("C");
      Serial.println(""); 
      LoRa.print("Humidity is under 40%@");
      LoRa.endPacket();
      song2();
      delay(2000);
    }
  else{
      song1();
      Serial.println("Sending packet: ");
      
      // send packet
      LoRa.beginPacket();
      LoRa.print("Humidity:");
      LoRa.print(hum);
      LoRa.print("%");
      LoRa.print("Temp:");
      LoRa.print(temp);
      LoRa.print("C");
      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print("%");
      Serial.print("Temp:");
      Serial.print(temp);
      Serial.println("C");
      Serial.println(""); 
      LoRa.endPacket();
      delay(2000);
  }  
}

void song1()
{
  // iterate over the notes of the melody:
  int val = analogRead(int_p);
  for (int thisNote = 0; thisNote < size1; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000.0 / noteDurations1[thisNote];
    tone(buzzerPin, val*3, noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzerPin);
  }
}

void song2()
{
  int val = analogRead(int_p);
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < size2; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000.0 / noteDurations2[thisNote];
    tone(buzzerPin, val*3, noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzerPin);
  }
}
