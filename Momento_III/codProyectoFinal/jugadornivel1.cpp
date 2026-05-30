#include "jugadornivel1.h"
#include <QBrush>

JugadorNivel1::JugadorNivel1(QGraphicsItem *parent)
    : CorredorNivel1(parent),
      personalidad(nullptr),
      golpeDisponible(true)
{}

void JugadorNivel1::acelerar() {}

void JugadorNivel1::frenar() {}

void JugadorNivel1::golpearIzquierda() {}

void JugadorNivel1::golpearDerecha() {}

void JugadorNivel1::aplicarPersonalidad() {}

void JugadorNivel1::setPersonalidad(Personalidad *p) {}

bool JugadorNivel1::isGolpeDisponible() const { return false; }
