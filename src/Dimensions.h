#ifndef H_Dimensions
    #define H_Dimensions

    #include <Arduino.h>

    struct PositionStatus
    {
        unsigned long x;
        unsigned long y;
        float delay;
        byte mode;
        unsigned long xNext;
        unsigned long yNext;
    };

    struct Dimensions
    {
        int32_t x;
        int32_t y;
        float delay;
    };

    struct ViajeEstimado : Dimensions
    {
        float xVelocidad;
        float yVelocidad;
    };

    struct ViajeActual : ViajeEstimado
    {
        long xActual;
        long yActual;
        float delayTotal;
    };

    struct DimensionsFull
    {
        const char* guid;
        long xPos;
        long yPos;
        float xSpeed;
        float ySpeed;
    };

    struct MotorsConfig
    {
        uint xMax;
        uint yMax;
        float xMaxSpeed;
        float yMaxSpeed;
    };

#endif
