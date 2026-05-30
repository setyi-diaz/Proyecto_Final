#include "rivalnivel1.h"
#include "jugadornivel1.h"
#include "personalidad.h"
#include <QtMath>
#include <QBrush>
#include <QRandomGenerator>

void AgenteInteligente::decidir(float posJugador, float posRival, int carrilJugador, int carrilRival) {}

bool AgenteInteligente::debeAcelerar() const { return false; }
bool AgenteInteligente::debeAtacar() const { return false; }
bool AgenteInteligente::debeCambiarCarril() const { return false; }
int AgenteInteligente::carrilObjetivo() const { return 0; }

RivalNivel1::RivalNivel1(QGraphicsItem *parent)
    : CorredorNivel1(parent),
      cooldownAtaque(3.0f),
      tiempoCooldown(0),
      tiempoCambioCarril(0),
      personalidad(nullptr),
      numCarriles(3)
{}

void RivalNivel1::acelerar() {}

void RivalNivel1::frenar() {}

bool RivalNivel1::tomarDecision(JugadorNivel1 *jugador) { return false; }

void RivalNivel1::atacarJugador(JugadorNivel1 *jugador) {}

void RivalNivel1::actualizarIA() {}

void RivalNivel1::setPersonalidad(Personalidad *p) {}

void RivalNivel1::setNumCarriles(int n) {}

void RivalNivel1::aplicarPersonalidad() {}
