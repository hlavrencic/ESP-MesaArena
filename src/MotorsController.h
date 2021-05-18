#ifndef MotorController_H
    #define MotorController_H

    #include <AccelStepper.h>
    #include <BuscaCoorrdenadas.h>
    #include <Dimensions.h>

    #ifdef ESP32
        //include ESP32 specific libs
        const byte motor1Pin1 = GPIO_NUM_2;  
        const byte motor1Pin2 = GPIO_NUM_4;  
        const byte motor1Pin3 = GPIO_NUM_16; 
        const byte motor1Pin4 = GPIO_NUM_17; 

        const byte motor2Pin1 = GPIO_NUM_23;    
        const byte motor2Pin2 = GPIO_NUM_19;    
        const byte motor2Pin3 = GPIO_NUM_18;    
        const byte motor2Pin4 = GPIO_NUM_5;    

        const byte end1Pin = GPIO_NUM_33;
        const byte end2Pin = GPIO_NUM_27  ;
    #else  
        const byte motor1Pin1 = D1;  
        const byte motor1Pin2 = D2;  
        const byte motor1Pin3 = D3; 
        const byte motor1Pin4 = D4; 

        const byte motor2Pin1 = D5;      // IN1 on the ULN2003 driver
        const byte motor2Pin2 = D6;      // IN2 on the ULN2003 driver
        const byte motor2Pin3 = D7;     // IN3 on the ULN2003 driver
        const byte motor2Pin4 = D8;     // IN4 on the ULN2003 driver

        const byte end1Pin = A0;
        const byte end2Pin = A0;
    #endif

    enum MotorsControllerMode {
        CALIBRATING = 0,
        ERROR = 1,
        TRAVELLING = 2,
        STANDBY = 3,
    };

    class MotorsController {
    public:
        void begin();
        void loop();
        void getPos(Dimensions& dimensions);
        void getNext(Dimensions& dimensions);
        ViajeActual getCurrent();
        void goTo(ViajeEstimado& estimacion);
        void goTo(long xPos, long yPos, float xSpeed, float ySpeed);
        void startMoving(float speedX, float speedY);
        void config(unsigned int xMax, unsigned int yMax, float xMaxSpeed, float yMaxSpeed);
        MotorsControllerMode mode = MotorsControllerMode::CALIBRATING;
        float getMaxSpeedX();
        float getMaxSpeedY();
    private:
        ViajeActual viajeActual;
        bool _enabled = true;
        bool _arrived = false;
        unsigned long _arrivedTick = 0;
        AccelStepper stepper = AccelStepper(8, motor1Pin3, motor1Pin1 , motor1Pin2, motor1Pin4);
        Carrito* carrito1 = new Carrito(&stepper, 4000U, false, end1Pin);
        AccelStepper stepper2 = AccelStepper(8, motor2Pin4, motor2Pin2, motor2Pin1, motor2Pin3);
        Carrito* carrito2 = new Carrito(&stepper2, 35000U, false, end2Pin);

        BuscaCoorrdenadas buscaCoorrdenadas = BuscaCoorrdenadas(carrito1, carrito2);

        void enableMotors(bool state);
    };
    

    

#endif