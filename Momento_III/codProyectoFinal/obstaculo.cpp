#include "obstaculo.h"

float Obstaculo::getPosX() const
{
    return posX;
}

float Obstaculo::getPosY() const
{
    return posY;
}

float Obstaculo::getAncho() const
{
    return ancho;
}

float Obstaculo::getAlto() const
{
    return alto;
}

void Obstaculo::setPosX(float newPosX)
{
    posX = newPosX;
}

void Obstaculo::setPosY(float newPosY)
{
    posY = newPosY;
}

Obstaculo::Obstaculo() {}
