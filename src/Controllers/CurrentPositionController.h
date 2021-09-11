#ifndef CurrentPositionController_h
#define CurrentPositionController_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <Services/MotorsControllerCache.h>

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

#define CURRENT_POSITION_ENDPOINT_PATH "/rest/CurrentPosition"

class CurrentPositionController {
 public:
  CurrentPositionController(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController,
                    MotorsControllerCache* motorsControllerCache);


 private:
    
    AsyncCallbackJsonWebHandler _postHandler;
    MotorsController* _motorsController;
    MotorsControllerCache* _motorsControllerCache;

    void getPos(AsyncWebServerRequest* request);

    void setPos(AsyncWebServerRequest* request, JsonVariant& json);

};

#endif
