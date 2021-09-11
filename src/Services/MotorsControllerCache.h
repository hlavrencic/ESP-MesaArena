#ifndef MotorsControllerCache_H 
    #define MotorsControllerCache_H

    #include <list>
    #include <Services/Dimensions.h>
    #include <Services/MotorsController.h>
    
    class MotorsControllerCache {
    public:
        MotorsControllerCache(MotorsController& _motorsController);
        ViajeActual goTo(Dimensions newPos);
        void loop();
        float getTotalDelay();
        u_short getQueueLength();
    private:
        u_short queueLength;
        Dimensions ultimaParada;
        MotorsController* motorsController;
        std::list<ViajeEstimado> cache;
        void estimarViaje(ViajeEstimado& estimado);
        
    };

    

#endif