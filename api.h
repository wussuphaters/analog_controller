/*****************************************************************************
 * 
 * Header file defining all functions related to the projects' web API.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

 #include <HTTPClient.h>
 #include <ArduinoJson.h>
 #include "wireless_lan.h"

 #define API_ADDR "http://192.168.1.100/smart_home_api/api/"

 HTTPClient api;
 String password = "3bf6g%pXV8lAh6$K@5Vw";
 String jwt = "";
 StaticJsonDocument<512> json_msg;

 String login()  {
  if(WiFi.status() == WL_CONNECTED) {
    api.begin(String(API_ADDR)+"device/login.php");
    api.addHeader("Content-Type", "application/json");
    api.addHeader("charset", "utf-8");
    int responseCode = api.POST("{\"ip\":\""+WiFi.localIP().toString()+"\",\"password\":\""+password+"\"}");
    if(responseCode < 0)  {
      Serial.println("Request error, trying again");
      return login();
    } else if(responseCode == 200) {
      Serial.println("API validated token");
      StaticJsonDocument<512> json_msg;
      
      auto error = deserializeJson(json_msg, api.getString());
      if(error) {
        Serial.println("JSON parsing error");
      } else  {
        Serial.println("Successfully parsed JSON message");
        if(json_msg.containsKey("token")) {
          return json_msg["token"];
        } else  {
          Serial.println("Error retrieving JWT");
        }
      }
    } else  {
      Serial.println("Login failed");
      return "";
    }
  } else  {
    Serial.println("WiFi disconnected");
    reconnect_wifi();
    return login();
  }
 }
 
 bool get_surrounding_devices() {
  if(WiFi.status() == WL_CONNECTED) {
    if(jwt != "") {
      api.begin(String(API_ADDR)+"device/get_surrounding.php");
      api.addHeader("Content-Type", "application/json;charset=utf-8");
      
      int responseCode = api.POST("{\"token\":\""+jwt+"\"}");
      if(responseCode < 0)  {
        Serial.println("Request error, trying again");
        return get_surrounding_devices();
      } else if(responseCode == 200) {
        Serial.println("API returned OK");
        
        auto error = deserializeJson(json_msg, api.getString());
        if(error) {
          Serial.println("JSON parsing error");
          Serial.println(error.c_str());
        } else  {
          Serial.println("Successfully parsed JSON message");
          if(json_msg.containsKey("devices")) {
            Serial.println("Successfully retrieved device list");
            return true;
          } else  {
            Serial.println("Error retrieving device list");
          }
        }
      } else  {
        Serial.println("Could not retrieve device list");
        return false;
      }
    } else  {
      jwt = login();
      return get_surrounding_devices();
    }
  } else  {
    reconnect_wifi();
    return get_surrounding_devices();
  }
 }

 bool toggle(int id)  {
  if(WiFi.status() == WL_CONNECTED) {
    if(jwt != "") {
      api.begin(String(API_ADDR)+"device/control.php");
      api.addHeader("Content-Type", "application/json;charset=utf-8");
      
      int responseCode = api.POST("{\"token\":\""+jwt+"\", \"devices\":[{\"id\":"+id+",\"toggle\":\"\"}]}");
      
      if(responseCode < 0)  {
        Serial.println("Request error, trying again");
        return toggle(id);
      } else if(responseCode == 200) {
        Serial.println("Successfully toggled device");
        return true;
      } else  {
        Serial.println("Could not toggle device");
        return false;
      }
    } else  {
      jwt = login();
      return toggle(id);
    }
  } else  {
    reconnect_wifi();
    return toggle(id);
  }
 }

 bool increase_brightness(int id, int brightness) {
  if(WiFi.status() == WL_CONNECTED) {
    if(jwt != "") {
      api.begin(String(API_ADDR)+"device/control.php");
      api.addHeader("Content-Type", "application/json;charset=utf-8");
      
      int responseCode = api.POST("{\"token\":\""+jwt+"\", \"devices\":[{\"id\":"+id+",\"increase_brightness\":\""+brightness+"\"}]}");
      
      if(responseCode < 0)  {
        Serial.println("Request error, trying again");
        return increase_brightness(id, brightness);
      } else if(responseCode == 200) {
        Serial.println("Successfully changed device brightness");
        return true;
      } else  {
        Serial.println("Could not toggle device brightness");
        return false;
      }
    } else  {
      jwt = login();
      return toggle(id);
    }
  } else  {
    reconnect_wifi();
    return increase_brightness(id, brightness);
  }
 }

 
