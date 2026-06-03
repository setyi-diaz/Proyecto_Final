#include "objetopista.h"
#include <QBrush>
#include <QPen>

ObjetoPista::ObjetoPista(float efecto, float posY, int carril,
                         float margenX, float anchoCarril, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      efecto(efecto),
      carril(carril),
      cooldown(0)
{
    if (efecto > 0) {
        setRect(0, 0, 22, 22);
        setBrush(QBrush(Qt::yellow));
        setPen(QPen(QColor(200, 160, 0), 1));
    } else {
        setRect(0, 0, 40, 14);
        setBrush(QBrush(QColor(80, 50, 20)));
        setPen(QPen(QColor(50, 30, 10), 1));
    }

    float x = (carril >= 0)
        ? margenX + carril * anchoCarril + anchoCarril * 0.5f
        : margenX;
    setPos(x, posY);
}

float ObjetoPista::getEfecto() const { return efecto; }
bool  ObjetoPista::esImpulso()  const { return efecto > 0; }
int   ObjetoPista::getCarril()  const { return carril; }

bool ObjetoPista::puedeColisionar() const { return cooldown <= 0; }

void ObjetoPista::activarCooldown(float duracion) { cooldown = duracion; }

void ObjetoPista::actualizarCooldown(float dt) {
    if (cooldown > 0) cooldown -= dt;
}
