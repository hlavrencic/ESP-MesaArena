#ifndef LightStateService_h
#define LightStateService_h


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

#define LIGHT_SETTINGS_ENDPOINT_PATH "/rest/lightState"
#define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightState"

class LightState {
 public:
  bool ledOn;
  int16_t brigthness;
  Dimensions* arduinoPosition;
  Dimensions* nextPosition;

  static void read(LightState& settings, JsonObject& root) {
    root["led_on"] = settings.ledOn;
    root["brightness"] = settings.brigthness;
    root["arduinoPosition"]["x"] = settings.arduinoPosition->x;
    root["arduinoPosition"]["y"] = settings.arduinoPosition->y;

    serializeJson(root, Serial); Serial.println(""); 
  }

  static StateUpdateResult update(JsonObject& root, LightState& lightState) {
    Serial.print("update: "); serializeJson(root, Serial);

    boolean newState = root["led_on"] | DEFAULT_LED_STATE;
    int16_t newBright = root["brightness"] | 100;
    
    int32_t newNextPositionX = root["nextPosition"]["x"] | 0;
    int32_t newNextPositionY = root["nextPosition"]["y"] | 0;

    Serial.println("updated.");

    if (lightState.ledOn != newState || 
    lightState.brigthness != newBright || 
    lightState.nextPosition->x != newNextPositionX ||
    lightState.nextPosition->y != newNextPositionY ) {
      lightState.brigthness = newBright;
      lightState.ledOn = newState;
      lightState.nextPosition->x = newNextPositionX;
      lightState.nextPosition->y = newNextPositionY;

      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

  
};

class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    MotorsController* motorsController,
                    MotorsControllerCache* motorsControllerCache);
  void begin();
  void loop();

 private:
  HttpEndpoint<LightState> _httpEndpoint;
  WebSocketTxRx<LightState> _webSocket;
  
  MotorsController* _motorsController;
  MotorsControllerCache* _motorsControllerCache;

  void registerConfig();
  void onConfigUpdated(const String& originId);
  unsigned long lastUpdate;
};

#endif
