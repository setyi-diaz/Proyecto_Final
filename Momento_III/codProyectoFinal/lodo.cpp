#include "lodo.h"
#include <algorithm>
using std::max;

Lodo::Lodo(float fr, float x, float y, float w, float h) {
    friccion = fr;
    posX = x; posY = y; ancho = w; alto = h;
}

void Lodo::aplicarEfecto(Personaje* personaje,float dt){
    float vx = personaje->getVelocidadX() - friccion * dt;
    personaje->setVelocidadX(max(0.0f, vx));
}

float Lodo::getFriccion() const{
    return friccion;
}

void Lodo::setFriccion(float newFriccion){
    friccion = newFriccion;
}
