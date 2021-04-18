#include <LightStateService.h>

LightStateService::LightStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController) :
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
  _motorsController->goTo(*_state.nextPosition);  
}

void LightStateService::registerConfig() {
  
}
