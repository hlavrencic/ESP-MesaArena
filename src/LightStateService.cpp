#include <LightStateService.h>
#include <MotorsControllerCache.h>

LightStateService::LightStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController,
                                     MotorsControllerCache* motorsControllerCache) :
    _httpEndpoint(LightState::read,
                  LightState::update,
                  this,
                  server,
                  LIGHT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(LightState::read,
               LightState::update,
               this,
               server,
               LIGHT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED){
  
  _motorsController = motorsController;
  _motorsControllerCache = motorsControllerCache;

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(originId); }, false);

  
}

void LightStateService::begin() {
  _state.ledOn = DEFAULT_LED_STATE;
  _state.arduinoPosition = new Dimensions();
  _state.nextPosition = new Dimensions();
  onConfigUpdated("INIT");
}

void LightStateService::onConfigUpdated(const String& originId) {
  if(originId.compareTo("A MANO") == 0) return; 

  _motorsControllerCache->goTo(*_state.nextPosition);  
}

void LightStateService::loop(){
  if(lastUpdate + 1000 < millis()){
    lastUpdate = millis();
    
    if(_motorsController->mode == MotorsControllerMode::ERROR){
      Serial.println("ERROR");
    } else {
      update([&](LightState& newState){

        auto pos = _motorsController->getCurrent();

        if(newState.arduinoPosition->x != pos.xActual || newState.arduinoPosition->y != pos.yActual){
          newState.arduinoPosition->x = pos.xActual;
          newState.arduinoPosition->y = pos.yActual;
          return StateUpdateResult::CHANGED;
        }

        return StateUpdateResult::UNCHANGED;
        
      }, "A MANO");
    }
  }
}