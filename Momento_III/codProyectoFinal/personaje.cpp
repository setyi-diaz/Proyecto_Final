#include "personaje.h"

Personaje::Personaje()
    : posX(0.f), posY(0.f),
      aceleracion(40.f),
      velocidadMax(150.f)
{}

float Personaje::getPosX() const       { return posX; }
float Personaje::getPosY() const       { return posY; }
void  Personaje::setPosX(float x)      { posX = x; }
void  Personaje::setPosY(float y)      { posY = y; }
float Personaje::getVelocidadMax() const { return velocidadMax; }
void  Personaje::setVelocidadMax(float v){ velocidadMax = v; }
float Personaje::getAceleracion() const  { return aceleracion; }
void  Personaje::setAceleracion(float a) { aceleracion = a; }
