#include <Controllers/CurrentPositionController.h>

CurrentPositionController::CurrentPositionController(
  AsyncWebServer* server,
  SecurityManager* securityManager,
  MotorsController* motorsController) :
    _httpEndpoint(
      CurrentPositionController::read,
      CurrentPositionController::update,
      this,
      server,
      CURRENT_POSITION_ENDPOINT_PATH,
      securityManager,
      AuthenticationPredicates::IS_ADMIN)                     
{
  
  _motorsController = motorsController;

  addUpdateHandler([&](const String& originId) { setPos(originId); }, false);
}

void CurrentPositionController::setPos(const String& originId){
  
  _motorsController->setCurrentPosition(_state.x, _state.y);
  
}