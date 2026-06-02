#include "objetopista.h"
#include <QBrush>
#include <QPen>

ObjetoPista::ObjetoPista(float efecto, float posY, int carril,
                         float margenX, float anchoCarril, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      efecto(efecto),
      carril(carril)
{
    if (efecto > 0) {
        // Banana: circulo amarillo compacto
        setRect(0, 0, 22, 22);
        setBrush(QBrush(Qt::yellow));
        setPen(QPen(QColor(200, 160, 0), 1));
    } else {
        // Lodo u obstaculo: elipse ancha y baja
        setRect(0, 0, 40, 14);
        setBrush(QBrush(QColor(80, 50, 20)));
        setPen(QPen(QColor(50, 30, 10), 1));
    }

    float x = (carril >= 0)
        ? margenX + carril * anchoCarril + anchoCarril * 0.25f
        : margenX;
    setPos(x, posY);
}

float ObjetoPista::getEfecto() const { return efecto; }
bool  ObjetoPista::esImpulso()  const { return efecto > 0; }
int   ObjetoPista::getCarril()  const { return carril; }
