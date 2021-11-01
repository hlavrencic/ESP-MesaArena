#ifndef CurrentPositionController_h
    #define CurrentPositionController_h


    #include <HttpEndpoint.h>
    #include <Services/MotorsControllerCache.h>

    #define CURRENT_POSITION_ENDPOINT_PATH "/rest/CurrentPosition"

    class CurrentPositionController {
    public:
    CurrentPositionController(AsyncWebServer* server,
                        SecurityManager* securityManager,
                        MotorsController* motorsController,
                        MotorsControllerCache* motorsControllerCache);


    private:
        
        AsyncCallbackJsonWebHandler _postHandler;
        MotorsController* _motorsController;
        MotorsControllerCache* _motorsControllerCache;

        void getPos(AsyncWebServerRequest* request);

        void setPos(AsyncWebServerRequest* request, JsonVariant& json);

    };

#endif
