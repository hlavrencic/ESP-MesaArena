#include <Controllers/NextPositionController.h>

NextPositionController::NextPositionController(AsyncWebServer* server,
    SecurityManager* securityManager,
    MotorsController* motorsController,
    MotorsControllerCache* motorsControllerCache) : 
    _postHandler(GO_TO_ENDPOINT_PATH,
                 securityManager->wrapCallback(
                     std::bind(&NextPositionController::goTo, this, std::placeholders::_1, std::placeholders::_2),
                     AuthenticationPredicates::IS_AUTHENTICATED))                                                     
{
  
  _motorsController = motorsController;
  _motorsControllerCache = motorsControllerCache;

  server->on(GO_TO_ENDPOINT_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&NextPositionController::getPos, this, std::placeholders::_1),
                                          AuthenticationPredicates::NONE_REQUIRED));

  _postHandler.setMethod(HTTP_POST);
  _postHandler.setMaxContentLength(256);
  server->addHandler(&_postHandler);
}

void NextPositionController::getPos(AsyncWebServerRequest* request){
  auto viajeActual = _motorsController->getCurrent();
  
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
  NextPositionController::read(viajeActual, root);
  root["mode"] = _motorsController->mode;
  root["queueLength"] = _motorsControllerCache->getQueueLength();

  response->setLength();
  request->send(response);
}

void NextPositionController::goTo(AsyncWebServerRequest* request, JsonVariant& json){
  Dimensions dimensions;
  dimensions.x = json['x'];
  dimensions.y = json['y'];

  auto viajeActual = _motorsControllerCache->goTo(dimensions);
  Serial.println(viajeActual.xVelocidad);
  
  AsyncWebServerResponse* response = request->beginResponse(200);
  request->send(response);
}