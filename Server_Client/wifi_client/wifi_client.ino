
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "Adafruit_LEDBackpack.h"

#ifndef STASSID
#define STASSID "bitr43"
#define STAPSK  "bitbitr43"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.30";
const uint16_t port = 5006;

ESP8266WiFiMulti WiFiMulti;

const uint8_t addr = 0x70; // HT16K33 default address
uint16_t displayBuffer[8];

Adafruit_7segment matrix = Adafruit_7segment();  // Not sure what this does, but apparently it's needed
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  matrix.begin(0x70);  
 
  Wire.beginTransmission(addr);
  Wire.write(0x20 | 1); // turn on oscillator
  Wire.endTransmission();
  
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}
 
void loop() {
  const int dTime = 50;
  int number=0;

  clear();

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  Serial.println("--------------------");
  
  while(1){
    if(client.available()) {
      String line = client.readStringUntil('\r');
      number = line.toInt();
      Serial.println(number);
      matrix_show(number);
    }
  }
}

void matrix_show(int number){
   matrix.writeDigitNum(0, number/1000);
   matrix.writeDigitNum(1, (number/100)%10);
   matrix.writeDigitNum(3, (number/10)%10);
   matrix.writeDigitNum(4, number%10);

   matrix.writeDisplay();  
}
 
void clear(){
  for(int i = 0; i < 8; i++){
    displayBuffer[i] = 0;
  }
}
