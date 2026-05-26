#include "rampa.h"

float Rampa::getCosAngulo() const
{
    return cosAngulo;
}

float Rampa::getSenAngulo() const
{
    return senAngulo;
}

Rampa::Rampa(float a) {
    this->angulo = a * M_PI/180.f;
    this->cosAngulo = cos(a);
    this->senAngulo = sin(a);
}
Rampa::Rampa(){
    angulo = 30 * M_PI/180.f;
}
float Rampa::getAngulo() const
{
    return angulo;
}

void Rampa::setAngulo(float newAngulo)
{
    angulo = newAngulo;
}


void Rampa::calcularImpulsoX(float& vx){
    vx = vx*cosAngulo;
}
void Rampa::calcularImpulsoY(float& vy){
    vy = vy*senAngulo;
}
