#ifndef LODO_H
#define LODO_H
#include "obstaculo.h"

class Lodo : public Obstaculo
{
private:
    float friccion;
public:
    Lodo(float fr, float x, float y, float w, float h);
    void aplicarEfecto(Personaje* personaje,float dt) override;
    float getFriccion() const;
    void setFriccion(float newFriccion);
};

#endif // LODO_H
