#ifndef RIVAL_H
#define RIVAL_H
#include "personaje.h"
#include "proyectil.h"

#include <list>
using std::list;

class Rival : public Personaje
{
private:
    list<Proyectil*> proyectiles;
    float cooldowBanana;
public:
    Rival();
    virtual void acelerar();
    bool tieneVentaja();
    void ajustarVelociadad();
    void lanzarRoca();
    void actualizarCoolDown(float dt);
};

#endif // RIVAL_H
