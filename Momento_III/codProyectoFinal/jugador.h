#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"

class Jugador : public Personaje
{
private:
    float rozamiento;
    float velocidadRot;
    float anguloRot;
    float bonusVelAcum;
public:
    Jugador(float ,float ,float ,float);
    void frenar(float dt);
    void inclinar(float dt);
    void evaluarAterrizaje();
    void aplicarBonus();
    void aplicarPenalizacion();
};

#endif // JUGADOR_H
