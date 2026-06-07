#include "jugadornivel2.h"
#include <cmath>

JugadorNivel2::JugadorNivel2()
    : friccionFreno(50.f), friccionSuelo(20.f), velocidadRot(180.f), anguloRot(0.f)
{}

JugadorNivel2::JugadorNivel2(float a, float b, float c, float d)
    : friccionFreno(a), friccionSuelo(b), velocidadRot(c), anguloRot(d)
{}

void JugadorNivel2::acelerar(float dt) {
    velocidadX += aceleracion * dt;
    if (velocidadX > velocidadMax) velocidadX = velocidadMax;
}

void JugadorNivel2::frenar(float dt) {
    velocidadX -= friccionFreno * dt;
    if (velocidadX < 0) velocidadX = 0;
}

void JugadorNivel2::desaceleracionAuto(float dt) {
    // Aplicar rozamiento solo si hay velocidad
    if (velocidadX > 0) {
        velocidadX -= friccionSuelo * dt;
        if (velocidadX < 0) velocidadX = 0;
    }
}

void JugadorNivel2::inclinar(float dt) {
    anguloRot   += velocidadRot * dt;
    anguloSprite = anguloRot;
}

void JugadorNivel2::aplicarBonus() {
    float anguloAbs = std::abs(anguloRot);
    float bonusVMax = 1.f;
    float impulsoVel = 0.f;

    if (anguloAbs >= 0 && anguloAbs <= 90) {
        bonusVMax = 1.02f;
        impulsoVel = 3.f;
    } else if (anguloAbs > 90 && anguloAbs <= 180) {
        bonusVMax = 1.04f;
        impulsoVel = 5.f;
    } else if (anguloAbs > 180) {
        bonusVMax = 1.06f;
        impulsoVel = 8.f;
    }

    velocidadMax *= bonusVMax;
    // Tope máximo absoluto para que el juego no se vuelva inmanejable
    if (velocidadMax > 130.f) velocidadMax = 130.f;

    velocidadX += impulsoVel;
    if (velocidadX > velocidadMax) velocidadX = velocidadMax;
}

JugadorNivel2::ResultadoAterrizaje JugadorNivel2::evaluarAterrizaje() {
    float anguloAbs = std::abs(anguloRot);
    ResultadoAterrizaje resultado;

    if (anguloAbs >= 360.f || anguloAbs < 10.f) {
        // Vuelta completa o sin rotación: bonus
        aplicarBonus();
        resultado = ResultadoAterrizaje::BONUS;
    } else {
        // Normalizar ángulo a -180..180 para detectar caída de espalda
        float norm = std::fmod(anguloAbs, 360.f);
        if (norm > 180.f) norm = 360.f - norm;
        // norm ahora es el ángulo más cercano: 0=derecho, 180=espalda

        if (norm >= 135.f) {
            // Caída de espalda (135°-180°): penalización severa
            velocidadMax *= 0.75f;
            velocidadX   *= 0.4f;
            resultado = ResultadoAterrizaje::PENALIZACION_ESPALDA;
        } else {
            // Rotación parcial sin caer de espalda: penalización leve
            velocidadMax *= 0.90f;
            resultado = ResultadoAterrizaje::PENALIZACION_LEVE;
        }
    }

    enSuelo      = true;
    velocidadY   = 0.f;
    anguloRot    = 0.f;
    anguloSprite = 0.f;
    return resultado;
}


void JugadorNivel2::reiniciarRotacion() {
    anguloRot    = 0.f;
    anguloSprite = 0.f;
}

void JugadorNivel2::actualizarPosicion(float dt) {
    posX += velocidadX * dt;
    if (enSuelo) return;
    posY += velocidadY * dt;
}

void JugadorNivel2::aplicarGravedad(float dt, float g) {
    velocidadY -= g * dt;
}
