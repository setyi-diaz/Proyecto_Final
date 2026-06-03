#include "proyectil.h"

Proyectil::Proyectil(float X,float Y,float velX,float velY)
    : posX(X), posY(Y), velocidadX(velX), velocidadY(velY){}

Proyectil::Proyectil()
    : posX(0), posY(0), velocidadX(0), velocidadY(0){}
