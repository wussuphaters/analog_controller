#include "api.h"

#define outputA 14
#define outputB 27
#define switchPin 26
#define BRIGHTNESS_INCREMENT 20

int currentDeviceIndex = 0;
long unsigned timeA;
bool stateA;
bool lastStateA;

void setup() {
  Serial.begin(115200);
  pinMode(outputA,INPUT);
  pinMode(outputB,INPUT);
  pinMode(switchPin, INPUT);
  
  lastStateA = digitalRead(outputA);
  timeA = millis();
  
  init_wifi();
  get_surrounding_devices();
} 
  
void loop() {
  reconnect_wifi();
  int id = json_msg["devices"][currentDeviceIndex]["id"];
  
  stateA = digitalRead(outputA);
  if( stateA != lastStateA ){ 
    if(abs(millis() - timeA) > 50 ){
      if(digitalRead(outputB) != lastStateA){
        Serial.println("Decreasing brightness");
        increase_brightness(id, -BRIGHTNESS_INCREMENT);
      }
      else{
        Serial.println("Increasing brightness");
        increase_brightness(id, BRIGHTNESS_INCREMENT);
      }
      timeA = millis();
    } 
    lastStateA = stateA ;
  }

  if(!digitalRead(switchPin))  {
    delay(300);
    if(!digitalRead(switchPin))  {
      get_surrounding_devices();
      if(currentDeviceIndex == (json_msg["devices"].size()-1)) currentDeviceIndex = 0;
      else currentDeviceIndex++;
    } else  {
      toggle(id);
    }
    delay(100);
  }
}
