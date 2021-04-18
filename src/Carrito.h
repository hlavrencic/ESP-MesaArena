#ifndef H_Carrito
    #define H_Carrito

    #include <AccelStepper.h>

    enum EstadoCalibracion { sinCalibrar, buscandoSensor, chocoSensor, alejandose, calibrado, errorCalibracion };

    class Carrito {
        public:
            Carrito(AccelStepper *stepper, unsigned int maxPos, bool logOn, uint8_t endPin);
            bool andar();
            bool calibrar();
            void setSpeed(float speed);
            float getMaxSpeed();
            void moveTo(long pos, float speed);
            long getPos();
        private:
            int8_t getSentido();
            AccelStepper *_stepper;
            unsigned int _maxPos;
            EstadoCalibracion _estadoCalibracion = EstadoCalibracion::sinCalibrar;
            bool _logOn;
            uint8_t _endPin;
    };



#endif