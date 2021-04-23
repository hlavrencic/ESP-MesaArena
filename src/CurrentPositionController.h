#ifndef CurrentPositionController_h
#define CurrentPositionController_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <MotorsController.h>

#define LED_PIN 2
#define PRINT_DELAY 5000

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#ifdef ESP32
#define LED_ON 0x1
#define LED_OFF 0x0
#elif defined(ESP8266)
#define LED_ON 0x0
#define LED_OFF 0x1
#endif

#define GET_POS_ENDPOINT_PATH "/rest/getPos"
#define GO_TO_ENDPOINT_PATH "/rest/goTo"


class CurrentPositionController : public StatefulService<Dimensions> {
 public:
  CurrentPositionController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController);

 private:
  MotorsController* _motorsController;

  void getPos(AsyncWebServerRequest* request);
};

class NextPositionController : public StatefulService<Dimensions> {
 public:
  NextPositionController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController);

 private:
  HttpEndpoint<Dimensions> _httpEndpoint;
  MotorsController* _motorsController;
  
  static void read(Dimensions& model, JsonObject& root){
      root["x"] = model.x;
      root["y"] = model.y;
  }

  static StateUpdateResult update(JsonObject& root, Dimensions& model){
      model.x = root["x"];
      model.y = root["y"];

      return StateUpdateResult::CHANGED;
  }

  void goTo(const String& originId);
};

#endif
