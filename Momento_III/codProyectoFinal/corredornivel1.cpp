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

void CorredorNivel1::actualizarPosicion(float dt) {}

void CorredorNivel1::actualizarAnimaciones(float dt) {}

void CorredorNivel1::setColorBase(const QColor &color) {}

bool CorredorNivel1::estaDesacelerado() const { return false; }

int CorredorNivel1::getCarrilActual() const { return 0; }

void CorredorNivel1::cambiarCarril(int nuevoCarril) {}

void CorredorNivel1::recibirGolpe(float fuerza) {}

void CorredorNivel1::aplicarImpulso(float impulso) {}

void CorredorNivel1::activarDesaceleracion(float duracion) {}

void CorredorNivel1::activarGolpeRecibido(float duracion) {}

float CorredorNivel1::getAceleracion() const { return 0.0f; }
float CorredorNivel1::getVelocidadMax() const { return 0.0f; }
float CorredorNivel1::getFuerzaGolpe() const { return 0.0f; }
float CorredorNivel1::getResistenciaGolpe() const { return 0.0f; }
