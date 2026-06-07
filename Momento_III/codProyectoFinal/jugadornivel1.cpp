#include "jugadornivel1.h"

JugadorNivel1::JugadorNivel1() : CorredorNivel1() {
    setColorBase(Qt::blue);
}

void JugadorNivel1::acelerar(float dt) {
    velocidadAvance += aceleracion * dt;
    if (velocidadAvance > velocidadMax) velocidadAvance = velocidadMax;
}

void JugadorNivel1::frenar(float dt) {
    velocidadAvance -= aceleracion * 0.5f * dt;
    if (velocidadAvance < 0) velocidadAvance = 0;
}

QString JugadorNivel1::rutaSprite() const {
    if (!hayPersonalidad) return ":/nivel1/SimioGolpeadorUsuario.png";
    const QString &nombre = personalidadActual.nombre;
    if (nombre == "Simio Veloz")     return ":/nivel1/SimioVelozUsuario.png";
    if (nombre == "Simio de Acero")  return ":/nivel1/SimioDeAceroUsuario.png";
    if (nombre == "Simio Golpeador") return ":/nivel1/SimioGolpeadorUsuario.png";
    return ":/nivel1/SimioGolpeadorUsuario.png";
}
