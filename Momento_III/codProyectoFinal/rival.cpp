#include "rival.h"
#include "banana.h"

Rival::Rival() {
    cooldowBanana = 0.0f;
    bananas.push_back(new Banana());
}
Rival::~Rival() {
    for (Proyectil* p : bananas)
        delete p;
    bananas.clear();
}
bool Rival::tieneVentaja(float posxJ) {
    if((posxJ - posX) <= 0.5f*posxJ)
        return true;
    else
        return false;
}
void Rival::ajustarVelociadad(float velxJ) {
    velocidadX = velxJ;
}
void Rival::lanzarBanana(float dt) {
    if (bananas.empty())
        return;
    Proyectil* banana = bananas.front();
    banana->calcularVelnicial(velocidadX,30);
    banana->calcularPosVuelo(dt,9.8f);
}
void Rival::actualizarCoolDown(float dt) {

}
