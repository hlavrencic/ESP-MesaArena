#include <Services/MotorsController.h>

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
    stepper2.setMaxSpeed(600);


}

float MotorsController::getMaxSpeedX(){
    return stepper.maxSpeed();
}

float MotorsController::getMaxSpeedY(){
    return stepper2.maxSpeed();
}

ViajeActual MotorsController::getCurrent(){
    viajeActual.xActual = stepper.currentPosition();
    viajeActual.yActual = stepper2.currentPosition();
    return viajeActual;
}

void MotorsController::setCurrentPosition(long xPos, long yPos){
    stepper.setCurrentPosition(xPos);
    stepper2.setCurrentPosition(yPos);
    mode = MotorsControllerMode::STANDBY;
}

void MotorsController::goTo(ViajeEstimado& estimacion){
    if(estimacion.xVelocidad == 0 && estimacion.yVelocidad == 0) return;
    if(mode != MotorsControllerMode::STANDBY) return;
    mode = MotorsControllerMode::TRAVELLING;
    buscaCoorrdenadas.irHasta(estimacion);
}

void MotorsController::loop(){
    switch (mode)
    {
    case MotorsControllerMode::CALIBRATING:
    {
        enableMotors(true);
        auto calibrado1 = carrito1->calibrar();
        auto calibrado2 = carrito2->calibrar();

        if(calibrado1 && calibrado2){
            mode = MotorsControllerMode::STANDBY;
        }
    }
    break;
    case MotorsControllerMode::ERROR:
    {
        enableMotors(false);
    }
    break;
    case MotorsControllerMode::TRAVELLING:
        enableMotors(true);
        _arrived = buscaCoorrdenadas.andar();
        if(_arrived){
            mode = MotorsControllerMode::STANDBY; 
            _arrivedTick = millis();
        } else if(carrito1->getState() == EstadoCalibracion::errorCalibracion || carrito2->getState() == EstadoCalibracion::errorCalibracion) {
            Serial.println("ERROR CALIBRACION");
            mode = MotorsControllerMode::ERROR;
        }

        break;
    case MotorsControllerMode::STANDBY:
        if(_enabled && _arrivedTick + 1000 < millis()){
            enableMotors(false);
        }
        break;
    default:
        Serial.print("MODO NO RECONOCIDO: "); Serial.println(mode);
        mode = MotorsControllerMode::ERROR;
        break;
    }
    
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

void MotorsController::config(unsigned int xMax, unsigned int yMax, float xMaxSpeed, float yMaxSpeed){
    stepper.setMaxSpeed(xMaxSpeed);
    stepper2.setMaxSpeed(yMaxSpeed);
    delete(carrito1);
    delete(carrito2);
    carrito1=new Carrito(&stepper, xMax, false, end1Pin);
    carrito2=new Carrito(&stepper2, yMax, false, end2Pin);
    mode = MotorsControllerMode::CALIBRATING;
}