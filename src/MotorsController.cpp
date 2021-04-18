#include <MotorsController.h>

void MotorsController::begin(){
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor1Pin3, OUTPUT);
    pinMode(motor1Pin4, OUTPUT);

    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(motor2Pin3, OUTPUT);
    pinMode(motor2Pin4, OUTPUT);

    pinMode(end1Pin, INPUT_PULLUP);
    pinMode(end2Pin, INPUT_PULLUP);

    stepper.setMaxSpeed(300);
    stepper2.setMaxSpeed(1000);


}

void MotorsController::getPos(Dimensions& dimensions){
    dimensions.x = stepper.currentPosition();
    dimensions.y = stepper2.currentPosition();
}

void MotorsController::goTo(Dimensions& dimensions){
    if(mode != MotorsControllerMode::STANDBY) return;
    mode = MotorsControllerMode::TRAVELLING;
    buscaCoorrdenadas.irHasta(dimensions.x, dimensions.y);
}

void MotorsController::loop(){
    switch (mode)
    {
    case MotorsControllerMode::CALIBRATING:
    {
        enableMotors(true);
        auto calibrado1 = carrito1.calibrar();
        auto calibrado2 = carrito2.calibrar();

        if(calibrado1 && calibrado2){
            mode = MotorsControllerMode::STANDBY;
        }
    }
    break;
    case MotorsControllerMode::TRAVELLING:
        enableMotors(true);
        _arrived = buscaCoorrdenadas.andar();
        if(_arrived){
            Serial.println("ARRIVED");
            mode = MotorsControllerMode::STANDBY; 
            _arrivedTick = millis();
        }
        break;
    case MotorsControllerMode::STANDBY:
        if(_enabled && _arrivedTick + 1000 < millis()){
            Serial.println("DISABLING...");
            enableMotors(false);
        }
        break;
    default:
        mode = MotorsControllerMode::ERROR;
        Serial.println("ERROR.");
        break;
    }
    
}

void MotorsController::startMoving(float speedX, float speedY){
    carrito1.setSpeed(speedX);
    carrito2.setSpeed(speedY);
}

void MotorsController::enableMotors(bool state){
    if(state && !_enabled){
        _enabled = true;
        stepper.enableOutputs();
        stepper2.enableOutputs();
    } else if (!state && _enabled)
    {
        _enabled = false;
        stepper.disableOutputs();
        stepper2.disableOutputs();     
    }
}