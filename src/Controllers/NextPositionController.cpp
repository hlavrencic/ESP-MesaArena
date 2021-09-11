#include <Controllers/NextPositionController.h>

NextPositionController::NextPositionController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsControllerCache* motorsController) : 
    _httpEndpoint(NextPositionController::read,
                  NextPositionController::update,
                  this,
                  server,
                  GO_TO_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED)                                     
{
  
  _motorsController = motorsController;

  addUpdateHandler([&](const String& originId) { goTo(originId); }, false);
}

void NextPositionController::goTo(const String& originId){
  auto newState = _motorsController->goTo(_state);
  _state.x = newState.x;
  _state.y = newState.y;
  _state.xActual = newState.xActual;
  _state.yActual = newState.yActual;
  _state.delay = newState.delay;
  _state.delayTotal = newState.delayTotal;
  _state.xVelocidad = newState.xVelocidad;
  _state.yVelocidad = newState.yVelocidad;

  Serial.println(newState.xVelocidad);
  Serial.println(_state.xVelocidad);
}