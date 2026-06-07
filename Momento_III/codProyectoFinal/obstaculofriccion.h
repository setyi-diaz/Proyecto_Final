#ifndef OBSTACULOFRICCION_H
#define OBSTACULOFRICCION_H
#include "obstaculo.h"

class ObstaculoFriccion : public Obstaculo {
public:
    enum Tipo { LODO, TRONCO, CHARCO };

    ObstaculoFriccion(Tipo tipo, float friccion, float x, float y, float w, float h);
    void aplicarEfecto(JugadorNivel2* personaje, float dt) override;
    void aplicarEfecto(RivalNivel2* personaje, float dt) override;

    Tipo  getTipo()     const { return tipo; }
    float getFriccion() const { return friccion; }

private:
    Tipo  tipo;
    float friccion;
};

#endif // OBSTACULOFRICCION_H
