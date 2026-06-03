#ifndef RIVAL_H
#define RIVAL_H
#include "personaje.h"
#include "proyectil.h"
#include <list>
using std::list;

class Rival : public Personaje
{
private:
    list<Proyectil*> bananas;
    float cooldowBanana;
    float acum;
    static constexpr float veriVentaja  = 2.0f;
public:
    Rival();
    ~Rival();
    bool tieneVentaja(float posxJ);
    void ajustarVelociadad(float velxJ);
    void lanzarBanana(float dt);
    void actualizarCoolDown(float dt);
};

#endif // RIVAL_H
