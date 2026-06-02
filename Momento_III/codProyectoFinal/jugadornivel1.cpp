#include "jugadornivel1.h"
#include <QBrush>

JugadorNivel1::JugadorNivel1(QGraphicsItem *parent)
    : CorredorNivel1(parent),
      personalidad(nullptr)
{
    setColorBase(Qt::blue);
}

void JugadorNivel1::acelerar() {
    velocidadY += aceleracion;
    if (velocidadY > velocidadMax) velocidadY = velocidadMax;
}

void JugadorNivel1::frenar() {
    velocidadY -= aceleracion * 0.5f;
    if (velocidadY < 0) velocidadY = 0;
}

void JugadorNivel1::aplicarPersonalidad() {}

void JugadorNivel1::setPersonalidad(Personalidad *p) {
    personalidad = p;
}
