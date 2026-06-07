#include "corredornivel1.h"
#include <QtMath>

CorredorNivel1::CorredorNivel1()
    : Personaje()
{}

CorredorNivel1::~CorredorNivel1() {}

void CorredorNivel1::setPersonalidad(const Personalidad &p) {
    personalidadActual = p;
    hayPersonalidad    = true;
}

bool CorredorNivel1::tienePersonalidad() const { return hayPersonalidad; }

CorredorNivel1::Personalidad CorredorNivel1::getPersonalidad() const {
    return personalidadActual;
}

void CorredorNivel1::aplicarPersonalidad() {
    if (!hayPersonalidad) return;
    aceleracion      *= personalidadActual.modAceleracion;
    velocidadMax     *= personalidadActual.modVelMax;
    resistenciaGolpe  = personalidadActual.resistenciaGolpe;
    fuerzaGolpe       = personalidadActual.fuerzaGolpe;
}

QString CorredorNivel1::getRutaSprite() const { return rutaSprite(); }

void CorredorNivel1::actualizarAnimaciones(float dt) {
    if (tiempoGolpeRecibido > 0) {
        tiempoGolpeRecibido -= dt;
        frameActual = (int)(tiempoGolpeRecibido * 10) % 2 == 0 ? spriteGolpe : 0;
        if (tiempoGolpeRecibido <= 0) frameActual = 0;
        return;
    }
    if (tiempoDesaceleracion > 0) {
        tiempoDesaceleracion -= dt;
        frameActual = 0;
        return;
    }
    if (tiempoRotacion > 0) {
        tiempoRotacion -= dt;
        float progreso = 1.0f - (tiempoRotacion / duracionRotacion);
        if (progreso < 0.5f)
            anguloRotacion = progreso * 2.0f * 180.0f;
        else
            anguloRotacion = (1.0f - progreso) * 2.0f * 180.0f;
        if (tiempoRotacion <= 0) anguloRotacion = 0;
        frameActual = 0;
        return;
    }
    if (qAbs(omegaAgua) > 1.0f) {
        anguloAcumAgua += omegaAgua * dt;
        omegaAgua      *= qExp(-kAmortAgua * dt);
        frameActual = 0;
        return;
    }
    if (anguloAcumAgua != 0) {
        anguloAcumAgua = 0;
        omegaAgua      = 0;
    }
    frameActual = 0;
}

float CorredorNivel1::getRotacionActual() const {
    if (qAbs(omegaAgua) > 1.0f || anguloAcumAgua != 0) return anguloAcumAgua;
    return anguloRotacion;
}

int    CorredorNivel1::getSpriteIndex()      const { return frameActual; }
QColor CorredorNivel1::getColorBase()        const { return colorBase; }
void   CorredorNivel1::setColorBase(const QColor &c) { colorBase = c; }

bool  CorredorNivel1::estaRotandoAgua()      const { return qAbs(omegaAgua) > 1.0f; }
bool  CorredorNivel1::estaDesacelerado()     const { return tiempoDesaceleracion > 0; }
int   CorredorNivel1::getCarrilActual()      const { return carrilActual; }
void  CorredorNivel1::cambiarCarril(int n)          { carrilActual = n; }

void CorredorNivel1::recibirGolpe(float fuerza) {
    velocidadAvance -= fuerza / resistenciaGolpe;
    if (velocidadAvance < 0) velocidadAvance = 0;
}

void CorredorNivel1::aplicarImpulso(float impulso) {
    velocidadAvance += impulso;
    if (velocidadAvance > velocidadMax) velocidadAvance = velocidadMax;
}

void CorredorNivel1::activarDesaceleracion(float d)  { tiempoDesaceleracion = d; }
void CorredorNivel1::activarGolpeRecibido(float d)   { tiempoGolpeRecibido  = d; }

void CorredorNivel1::activarGolpeAnimacion(float duracion, int indiceSpriteGolpe) {
    tiempoGolpeRecibido = duracion;
    spriteGolpe         = indiceSpriteGolpe;
    frameActual         = indiceSpriteGolpe;
}

void CorredorNivel1::activarRotacion(float duracion) {
    tiempoRotacion   = duracion;
    duracionRotacion = duracion;
    anguloRotacion   = 0;
}

void CorredorNivel1::activarRotacionAgua(float vel, float amort) {
    omegaAgua      = vel;
    kAmortAgua     = amort;
    anguloAcumAgua = 0;
}

float CorredorNivel1::getFuerzaGolpe()      const { return fuerzaGolpe; }
float CorredorNivel1::getResistenciaGolpe() const { return resistenciaGolpe; }
float CorredorNivel1::getVelocidadAvance()  const { return velocidadAvance; }
