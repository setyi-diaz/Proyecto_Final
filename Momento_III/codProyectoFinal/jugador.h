#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"

class Jugador : public Personaje
{
private:
    float friccionFreno;
    float friccionSuelo;
    float velocidadRot;
    float anguloRot;

    void aplicarBonus();
    void aplicarPenalizacion();
public:
    Jugador(float ,float ,float ,float);
    Jugador();
    void frenar(float dt);
    void desaceleracionAuto(float dt);
    void inclinar(float dt);
    void evaluarAterrizaje();
};

#endif // JUGADOR_H
