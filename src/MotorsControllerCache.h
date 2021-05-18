#ifndef MotorsControllerCache_H 
    #define MotorsControllerCache_H

    #include <list>
    #include <Dimensions.h>
    #include <MotorsController.h>
    
    class MotorsControllerCache {
    public:
        MotorsControllerCache(MotorsController& _motorsController);
        ViajeActual goTo(Dimensions newPos);
        void loop();
        float getTotalDelay();
    private:
        Dimensions ultimaParada;
        MotorsController* motorsController;
        std::list<ViajeEstimado> cache;
        void estimarViaje(ViajeEstimado& estimado);
        
    };

    

#endif