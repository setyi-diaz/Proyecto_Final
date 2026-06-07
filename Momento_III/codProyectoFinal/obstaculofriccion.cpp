#include "obstaculofriccion.h"
#include "rivalnivel2.h"
#include <algorithm>

ObstaculoFriccion::ObstaculoFriccion(Tipo tipo, float friccion, float x, float y, float w, float h)
    : Obstaculo(x, y, w, h), tipo(tipo), friccion(friccion)
{}

void ObstaculoFriccion::aplicarEfecto(JugadorNivel2* personaje, float dt) {
    float vx = personaje->getVelocidadX() - friccion * dt;
    personaje->setVelocidadX(std::max(0.0f, vx));
}

void ObstaculoFriccion::aplicarEfecto(RivalNivel2* personaje, float dt) {
    float vx = personaje->getVelocidadX() - friccion * dt;
    personaje->setVelocidadX(std::max(0.0f, vx));
}
