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
    Rampa(float gradoAngulo, float x, float y, float w, float h);

    void aplicarEfecto(JugadorNivel2* personaje, float dt) override;
    void aplicarEfecto(RivalNivel2* personaje, float dt) override;
    float getAngulo() const;
    void setAngulo(float newAngulo);
    float getCosAngulo() const;
    float getSenAngulo() const;
};

#endif // RAMPA_H
