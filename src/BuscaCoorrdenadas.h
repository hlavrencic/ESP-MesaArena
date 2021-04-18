#ifndef H_BuscaCoorrdenadas
    #define H_BuscaCoorrdenadas

    #include <Arduino.h>
    #include <Carrito.h>

    class BuscaCoorrdenadas
    {
    private:
        Carrito *_carrito1;
        Carrito *_carrito2;
        float _ajusteVelocidad1 = 0.4F;
        unsigned short VELOCIDAD_MAX = 1000;
        long _irHasta1 = 0;
        long _irHasta2 = 0;
    public:
        BuscaCoorrdenadas(Carrito *carrito1, Carrito *carrito2);
        ~BuscaCoorrdenadas();
        void irHasta(long pos1, long pos2);
        bool andar();
        long irHasta1();
        long irHasta2();
    };
    

#endif