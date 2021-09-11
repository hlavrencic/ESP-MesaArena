#include <MotorsConfigController.h>

MotorsConfigController::MotorsConfigController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController) : 
    _httpEndpoint(MotorsConfigController::read,
                  MotorsConfigController::update,
                  this,
                  server,
                  CONFIG_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED)                                     
{
  
  _motorsController = motorsController;

  addUpdateHandler([&](const String& originId) { config(originId); }, false);
}

void MotorsConfigController::config(const String& originId){
  _motorsController->config(_state.xMax, _state.yMax, _state.xMaxSpeed, _state.yMaxSpeed );
}