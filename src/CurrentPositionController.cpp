#include <CurrentPositionController.h>

CurrentPositionController::CurrentPositionController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController) 
{
  
  _motorsController = motorsController;

  server->on(GET_POS_ENDPOINT_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&CurrentPositionController::getPos, this, std::placeholders::_1),
                                          AuthenticationPredicates::NONE_REQUIRED));
}

void CurrentPositionController::getPos(AsyncWebServerRequest* request){
  Dimensions pos;
  _motorsController->getPos(pos);
  
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
  root["x"] = pos.x;
  root["y"] = pos.y;
  
  response->setLength();
  request->send(response);
}

NextPositionController::NextPositionController(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     MotorsController* motorsController) : 
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
  _motorsController->goTo(_state);
}
