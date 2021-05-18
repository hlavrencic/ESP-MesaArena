#include <MotorsControllerCache.h>

MotorsControllerCache::MotorsControllerCache(MotorsController& _motorsController){
    motorsController = &_motorsController;
};

void MotorsControllerCache::loop(){
    motorsController->loop();
    if(cache.empty()){
        return;
    }

    if(motorsController->mode == MotorsControllerMode::STANDBY){
        auto newPos = cache.front();
        motorsController->goTo(newPos);
        cache.pop_front();
    }
};

void MotorsControllerCache::goTo(Dimensions newPos){
    cache.push_back(newPos);
};