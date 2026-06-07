#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <cmath>
#include "jugadornivel2.h"

class RivalNivel2;

class Obstaculo
{
protected:
    float posX;
    float posY;
    float ancho;
    float alto;
public:
    Obstaculo();
    Obstaculo(float x, float y, float w, float h);
    virtual ~Obstaculo() = default;

    float getPosX() const;
    float getPosY() const;
    float getAncho() const;
    float getAlto() const;
    void setPosX(float newPosX);
    void setPosY(float newPosY);

    bool colisiona(const JugadorNivel2* p, float pAncho, float pAlto) const;
    virtual void aplicarEfecto(JugadorNivel2* personaje, float dt) = 0;
    virtual void aplicarEfecto(RivalNivel2* personaje, float dt) = 0;
};

#endif // OBSTACULO_H
