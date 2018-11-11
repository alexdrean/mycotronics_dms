#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <FirebaseArduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define RELAY 5

#define WIFI_SSID "robocar"
#define WIFI_PASSWORD NULL

#define FIREBASE_HOST "hackathon-4d0b7.firebaseio.com"
#define FIREBASE_AUTH "RCOERBKp0G5T8i0qfKp08vRSJEiouIAtE10rBPjD"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
long lastTempUpdate = -86400000;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, 1);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting");
  DS18B20.begin();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(1);
  }
  Serial.println("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

float getTemperature() {
    DS18B20.requestTemperatures();
    return DS18B20.getTempCByIndex(0);
}

DynamicJsonBuffer jsonBuffer;

void loop() {
  float temperature = getTemperature();
  Serial.print("temp=");
  Serial.println(temperature);
  long interval = Firebase.getInt("settings/secondaryInterval");
  Serial.print("interval=");
  Serial.println(interval);
  float cutoff = Firebase.getFloat("settings/cutoff");
  Serial.print("cutoff=");
  Serial.println(cutoff);
  digitalWrite(RELAY, temperature >= cutoff);
  long mil = millis();
  if (mil - lastTempUpdate >  interval * 1000L) {
    lastTempUpdate = mil;
    Serial.println("Pushing...");
    JsonObject& timeStampObject = jsonBuffer.createObject();
    timeStampObject[".sv"] = "timestamp";
    JsonObject& tempData = jsonBuffer.createObject();
    tempData["temperature"] = temperature;
    tempData["timestamp"] = timeStampObject;
    Firebase.push("secondary", tempData);
    Serial.print("pushed=");
    Serial.println(Firebase.success());
  }
}
