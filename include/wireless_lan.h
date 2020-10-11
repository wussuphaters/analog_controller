/*****************************************************************************
 * 
 * Header file defining all functions related to wireless LAN interface.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <WiFi.h>

#define WIFI_SSID "tuveuxdupoulet"
#define WIFI_PASSWORD "Mcwalt3r15bacK"
#define WIFI_RECONNECT_TIMEOUT 120000

unsigned long last_wifi_connect_attempt = 0;

void init_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi network..");
  while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) Serial.println("Connected successfully");
  else  {
    Serial.println("Failed to connect");
    last_wifi_connect_attempt = millis();
  }
}

void reconnect_wifi() {
  if(WiFi.status() != WL_CONNECTED && (millis() > (last_wifi_connect_attempt + WIFI_RECONNECT_TIMEOUT) | last_wifi_connect_attempt > millis())) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnected successfully");
    }
    else  {
      Serial.println("Failed to reconnect");
      last_wifi_connect_attempt = millis();
    }
  }
}
