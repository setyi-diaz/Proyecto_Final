#include "rampa.h"

float Rampa::getAngulo() const
{
    return angulo;
}

void Rampa::setAngulo(float newAngulo)
{
    angulo = newAngulo;
}

Rampa::Rampa() {}

float Rampa::calcularImpulsoX(float vx){
    vxImpulso = vx*cos(angulo);
    return vxImpulso;
}
float calcularImpulsoY(float vy){
    vyImpulso = vy*sin(angulo);
    return vyImpulso;
}
