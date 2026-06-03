#include "corredornivel1.h"
#include <QBrush>
#include <QColor>

CorredorNivel1::CorredorNivel1(QGraphicsItem *parent)
    : Personaje(parent),
      carrilActual(1),
      resistenciaGolpe(1.0f),
      fuerzaGolpe(50.0f),
      tiempoDesaceleracion(0),
      tiempoGolpeRecibido(0)
{}

CorredorNivel1::~CorredorNivel1() {}

void CorredorNivel1::actualizarPosicion(float dt) {
    Personaje::actualizarPosicion(dt);
}

void CorredorNivel1::actualizarAnimaciones(float dt) {
    if (tiempoGolpeRecibido > 0) {
        tiempoGolpeRecibido -= dt;
        setBrush(QBrush((int)(tiempoGolpeRecibido * 15) % 2 == 0
                        ? QColor(255, 200, 0) : colorBase));
        if (tiempoGolpeRecibido <= 0)
            setBrush(QBrush(colorBase));
        return;
    }
    if (tiempoDesaceleracion > 0) {
        tiempoDesaceleracion -= dt;
        setBrush(QBrush((int)(tiempoDesaceleracion * 10) % 2 == 0
                        ? QColor(255, 100, 100) : colorBase));
        if (tiempoDesaceleracion <= 0)
            setBrush(QBrush(colorBase));
    }
}

int CorredorNivel1::getCarrilActual() const { return carrilActual; }

void CorredorNivel1::cambiarCarril(int nuevoCarril) {
    carrilActual = nuevoCarril;
}

void CorredorNivel1::recibirGolpe(float fuerza) {
    velocidadY -= fuerza / resistenciaGolpe;
    if (velocidadY < 0) velocidadY = 0;
}

void CorredorNivel1::aplicarImpulso(float impulso) {
    velocidadY += impulso;
    if (velocidadY > velocidadMax) velocidadY = velocidadMax;
}

bool CorredorNivel1::estaDesacelerado() const {
    return tiempoDesaceleracion > 0;
}

void CorredorNivel1::activarDesaceleracion(float duracion) {
    tiempoDesaceleracion = duracion;
}

void CorredorNivel1::activarGolpeRecibido(float duracion) {
    tiempoGolpeRecibido = duracion;
}

void CorredorNivel1::setColorBase(const QColor &color) {
    colorBase = color;
    setBrush(QBrush(color));
}

float CorredorNivel1::getAceleracion() const { return aceleracion; }
float CorredorNivel1::getVelocidadMax() const { return velocidadMax; }
float CorredorNivel1::getFuerzaGolpe() const { return fuerzaGolpe; }
float CorredorNivel1::getResistenciaGolpe() const { return resistenciaGolpe; }
