#include "proyectil.h"

Proyectil::Proyectil(float X,float Y,float velX,float velY)
    : posX(X), posY(Y), velocidadX(velX), velocidadY(velY), activa(false){}

Proyectil::Proyectil()
    : posX(0), posY(0), velocidadX(0), velocidadY(0), activa(false){}

float Proyectil::getPosX() const
{
    return posX;
}

void Proyectil::setPosX(float newPosX)
{
    posX = newPosX;
}

float Proyectil::getPosY() const
{
    return posY;
}

void Proyectil::setPosY(float newPosY)
{
    posY = newPosY;
}

bool Proyectil::getActiva() const
{
    return activa;
}

void Proyectil::setActiva(bool newActiva)
{
    activa = newActiva;
}
