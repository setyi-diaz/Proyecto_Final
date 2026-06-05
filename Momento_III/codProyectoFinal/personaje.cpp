#include "personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      posX(0), posY(0),
      velocidadY(0),
      aceleracion(40.0f),
      velocidadMax(150.0f)
{
    setRect(0, 0, 50, 40);
}

Personaje::~Personaje() {}

float Personaje::getPosX() const { return posX; }
float Personaje::getPosY() const { return posY; }
void  Personaje::setPosX(float x) { posX = x; }
void  Personaje::setPosY(float y) { posY = y; }
