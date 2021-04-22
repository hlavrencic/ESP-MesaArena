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
        const byte motor1Pin1 = 0;  
        const byte motor1Pin2 = 0;  
        const byte motor1Pin3 = 0; 
        const byte motor1Pin4 = 0; 

        const byte motor2Pin1 = 3;      // IN1 on the ULN2003 driver
        const byte motor2Pin2 = 4;      // IN2 on the ULN2003 driver
        const byte motor2Pin3 = 5;     // IN3 on the ULN2003 driver
        const byte motor2Pin4 = 6;     // IN4 on the ULN2003 driver

        const byte end1Pin = 10;
        const byte end2Pin = 11;
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
        void goTo(Dimensions& dimensions);
        void startMoving(float speedX, float speedY);
        MotorsControllerMode mode = MotorsControllerMode::CALIBRATING;
    private:
        bool _enabled = true;
        bool _arrived = false;
        unsigned long _arrivedTick = 0;
        AccelStepper stepper = AccelStepper(8, motor1Pin3, motor1Pin1 , motor1Pin2, motor1Pin4);
        Carrito carrito1 = Carrito(&stepper, 4000U, false, end1Pin);

        AccelStepper stepper2 = AccelStepper(8, motor2Pin4, motor2Pin2, motor2Pin1, motor2Pin3);
        Carrito carrito2 = Carrito(&stepper2, 35000U, false, end2Pin);

        BuscaCoorrdenadas buscaCoorrdenadas = BuscaCoorrdenadas(&carrito1, &carrito2);

        void enableMotors(bool state);
    };
    

    

#endif