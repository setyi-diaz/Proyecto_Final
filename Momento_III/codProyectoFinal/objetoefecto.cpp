#include "objetoefecto.h"

ObjetoEfecto::ObjetoEfecto(unsigned short posX, unsigned short posY, float efecto, QGraphicsItem *parent)
    : ObjetoPista(posX, posY, parent), efecto(efecto)
{}

float ObjetoEfecto::getEfecto() const { return 0.0f; }
bool  ObjetoEfecto::esImpulso()  const { return false; }
