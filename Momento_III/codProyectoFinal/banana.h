#ifndef BANANA_H
#define BANANA_H
#include "proyectil.h"

class Banana : public Proyectil
{
public:
    Banana();
    virtual void calcularVelnicial();
    virtual void calcularPosVuelo();
};

#endif // BANANA_H
