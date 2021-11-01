
#ifndef ControllersLoader_H
    #define ControllersLoader_H

    #include <Controllers/CurrentPositionController.h>
    #include <Controllers/NextPositionController.h>
    #include <Controllers/MotorsConfigController.h>

    class ControllersLoader {
        public:
            ControllersLoader(AsyncWebServer* server, ESP8266React* esp8266React, MotorsController* motorsController, MotorsControllerCache* motorsControllerCache) :
            _currentPositionController(server, esp8266React->getSecurityManager(), motorsController, motorsControllerCache),
            _nextPositionController(server, esp8266React->getSecurityManager(), motorsController, motorsControllerCache),
            _motorsConfigController(server, esp8266React->getSecurityManager(), motorsController) {

            }

            void begin(){

            }
            void loop(){

            }
        private:
            CurrentPositionController _currentPositionController;
            NextPositionController _nextPositionController;
            MotorsConfigController _motorsConfigController;
    };

#endif