#ifndef CurrentPositionController_h
#define CurrentPositionController_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <MotorsControllerCache.h>

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
#define GO_TO_SPEED_ENDPOINT_PATH "/rest/goToSpeed"
#define CONFIG_ENDPOINT_PATH "/rest/config"


class CurrentPositionController : public StatefulService<PositionStatus> {
 public:
  CurrentPositionController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController,
                    MotorsControllerCache* motorsControllerCache);


 private:
    MotorsController* _motorsController;
    MotorsControllerCache* _motorsControllerCache;

    void getPos(AsyncWebServerRequest* request);
};

class NextPositionController : public StatefulService<ViajeActual> {
 public:
  NextPositionController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsControllerCache* motorsController);
                    
    static void read(ViajeActual& model, JsonObject& root){
        root["x"] = model.x;
        root["y"] = model.y;
        root["xActual"] = model.xActual;
        root["yActual"] = model.yActual;
        root["xVelocidad"] = model.xVelocidad;
        root["yVelocidad"] = model.yVelocidad;
        root["delay"] = model.delay;
    }
 private:
  HttpEndpoint<ViajeActual> _httpEndpoint;
  MotorsControllerCache* _motorsController;
  
  static StateUpdateResult update(JsonObject& root, ViajeActual& model){
      model.x = root["x"];
      model.y = root["y"];
      model.delay = root["delay"];

      return StateUpdateResult::CHANGED;
  }

  void goTo(const String& originId);
};

class NextPositionFullController : public StatefulService<DimensionsFull> {
 public:
  NextPositionFullController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController);

 private:
  HttpEndpoint<DimensionsFull> _httpEndpoint;
  MotorsController* _motorsController;
  
  static void read(DimensionsFull& model, JsonObject& root){
      root["guid"] = model.guid;
      root["x"] = model.xPos;
      root["y"] = model.yPos;
      root["xSpeed"] = model.xSpeed;
      root["ySpeed"] = model.ySpeed;
  }

  static StateUpdateResult update(JsonObject& root, DimensionsFull& model){
      model.guid = root["guid"];
      model.xPos = root["x"];
      model.yPos = root["y"];
      model.xSpeed = root["xSpeed"];
      model.ySpeed = root["ySpeed"];
      return StateUpdateResult::CHANGED;
  }

  void goTo(const String& originId);
};

class MotorsConfigController : public StatefulService<MotorsConfig> {
 public:
  MotorsConfigController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController);

 private:
  HttpEndpoint<MotorsConfig> _httpEndpoint;
  MotorsController* _motorsController;
  
  static void read(MotorsConfig& model, JsonObject& root){
      root["xMax"] = model.xMax;
      root["yMax"] = model.yMax;
      root["xMaxSpeed"] = model.xMaxSpeed;
      root["yMaxSpeed"] = model.yMaxSpeed;
  }

  static StateUpdateResult update(JsonObject& root, MotorsConfig& model){
      model.xMax = root["xMax"];
      model.yMax = root["yMax"];
      model.xMaxSpeed = root["xMaxSpeed"];
      model.yMaxSpeed = root["yMaxSpeed"];
      return StateUpdateResult::CHANGED;
  }

  void config(const String& originId);
};

#endif
