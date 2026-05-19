#include "jugador.h"

Jugador::Jugador(){
    rozamiento = 20;
    velocidadRot = 180;
    anguloRot = 0;
    bonusVelAcum = 0;
}
Jugador::Jugador(float a, float b , float c , float d){
    rozamiento = a;
    velocidadRot = b;
    anguloRot = c;
    bonusVelAcum = d;
}

void Jugador::frenar(float dt){
    float vx = getVelocidadX();
    vx -= (rozamiento * dt);

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
