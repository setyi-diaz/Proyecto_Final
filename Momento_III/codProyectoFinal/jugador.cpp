#include "jugador.h"

Jugador::Jugador(){
    friccionFreno = 50;
    friccionSuelo = 20;
    velocidadRot = 180;
    anguloRot = 0;
}
Jugador::Jugador(float a, float b , float c , float d){
    friccionFreno = a;
    friccionSuelo = b;
    velocidadRot = c;
    anguloRot = d;
}

void Jugador::frenar(float dt){
    velocidadX  -= friccionFreno * dt;
    if (velocidadX < 0) velocidadX = 0;
}

void Jugador::desaceleracionAuto(float dt){
    velocidadX -= friccionSuelo * dt;
    if (velocidadX < 0) velocidadX = 0;
}
void Jugador::inclinar(float dt){
    anguloRot   += velocidadRot * dt;
    anguloSprite = anguloRot;
}
void Jugador::aplicarBonus(){
    if (anguloRot <= 90)
        velocidadMax *= 1.05f;
    else if (anguloRot <= 180)
        velocidadMax *= 1.10f;
    else
        velocidadMax *= 1.15f;
}
void Jugador::evaluarAterrizaje(){
    if (anguloSprite >= -15.0f && anguloSprite <= 15.0f)
        aplicarBonus();
    else
        aplicarPenalizacion();

    enSuelo = true;
    velocidadY = 0.0f;
    anguloRot = 0.0f;
    anguloSprite = 0.0f;
}

void Jugador::aplicarPenalizacion(){
    velocidadMax *= 0.85f;
}
