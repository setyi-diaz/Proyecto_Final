#include "personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      posX(0), posY(0),
      velocidadX(0), velocidadY(0),
      aceleracion(40.0f), velocidadMax(150),
      enSuelo(true), anguloGente(0),
      velocidadAlDespegue(0)
{}

Personaje::~Personaje() {}

void Personaje::aplicarGravedad(float dt) {}

void Personaje::actualizarPosicion(float dt) {}

float Personaje::getPosX() const { return 0.0f; }
float Personaje::getPosY() const { return 0.0f; }
void Personaje::setPosX(float x) {}
void Personaje::setPosY(float y) {}
float Personaje::getVelocidadX() const { return 0.0f; }
float Personaje::getVelocidadY() const { return 0.0f; }
