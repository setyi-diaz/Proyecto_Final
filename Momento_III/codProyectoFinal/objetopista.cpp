#include "objetopista.h"

ObjetoPista::ObjetoPista(unsigned short posX, unsigned short posY, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      posX(posX),
      posY(posY)
{}

ObjetoPista::~ObjetoPista() {}

unsigned short ObjetoPista::getPosX() const { return 0; }
unsigned short ObjetoPista::getPosY() const { return 0; }
