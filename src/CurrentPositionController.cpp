#include <CurrentPositionController.h>

CurrentPositionController::CurrentPositionController(
  AsyncWebServer* server,
  SecurityManager* securityManager,
  MotorsController* motorsController,
  MotorsControllerCache* motorsControllerCache) :
  _postHandler(CURRENT_POSITION_ENDPOINT_PATH,
                 securityManager->wrapCallback(
                     std::bind(&CurrentPositionController::setPos, this, std::placeholders::_1, std::placeholders::_2),
                     AuthenticationPredicates::IS_ADMIN))  
{
  
  _motorsController = motorsController;
  _motorsControllerCache = motorsControllerCache;

  server->on(CURRENT_POSITION_ENDPOINT_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&CurrentPositionController::getPos, this, std::placeholders::_1),
                                          AuthenticationPredicates::NONE_REQUIRED));

  server->on(GET_POS_ENDPOINT_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&CurrentPositionController::getPos, this, std::placeholders::_1),
                                          AuthenticationPredicates::NONE_REQUIRED));

  _postHandler.setMethod(HTTP_POST);
  _postHandler.setMaxContentLength(256);
  server->addHandler(&_postHandler);
}

void CurrentPositionController::getPos(AsyncWebServerRequest* request){
  auto viajeActual = _motorsController->getCurrent();
  
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
  NextPositionController::read(viajeActual, root);
  root["mode"] = _motorsController->mode;
  root["queueLength"] = _motorsControllerCache->getQueueLength();

  response->setLength();
  request->send(response);
}

void CurrentPositionController::setPos(AsyncWebServerRequest* request, JsonVariant& json){
  long x = json["x"];
  long y = json["y"];
  _motorsController->setCurrentPosition(x, y);

  AsyncWebServerResponse* response = request->beginResponse(200);
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