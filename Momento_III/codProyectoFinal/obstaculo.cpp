#include "obstaculo.h"
Obstaculo::Obstaculo() : posX(0), posY(0), ancho(0), alto(0) {}

Obstaculo::Obstaculo(float x, float y, float w, float h)
    : posX(x), posY(y), ancho(w), alto(h) {}

bool Obstaculo::colisiona(const Personaje* p, float pAncho, float pAlto) const {
    return (p->getPosX() + pAncho > posX) &&
           (p->getPosX() < posX + ancho) &&
           (p->getPosY() < posY + alto) &&
           (p->getPosY() + pAlto  > posY);
}

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

