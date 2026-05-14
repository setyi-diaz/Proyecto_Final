#ifndef JUGADOR_H
#define JUGADOR_H
#include "personaje.h"

class Jugador : public Personaje
{
private:
    float velocidadRot;
    float anguloRot;
    float bonusVelAcum;
public:
    Jugador();
    virtual void acelerar();
    void frenar();
    void inclinar();
    void evaluarAterrizaje();
    void aplicarBonus();
    void aplicarPenalizacion();

};

#endif // JUGADOR_H
