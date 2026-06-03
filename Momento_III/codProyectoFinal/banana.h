#ifndef BANANA_H
#define BANANA_H
#include "proyectil.h"

class Banana : public Proyectil
{
public:
    Banana();
    void calcularVelnicial(float velxRival, float angulo) override;
    void calcularPosVuelo(float dt,float g) override;
};

#endif // BANANA_H
