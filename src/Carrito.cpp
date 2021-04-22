#include <Carrito.h>

Carrito::Carrito(
    AccelStepper *stepper, 
    unsigned int maxPos,
    bool logOn,
    uint8_t endPin
    ){
    _stepper = stepper;
    _maxPos = maxPos;
    _logOn = logOn;
    _endPin = endPin;

    _stepper->setAcceleration(1000);
}

float Carrito::getMaxSpeed(){
    return _stepper->maxSpeed();
}

void Carrito::setSpeed(float speed){
    
    if(_logOn){
        Serial.print("setSpeed(");
        Serial.print(speed);
        Serial.println(")");
    }
            
    _stepper->setSpeed(speed);

    return;             
}

int8_t Carrito::getSentido(){
    auto speed = _stepper->speed();
    int8_t sentidoActual;
    if(speed < 0) sentidoActual = -1;
    else if (speed > 0) sentidoActual = 1;
    else sentidoActual = 0;
    return sentidoActual;
}

bool Carrito::calibrar(){
    auto choqueConSensor = !digitalRead(_endPin);

    if(choqueConSensor){
        if(_logOn) Serial.println("[CHOCO SENSOR]");
        _estadoCalibracion = EstadoCalibracion::chocoSensor;
        setSpeed(_stepper->maxSpeed());
    } else {
        switch (_estadoCalibracion)
        {
        case EstadoCalibracion::sinCalibrar:
            if(_logOn) Serial.println("[INICIO CALIBRACION]");
            _estadoCalibracion = EstadoCalibracion::buscandoSensor;
            _stepper->setCurrentPosition(_maxPos);// Asumo que estoy en el otro extremo y comienzo a bajar
            setSpeed(-_stepper->maxSpeed());
            break;
        case EstadoCalibracion::buscandoSensor:
            // Esperar a que toque el sensor...
            break;
        case EstadoCalibracion::chocoSensor :
            if(_logOn) Serial.println("[DEJO DE CHOCAR]");
            _estadoCalibracion = EstadoCalibracion::alejandose;
            _stepper->setCurrentPosition(0);
            setSpeed(_stepper->maxSpeed());
            break;
        case EstadoCalibracion::alejandose:
            // 1% alejado del sensor
            if(_stepper->currentPosition() >= _maxPos/100){
                if(_logOn) Serial.println("[ALEJADO/CALIBRADO]");
                _estadoCalibracion = EstadoCalibracion::calibrado;
                _stepper->setCurrentPosition(0);
                return true;
            }
            break;
        case EstadoCalibracion::calibrado:
            setSpeed(0);
            return true;
            break;
        default:
            _estadoCalibracion = EstadoCalibracion::errorCalibracion;
            return false;
            break;
        }
    }

    auto fin = andar();
    if(fin){
        _estadoCalibracion = EstadoCalibracion::errorCalibracion;
        if(_logOn) Serial.println("[ERROR CALIBRACION]");
    }

    return false;
}

bool Carrito::andar(){

    if(_estadoCalibracion == EstadoCalibracion::errorCalibracion) {
        if(_logOn) Serial.println("Need recalibration.");
        return false;
    }

    if(_estadoCalibracion == EstadoCalibracion::calibrado){
        auto choqueConSensor = !digitalRead(_endPin);
        if(choqueConSensor){
            if(_logOn) Serial.println("CHOQUE SENSOR.");
            _estadoCalibracion = EstadoCalibracion::errorCalibracion;
            setSpeed(0);
            return false;
        }
    }

    if(_stepper->speed() == 0) {
        if(_logOn) Serial.println("Speed NULL.");
        return true;
    }

    auto pos = _stepper->currentPosition();

    int8_t estoyEnElLimite = 0;
    if(pos <= 0){
        estoyEnElLimite = -1;
    } else if(pos >= _maxPos){
        estoyEnElLimite = 1;
    } else if (pos == _stepper->targetPosition()){
        if(_logOn) Serial.println("Reach target.");
        setSpeed(0);
        return true;
    }

    if(estoyEnElLimite * getSentido() == 1 ){ // Sin choque
        setSpeed(0);
        return true;
    } 

    _stepper->runSpeed();
    return false;
}

void Carrito::moveTo(long pos, float speed){
    if(_logOn) Serial.println("moveTo...");
    _stepper->moveTo(pos);

    if(_stepper->currentPosition() > pos) speed = -speed;  // Invierto el sentido
    setSpeed(speed);
}

long Carrito::getPos(){
    return _stepper->currentPosition();
}

EstadoCalibracion Carrito::getState(){
    return _estadoCalibracion;
}