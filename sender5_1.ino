#include <SPI.h>
#include <LoRa.h>

#include <WiFi.h>                                                
#include <IOXhop_FirebaseESP32.h>                                             

#define FIREBASE_HOST "lora-b315a.firebaseio.com"                         
#define FIREBASE_AUTH "5zRqvJYOolmjKZbCzC5bOZ5fawo1rZjqMMVuAyKs"                    
#define WIFI_SSID "Aman"                                          
#define WIFI_PASSWORD "Aman@123"    

#define ss 5
#define rst 14  
#define dio0 2
String InputString;
String loRaMessage;
String voltage;
String mot;
void setup() {
  Serial.begin(115200);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                    
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) { 
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  while (Serial.available()) {
    InputString = Serial.readString();
    LoRa.beginPacket();
    LoRa.print(InputString);
    LoRa.endPacket();
    Serial.print("Lora Send : ");
    Serial.println(InputString);
  }
   getLoRaData();
}

void getLoRaData() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet ");
    while (LoRa.available()) {
     // Serial.print((char)LoRa.read());
      String LoRaData = LoRa.readString();
      //Serial.print(LoRaData); 
    
    // Get readingID, temperature and soil moisture
    int pos1 = LoRaData.indexOf('/');
    int pos2 = LoRaData.indexOf('&');
    voltage = LoRaData.substring(0, pos1);
    mot = LoRaData.substring(pos1 +1, pos2);
    Serial.print("voltage= ");
    Serial.println(voltage); 
    Serial.print("motor= ");
    Serial.println(mot);
    Firebase.setString("Motor", mot);
     Firebase.setString("Voltage",voltage);
    }
  }
}
