#include "rampa.h"
#include <cmath>
using std::cos;
using std::sin;
using std::sqrt;
using std::pow;

Rampa::Rampa(float grados, float x, float y, float w, float h)
    : Obstaculo(x, y, w, h){
    angulo  = grados * M_PI / 180.f;
    cosAngulo  = cos(angulo);
    senAngulo  = sin(angulo);
}

void Rampa::aplicarEfecto(Personaje* p, float dt) {
    if (!p->getEnSuelo()) return;
    float vx = p->getVelocidadX();
    p->setVelocidadX(vx * cosAngulo);
    p->setVelocidadY(vx * senAngulo);
    p->setVelAlDespegue(sqrt(pow(vx * cosAngulo,2.0)+pow(vx * senAngulo,2.0)));
    p->setEnSuelo(false);
}
float Rampa::getAngulo() const{
    return angulo;
}
void Rampa::setAngulo(float newAngulo){
    angulo = newAngulo;
}
float Rampa::getCosAngulo() const
{
    return cosAngulo;
}

float Rampa::getSenAngulo() const
{
    return senAngulo;
}


