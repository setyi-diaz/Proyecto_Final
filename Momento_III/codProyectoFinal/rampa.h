#ifndef RAMPA_H
#define RAMPA_H
#include "obstaculo.h"

class Rampa : public Obstaculo
{
private:
    float angulo;
    float cosAngulo;
    float senAngulo;
public:
    Rampa(float );
    Rampa();
    void calcularImpulsoX(float& );
    void calcularImpulsoY(float& );
    float getAngulo() const;
    void setAngulo(float newAngulo);
    float getCosAngulo() const;
    float getSenAngulo() const;
};

#endif // RAMPA_H
