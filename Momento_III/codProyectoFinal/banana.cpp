#include "banana.h"
#include <cmath>
using std::cos;
using std::sin;

Banana::Banana() {}

void Banana::calcularVelnicial(float velxRival, float angulo){
    velocidadX = velxRival * cos(angulo);
    velocidadY = velxRival * sin(angulo);
}
void Banana::calcularPosVuelo(float dt,float g){
    posX += velocidadX * dt;
    posY += velocidadY * dt;
    velocidadY -= g * dt;
}
