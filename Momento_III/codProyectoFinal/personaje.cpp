#include "personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      posX(0), posY(0),
      velocidadX(0), velocidadY(0),
      aceleracion(40.0f), velocidadMax(150.0f),
      enSuelo(true), anguloGente(0.0f),
      velocidadAlDespegue(0.0f)
{
    setRect(0, 0, 50, 40);
}

Personaje::~Personaje() {}

void Personaje::aplicarGravedad(float dt) {}

void Personaje::actualizarPosicion(float dt) {
    posY += velocidadY * dt;
    setPos(posX, posY);
}

float Personaje::getPosX() const { return posX; }
float Personaje::getPosY() const { return posY; }
void Personaje::setPosX(float x) { posX = x; }
void Personaje::setPosY(float y) { posY = y; }
float Personaje::getVelocidadX() const { return velocidadX; }
float Personaje::getVelocidadY() const { return velocidadY; }
