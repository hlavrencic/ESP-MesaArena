#include <Services/MotorsControllerCache.h>

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
        queueLength--;
    }
};

u_short MotorsControllerCache::getQueueLength(){
    return queueLength;
}

void MotorsControllerCache::estimarViaje(ViajeEstimado& estimado){

    auto distancia1 = abs(ultimaParada.x - estimado.x);
    auto distancia2 = abs(ultimaParada.y - estimado.y);

    if(distancia1 == 0 && distancia2 == 0) {
        estimado.xVelocidad = 0.0f;
        estimado.yVelocidad = 0.0f;
        return;
    }

    auto maxSpeedX = motorsController->getMaxSpeedX();
    auto maxSpeedY = motorsController->getMaxSpeedY();

    float velocidad1 = distancia2 > 0 ? (distancia1 * maxSpeedY) / distancia2 : maxSpeedX;
    float velocidad2 = distancia1 > 0 ? (distancia2 * maxSpeedX) / distancia1 : maxSpeedY;
    if(velocidad1 > maxSpeedX){
        velocidad1 = maxSpeedX;
    } else {
        velocidad2 = maxSpeedY;
    }

    auto tiempo1 = distancia1 / velocidad1; 
    auto tiempo2 = distancia2 / velocidad2; 

    estimado.delay = tiempo1 > tiempo2 ? tiempo1 : tiempo2;
    estimado.xVelocidad = velocidad1;
    estimado.yVelocidad = velocidad2;
}

float MotorsControllerCache::getTotalDelay(){
    float tiempoTotal = 0;
    for (const Dimensions& cacheItem : cache) {
        tiempoTotal += cacheItem.delay;
    }

    return tiempoTotal;
}

ViajeActual MotorsControllerCache::goTo(Dimensions newPos){
    ViajeActual viaje;
    viaje.x = newPos.x;
    viaje.y = newPos.y;
    viaje.delay = newPos.delay;
    estimarViaje(viaje);
    cache.push_back(viaje);
    queueLength++;
    viaje.delayTotal = getTotalDelay();

    ultimaParada = newPos;
    return viaje;
};