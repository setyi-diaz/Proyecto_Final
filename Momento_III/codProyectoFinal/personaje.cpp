#include "personaje.h"

Personaje::Personaje() {
    posX = 0;
    posY = 0;
    velocidadX = 0;
    velocidadY = 0;
    aceleracion = 0;
    velocidadMax = 50;
    enSuelo = true;
    anguloSprite = 0;
    velAlDespegue = 0;
}

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
void Personaje::setAceleracion(float newAceleracion)
{
    aceleracion = newAceleracion;
}
float Personaje::getVelocidadMax() const
{
    return velocidadMax;
}

void Personaje::setVelocidadMax(float newVelocidadMax)
{
    velocidadMax = newVelocidadMax;
}

float Personaje::getAnguloSprite() const
{
    return anguloSprite;
}

void Personaje::setAnguloSprite(float newAnguloSprite)
{
    anguloSprite = newAnguloSprite;
}

bool Personaje::getEnSuelo() const
{
    return enSuelo;
}

void Personaje::setEnSuelo(bool newEnSuelo)
{
    enSuelo = newEnSuelo;
}

void Personaje::acelerar(float dt){

    if(velocidadX < velocidadMax){
        velocidadX = velocidadX + (aceleracion * dt);
        if(velocidadX > velocidadMax)
            velocidadX = velocidadMax;
    }
}

void Personaje::aplicarGravedad(float dt,float g){
    velocidadY = velocidadY - (g * dt);
}

void Personaje::actualizarPosicion(float dt){
    posX = posX + (velocidadX*dt);
}

void Personaje::movParabolico(float dt,float g){
    posX = posX + velocidadX * dt;
    posY = posY + velocidadY * dt + 0.5f * g * dt * dt;
}
