#ifndef H_BuscaCoorrdenadas
    #define H_BuscaCoorrdenadas

    #include <Arduino.h>
    #include <Carrito.h>
    #include <Dimensions.h>

    class BuscaCoorrdenadas
    {
    private:
        Carrito *_carrito1;
        Carrito *_carrito2;
        long _irHasta1 = 0;
        long _irHasta2 = 0;
    public:
        BuscaCoorrdenadas(Carrito *carrito1, Carrito *carrito2);
        ~BuscaCoorrdenadas();
        void irHasta(ViajeEstimado& estimacion);
        bool andar();
        long irHasta1();
        long irHasta2();
    };
    
#endif