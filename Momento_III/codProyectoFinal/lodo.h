#ifndef LODO_H
#define LODO_H
#include "obstaculo.h"

class Lodo : public Obstaculo
{
private:
    float  friccion;
public:
    Lodo();
    void generarFriccion(float& ,float );
};

#endif // LODO_H
