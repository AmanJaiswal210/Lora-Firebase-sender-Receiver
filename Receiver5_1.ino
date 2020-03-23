#include <SPI.h>
#include <LoRa.h>

#define ss 10
#define rst 9
#define dio0 2

String LoRaMessage = "";
int voltage;
int offset = 0;
char mot;
static String recdata;                      


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("LoRa Receiver here.....");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else Serial.println("all good");
}

void loop() {
    getdata();
      
    if (recdata == "on\n"){
       sendReadings();
      delay(200);
    }
    
    else if (recdata == "off\n")  Serial.println("Motor off");
    
}


void getdata(){
    int packetSize = LoRa.parsePacket();                
    if (packetSize) {
      Serial.print("packetsize : ");
      Serial.println(packetSize);                 
    // read packet
    while (LoRa.available()) {                      
      recdata =  LoRa.readString();
      Serial.print("Received packet  : ");
      Serial.println(recdata);
   
    }
  } 
}

  
void VoltageCheck(){
  int volt = analogRead(35);
  voltage = map(volt, 0, 4095, 0, 2500) + offset;
  voltage /= 100;
   }

void motor(){
  mot = Serial.print("Motor is on");
  }

void sendReadings() {
  LoRaMessage = String(voltage) + "/" + String(mot) + "&";
  LoRa.beginPacket();
  LoRa.println(LoRaMessage);
  LoRa.endPacket();}


