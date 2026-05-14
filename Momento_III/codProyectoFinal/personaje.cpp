#include "personaje.h"

Personaje::Personaje() {}

void Personaje::aplicarGravedad(float dt){
    unsigned short g = 500; //500 px/s^2
    velocidadY = velocidadY - (g * dt);
}

void Personaje::actualizarPosicion(float dt){

}

