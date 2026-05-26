#include "jugador.h"

Jugador::Jugador(){
    friccionFreno = 50;
    friccionSuelo = 20;
    velocidadRot = 180;
    anguloRot = 0;
    bonusVelAcum = 0;
}
Jugador::Jugador(float a, float b , float c , float d, float e){
    friccionFreno = a;
    friccionSuelo = b;
    velocidadRot = c;
    anguloRot = d;
    bonusVelAcum = e;
}

void Jugador::frenar(float dt){
    float vx = getVelocidadX();
    vx -= (friccionFreno * dt);

    setVelocidadX(vx);
}

void Jugador::desaceleracionAuto(float dt){
    float vx = getVelocidadX();
    vx -= (friccionSuelo * dt);

    setVelocidadX(vx);
}
void Jugador::inclinar(float dt){
    float angulo = getAnguloSprite();
    angulo = angulo + (velocidadRot * dt);
    setAnguloSprite(angulo);
}
void Jugador::aplicarBonus(){
    if(anguloRot <= 90){
        bonusVelAcum = 0.05*getVelocidadMax();
        setVelocidadMax(bonusVelAcum + getVelocidadMax());
    }
    else if (anguloRot <= 180){
        bonusVelAcum = 0.1*getVelocidadMax();
        setVelocidadMax(bonusVelAcum + getVelocidadMax());
    }
    else if (anguloRot <= 360){
        bonusVelAcum = 0.15*getVelocidadMax();
        setVelocidadMax(bonusVelAcum + getVelocidadMax());
    }
    else{
        bonusVelAcum = -0.1*getVelocidadMax();
        setVelocidadMax(bonusVelAcum + getVelocidadMax());
    }
}
void Jugador::evaluarAterrizaje(){

}

void Jugador::aplicarPenalizacion(){

}
