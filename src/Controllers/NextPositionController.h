#ifndef NextPositionController_H
    #define NextPositionController_H

    #include <HttpEndpoint.h>
    #include <Services/MotorsControllerCache.h>

    #define GO_TO_ENDPOINT_PATH "/rest/goTo"

    class NextPositionController {
    public:
        NextPositionController(AsyncWebServer* server,
                        SecurityManager* securityManager,
                        MotorsController* motorsController,
                        MotorsControllerCache* motorsControllerCache);

        static void read(ViajeActual& model, JsonObject& root){
            root["x"] = model.x;
            root["y"] = model.y;
            root["xActual"] = model.xActual;
            root["yActual"] = model.yActual;
            root["xVelocidad"] = model.xVelocidad;
            root["yVelocidad"] = model.yVelocidad;
            root["delay"] = model.delay;
            root["delayTotal"] = model.delayTotal;
        }
    private:
        AsyncCallbackJsonWebHandler _postHandler;
        MotorsController* _motorsController;
        MotorsControllerCache* _motorsControllerCache;
        
        static StateUpdateResult update(JsonObject& root, ViajeActual& model){
            model.x = root["x"];
            model.y = root["y"];
            model.delay = root["delay"];
            model.xVelocidad = root["xVelocidad"];
            model.yVelocidad = root["yVelocidad"];

            return StateUpdateResult::CHANGED;
        }

        void getPos(AsyncWebServerRequest* request);

        void goTo(AsyncWebServerRequest* request, JsonVariant& json);
    };
#endif

