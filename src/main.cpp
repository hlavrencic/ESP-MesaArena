#include <ESP8266React.h>
#include <Services/MotorsControllerCache.h>
#include <LightStateService.h>
#include <Controllers/ControllersLoader.h>


#define SERIAL_BAUD_RATE 115200


AsyncWebServer server(80);
ESP8266React esp8266React(&server);
MotorsController motorsController;
MotorsControllerCache motorsControllerCache(motorsController);
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        &motorsController,
                                                        &motorsControllerCache);

ControllersLoader controllersLoader(&server, &esp8266React, &motorsController, &motorsControllerCache);                                          

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

  controllersLoader.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
  
  motorsControllerCache.loop();

  lightStateService.loop();

  controllersLoader.loop();
}