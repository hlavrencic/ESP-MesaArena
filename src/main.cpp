#include <ESP8266React.h>
#include <MotorsController.h>
#include <MotorsControllerCache.h>
#include <LightStateService.h>
#include <CurrentPositionController.h>

#define SERIAL_BAUD_RATE 115200


AsyncWebServer server(80);
ESP8266React esp8266React(&server);
MotorsController motorsController;
MotorsControllerCache motorsControllerCache(motorsController);
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsControllerCache);

CurrentPositionController currentPositionController = CurrentPositionController(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsController,
                                                        &motorsControllerCache);

NextPositionController nextcontroller = NextPositionController(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsControllerCache);

NextPositionFullController nextPositionFullcontroller = NextPositionFullController(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsController); 

MotorsConfigController motorsConfigController = MotorsConfigController(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsController);                                                  

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // start the server
  server.begin();

  motorsController.begin();
}

unsigned long lastUpdate;

void loop() {
  // run the framework's loop function
  esp8266React.loop();
  
  motorsControllerCache.loop();

  if(lastUpdate + 1000 < millis()){
    lastUpdate = millis();
    
    if(motorsController.mode == MotorsControllerMode::ERROR){
      Serial.println("ERROR");
    } else {
      lightStateService.update([&](LightState& newState){

        Dimensions pos;
        motorsController.getPos(pos);

        if(newState.arduinoPosition->x != pos.x || newState.arduinoPosition->y != pos.y){
          newState.arduinoPosition->x = pos.x;
          newState.arduinoPosition->y = pos.y;
          return StateUpdateResult::CHANGED;
        }

        return StateUpdateResult::UNCHANGED;
        
      }, "A MANO");
    }
  }
}
