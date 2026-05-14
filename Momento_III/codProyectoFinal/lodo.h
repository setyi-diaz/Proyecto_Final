#ifndef LODO_H
#define LODO_H
#include "obstaculo.h"

class Lodo : public Obstaculo
{
private:
    unsigned short friccion;
public:
    Lodo();
    void generarFriccion(unsigned short);
};

#endif // LODO_H
