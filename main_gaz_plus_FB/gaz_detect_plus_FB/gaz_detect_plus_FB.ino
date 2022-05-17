#include <DHT.h>

#define led D3
#define ledg D5
#define buzz D0
#define gasA A0
#define hum D4
#define dhttype DHT11
int gassensorAnalog = 0;


DHT dht(hum, dhttype);

//Wifi stuff
#include "ESP8266WiFi.h"
const char* ssid = "rahma";
const char* password = "012345678";


//FB
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "gaz-detect01-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "DKWhpEvjKQzQCtm5gMUMSSSkGojMAPh2d4tEhEm1"
#define WIFI_SSID "rahma"
#define WIFI_PASSWORD "012345678"
int n = 0;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);  
  pinMode(ledg, OUTPUT); 
  pinMode(buzz, OUTPUT); 


  //Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  //FB
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {
  gassensorAnalog = analogRead(gasA);

  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t\n");
  

 if (gassensorAnalog > 480  ) {
    Serial.println("Gas");
    digitalWrite (buzz, HIGH);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite (buzz, LOW); 
    digitalWrite(led, LOW);
    digitalWrite(ledg, LOW);
    delay(1000);
  }
  else {
    Serial.println("No Gas");
    digitalWrite(ledg, HIGH);
    digitalWrite(led, LOW);
    digitalWrite(buzz, LOW);
  }
  delay(100);

  //Humidity
  Serial.print("Temperature = ");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity = ");
  Serial.println(dht.readHumidity());
  delay(1000);


  //FB
  Firebase.setFloat("gaz",gassensorAnalog);
  Firebase.setFloat("temp",dht.readTemperature());
  Firebase.setFloat("hum",dht.readHumidity());
  
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }

 

}
