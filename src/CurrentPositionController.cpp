#include <CurrentPositionController.h>

CurrentPositionController::CurrentPositionController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController,
                                     MotorsControllerCache* motorsControllerCache) 
{
  
  _motorsController = motorsController;
  _motorsControllerCache = motorsControllerCache;

  server->on(GET_POS_ENDPOINT_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&CurrentPositionController::getPos, this, std::placeholders::_1),
                                          AuthenticationPredicates::NONE_REQUIRED));
}

void CurrentPositionController::getPos(AsyncWebServerRequest* request){
  auto viajeActual = _motorsController->getCurrent();
  
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
  NextPositionController::read(viajeActual, root);
  root["mode"] = _motorsController->mode;
  root["totalDelay"] = _motorsControllerCache->getTotalDelay();
  root["queueLength"] = _motorsControllerCache->getQueueLength();

  response->setLength();
  request->send(response);
}

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
  _state = _motorsController->goTo(_state);
}

NextPositionFullController::NextPositionFullController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController) : 
    _httpEndpoint(NextPositionFullController::read,
                  NextPositionFullController::update,
                  this,
                  server,
                  GO_TO_SPEED_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED)                                     
{
  
  _motorsController = motorsController;

  addUpdateHandler([&](const String& originId) { goTo(originId); }, false);
}

void NextPositionFullController::goTo(const String& originId){
  _motorsController->goTo(_state.xPos, _state.yPos, _state.xSpeed, _state.ySpeed );
}

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