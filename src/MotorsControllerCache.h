#ifndef MotorsControllerCache_H 
    #define MotorsControllerCache_H

    #include <list>
    #include <Dimensions.h>
    #include <MotorsController.h>
    
    class MotorsControllerCache {
    public:
        MotorsControllerCache(MotorsController& _motorsController);
        void goTo(Dimensions newPos);
        void loop();
    private:
        MotorsController* motorsController;
        std::list<Dimensions> cache;
    };

    

#endif