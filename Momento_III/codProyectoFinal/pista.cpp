#include "pista.h"
#include <QBrush>
#include <QPen>
#include <QColor>

Pista::Pista(short numCarriles, short longitud, QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      numCarriles(numCarriles),
      longitud(longitud)
{}

short Pista::obtenerCarril(int indice) const { return 0; }

bool Pista::carrilValido(int indice) const { return false; }

short Pista::getNumCarriles() const { return 0; }
short Pista::getLongitud() const { return 0; }
