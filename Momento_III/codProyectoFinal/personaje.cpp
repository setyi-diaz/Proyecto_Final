#include "personaje.h"

Personaje::Personaje() {}

float Personaje::getPosX() const
{
    return posX;
}

void Personaje::setPosX(float newPosX)
{
    posX = newPosX;
}

float Personaje::getPosY() const
{
    return posY;
}

void Personaje::setPosY(float newPosY)
{
    posY = newPosY;
}

float Personaje::getVelocidadX() const
{
    return velocidadX;
}

void Personaje::setVelocidadX(float newVelocidadX)
{
    velocidadX = newVelocidadX;
}

float Personaje::getVelocidadY() const
{
    return velocidadY;
}

void Personaje::setVelocidadY(float newVelocidadY)
{
    velocidadY = newVelocidadY;
}

float Personaje::getAceleracion() const
{
    return aceleracion;
}

float Personaje::getVelocidadMax() const
{
    return velocidadMax;
}

void Personaje::setVelocidadMax(float newVelocidadMax)
{
    velocidadMax = newVelocidadMax;
}

void Personaje::acelerar(float dt){

    if(velocidadX < velocidadMax){
        velocidadX = velocidadX + (a * dt);
    }
}

void Personaje::aplicarGravedad(float dt,float g){
    velocidadY = velocidadY - (g * dt);
}

void Personaje::actualizarPosicion(float dt){

}

void Personaje::movParabolico(float dt,float g){
    posX = posX + velocidadX * dt;
    posY = posY + velocidadY * dt + 0.5 * g * dt * dt;
}
