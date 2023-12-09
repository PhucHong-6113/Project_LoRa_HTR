#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 8);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)  
      ;
  }
  lcd.begin(16, 2);
}

void loop()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // received a packet
    Serial.println("");
    Serial.println("...................................");
    Serial.println("Received packet: ");

    // read packet and print to Serial
    while (LoRa.available())
    {
      String incoming = LoRa.readString();
      int stringLength = incoming.length(); 
      if (incoming == 'c')
      {
        Serial.print("Error");
      }
      else if(incoming[stringLength - 1] == '!'){
        int l=0;
        lcd.clear();
        for(int i = 0; i < 16; i++){
          lcd.setCursor(i, 0);
          lcd.print(incoming[i]);
        }
        for(int i = 16; i < incoming.length()-2; i++){
          lcd.setCursor(l,1);
          lcd.print(incoming[i+1]);
          l++;
        }
      }
      else if(incoming[stringLength - 1] == '@'){
        int l = 0;
        lcd.clear();
        for(int i = 0; i < 12; i++){
          lcd.setCursor(i, 0);
          lcd.print(incoming[i]);
        }
        for(int i = 12; i < incoming.length()-2; i++){
          lcd.setCursor(l,1);
          lcd.print(incoming[i]);
          l++;
        }
      }
      else
      {
        lcd.clear();
        Serial.print(incoming);
        int j=0;
        // print to LCD
         for(int i=0; i < 12; i++){
          
          lcd.setCursor(i,0);
          lcd.print(incoming[i]);
          
          
        }
        
        
        for(int i=12; i < 23; i++){
          lcd.setCursor(j,1);
          lcd.print(incoming[i]);
          j++;
        }
       
      }
    }
  }
}
