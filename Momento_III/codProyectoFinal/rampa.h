#ifndef RAMPA_H
#define RAMPA_H
#include "obstaculo.h"

class Rampa : public Obstaculo
{
private:
    float angulo;
public:
    Rampa();
    float calcularImpulsoX();
    float calcularImpulsoY();
};

#endif // RAMPA_H
