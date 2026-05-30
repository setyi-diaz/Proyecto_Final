#include "obstaculo.h"
#include <QBrush>
#include <QColor>

Obstaculo::Obstaculo(int carril, float posY, float margenX, float anchoCarril, QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      carril(carril),
      penalizacion(30.0f)
{}

int Obstaculo::getCarril() const { return 0; }
float Obstaculo::getPenalizacion() const { return 0.0f; }
