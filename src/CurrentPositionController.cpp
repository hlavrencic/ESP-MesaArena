#include <CurrentPositionController.h>
#include <NextPositionController.h>

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