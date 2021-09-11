#include <Services/BuscaCoorrdenadas.h>
#include <Services/Dimensions.h>

BuscaCoorrdenadas::BuscaCoorrdenadas(Carrito *carrito1, Carrito *carrito2)
{
    _carrito1 = carrito1;
    _carrito2 = carrito2;
}

BuscaCoorrdenadas::~BuscaCoorrdenadas()
{
}

long BuscaCoorrdenadas::irHasta1(){
    return _irHasta1;
}

long BuscaCoorrdenadas::irHasta2(){
    return _irHasta2;
}

void BuscaCoorrdenadas::irHasta(ViajeEstimado& estimacion)
{
    _carrito1->moveTo(estimacion.x, estimacion.xVelocidad);
    _carrito2->moveTo(estimacion.y, estimacion.yVelocidad);
}

bool BuscaCoorrdenadas::andar(){
    auto llego1 = _carrito1->andar();
    auto llego2 = _carrito2->andar();
    return llego1 && llego2;
}
