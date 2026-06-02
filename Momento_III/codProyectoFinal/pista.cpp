#include "pista.h"
#include <QBrush>
#include <QPen>
#include <QColor>

Pista::Pista(short numCarriles, short longitud, float anchoCarril, QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      numCarriles(numCarriles),
      longitud(longitud)
{
    setRect(0, 0, numCarriles * anchoCarril, 900);
    setBrush(QBrush(QColor(50, 50, 50)));
}

short Pista::getNumCarriles() const { return numCarriles; }
