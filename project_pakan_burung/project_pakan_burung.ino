#include <DHT.h>
#include <DHT_U.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// Define Pin Ultra Sonik
#define echoPin D7 // Echo Pin
#define trigPin D6 // Trigger Pin
#include <Servo.h>
// Define Var DHT11
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D5
DHT dht(dht_dpin, DHTTYPE); 

Servo servo1;


long duration, distance;
int jarak;

const char *ssid = "Rindu kamu";  
const char *password = "ciumkadulu";
char *host = "http://apii.mcdeveloper.click/api/";   //your IP/web server address
HTTPClient http;

// Deklarasi Fungsi
void koneksiWifi();
int ultrasonik();
void kirimStok();
void kirimSuhu();

// mqtt
String MQTTBroker = "test.mosquitto.org";
WiFiClient subSclient;
PubSubClient mqtt(subSclient);

void konekMqtt();
void mqttResive(char *topic, byte*msg, unsigned int lengthMessage);

void matikan();
String message = "";
void mqttResive(char *topic, byte*msg, unsigned int lengthMessage){
  
  if(String(topic) == "sentSuhu"){
    for(int i = 0; i< lengthMessage; i++){
      message += String(char(msg[i]));
    }
    Serial.print(message);

  }
}
// Conneksi mqtt
void konekMqtt(){
  while(!mqtt.connected()){
    Serial.println("Connecting MQTT");
    if(mqtt.connect("44684c81-c9b7-48de-ab0d-44ed599a2ccb")){
      mqtt.subscribe("sentSuhu");
    }
  }
}



void setup() {
  // put your setup code here, to run once:
  delay(500);
  servo1.attach(2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  dht.begin();
  // Koneksikan Wifi
  koneksiWifi();

    mqtt.setServer(MQTTBroker.c_str(), 1883);
    mqtt.setCallback(mqttResive);
}

void loop() {   
  if(WiFi.status()== WL_CONNECTED){
    if(!mqtt.connected()){
      konekMqtt();
      
    } 
    if(message == "aktif"){
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
           servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
           servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
           servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
           servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(0);
          Serial.println("Proses Memberi Makan");
          delay(500);
          
          servo1.write(180);
          Serial.println("Proses Memberi Makan");
          matikan();
           message = "";
    }else if(message == "mati"){
        servo1.write(0);
          Serial.println("Proses Memberi Makan");
        message = "";
    }
    kirimStok();
    kirimSuhu();
    message = "";
    mqtt.loop();

  }
  delay(100);
  
}




void matikan(){
  WiFiClient kirimStatus;
  host = "http://apii.mcdeveloper.click/api/button-set-makan";   //your IP/web server address
  
  http.begin(kirimStatus, host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String httpRequestData = "status=mati";

  int httpResponseCode = http.POST(httpRequestData);
  http.end();

}

void kirimSuhu(){

  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  WiFiClient kirimStok;
  
  host = "http://apii.mcdeveloper.click/api/data-suhu";   //your IP/web server address
  
  http.begin(kirimStok, host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String httpRequestData = "temperature=" + String(t) + "&humidity=" + String(h);

  int httpResponseCode = http.POST(httpRequestData);
  http.end();
}

// Mengirimkan Data Stok
void kirimStok(){

  jarak = ultrasonik();
  
  WiFiClient kirimStok;
  
  host = "http://apii.mcdeveloper.click/api/data-stok";   //your IP/web server address
  
  http.begin(kirimStok, host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  float jarakStok = 16;
  float sisa = jarakStok - jarak;
  float persent = sisa / jarakStok * 100;
  String httpRequestData = "stok=" + String(persent);

  int httpResponseCode = http.POST(httpRequestData);
  http.end();
}

// Fungsi ultrasonik
int ultrasonik(){
    // put your main code here, to run repeatedly:
  // if(WiFi.status()== WL_CONNECTED){
    digitalWrite(trigPin, LOW);delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
 
  //perhitungan untuk dijadikan jarak
    distance = duration/58.2;
    //Calculate the distance (in cm) based on the speed of sound.
    // Serial.println(distance);
    return distance;
}

// Koneksi Wifi
void koneksiWifi(){
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

}
