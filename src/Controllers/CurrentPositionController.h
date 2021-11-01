#ifndef CurrentPositionController_h
#define CurrentPositionController_h

    #include <HttpEndpoint.h>
    #include <Services/MotorsController.h>

    #define CURRENT_POSITION_ENDPOINT_PATH "/rest/CurrentPosition"

    class CurrentPositionController : public StatefulService<Dimensions>{
    public:
        CurrentPositionController(
            AsyncWebServer* server,
            SecurityManager* securityManager,
            MotorsController* motorsController);

        static void read(Dimensions& model, JsonObject& root){
            root["x"] = model.x;
            root["y"] = model.y;
        }

        static StateUpdateResult update(JsonObject& root, Dimensions& model){
            model.x = root["x"];
            model.y = root["y"];
            return StateUpdateResult::CHANGED;
        }

    private:
        HttpEndpoint<Dimensions> _httpEndpoint;
        MotorsController* _motorsController;

        void setPos(const String& originId);
    };

#endif
