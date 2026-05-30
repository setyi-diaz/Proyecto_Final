#ifndef OBJETOEFECTO_H
#define OBJETOEFECTO_H

#include "objetopista.h"

class ObjetoEfecto : public ObjetoPista {
public:
    ObjetoEfecto(unsigned short posX, unsigned short posY, float efecto, QGraphicsItem *parent = nullptr);

    float getEfecto() const;
    bool esImpulso() const;

private:
    float efecto;
};

#endif // OBJETOEFECTO_H
