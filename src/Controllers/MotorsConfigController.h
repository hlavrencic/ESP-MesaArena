#ifndef MotorsConfigController_H
    #define MotorsConfigController_H

    #include <HttpEndpoint.h>
    #include <Services/MotorsController.h>

    #define CONFIG_ENDPOINT_PATH "/rest/config"
    
    struct MotorsConfig
    {
        uint xMax;
        uint yMax;
        float xMaxSpeed;
        float yMaxSpeed;
    };

    class MotorsConfigController : public StatefulService<MotorsConfig> {
    public:
    MotorsConfigController(AsyncWebServer* server,
                        SecurityManager* securityManager,
                        MotorsController* motorsController);

    private:
    HttpEndpoint<MotorsConfig> _httpEndpoint;
    MotorsController* _motorsController;
    
    static void read(MotorsConfig& model, JsonObject& root){
        root["xMax"] = model.xMax;
        root["yMax"] = model.yMax;
        root["xMaxSpeed"] = model.xMaxSpeed;
        root["yMaxSpeed"] = model.yMaxSpeed;
    }

    static StateUpdateResult update(JsonObject& root, MotorsConfig& model){
        model.xMax = root["xMax"];
        model.yMax = root["yMax"];
        model.xMaxSpeed = root["xMaxSpeed"];
        model.yMaxSpeed = root["yMaxSpeed"];
        return StateUpdateResult::CHANGED;
    }

    void config(const String& originId);
    };

#endif