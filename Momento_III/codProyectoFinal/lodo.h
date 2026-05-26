#ifndef LODO_H
#define LODO_H
#include "obstaculo.h"

class Lodo : public Obstaculo
{
private:
    float  friccion;
    float posX;
    float posY;
    float ancho;
    float alto;
public:
    Lodo(float f, float x, float y, float w, float h);
    void generarFriccion(float& ,float );
};

#endif // LODO_H
