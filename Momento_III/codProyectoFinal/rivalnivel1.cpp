#include "rivalnivel1.h"
#include "jugadornivel1.h"
#include "personalidad.h"

// ---- AgenteInteligente stubs ----

void AgenteInteligente::decidir(float posJugador, float posRival, int carrilJugador, int carrilRival,
                                Dificultad dificultad) {}

bool AgenteInteligente::debeAcelerar() const { return false; }
bool AgenteInteligente::debeAtacar() const { return false; }
bool AgenteInteligente::debeCambiarCarril() const { return false; }
int  AgenteInteligente::carrilObjetivo() const { return 0; }

void AgenteInteligente::registrarFallo() {}
void AgenteInteligente::registrarAcierto() {}
void AgenteInteligente::evaluarAprendizaje(bool enRangoAhora) {}
float AgenteInteligente::getRangoAtaque() const { return rangoAtaque_; }

// ---- RivalNivel1 ----

RivalNivel1::RivalNivel1(QGraphicsItem *parent)
    : CorredorNivel1(parent),
      cooldownAtaque(0.0f),
      tiempoCooldown(0.0f),
      tiempoCambioCarril(0.0f),
      personalidad(nullptr),
      numCarriles(3),
      dificultad(Dificultad::FACIL)
{
    setColorBase(Qt::red);
    aceleracion  = 30.0f;
    velocidadMax = 120.0f;
}

void RivalNivel1::acelerar() {
    velocidadY += aceleracion;
    if (velocidadY > velocidadMax) velocidadY = velocidadMax;
}

void RivalNivel1::frenar() {
    velocidadY -= aceleracion * 0.3f;
    if (velocidadY < 0) velocidadY = 0;
}

bool RivalNivel1::tomarDecision(JugadorNivel1 *jugador, float dt) { return false; }
void RivalNivel1::atacarJugador(JugadorNivel1 *jugador) {}
void RivalNivel1::actualizarIA(float dt) {}
void RivalNivel1::setPersonalidad(Personalidad *p) { personalidad = p; }
void RivalNivel1::aplicarPersonalidad() {}
void RivalNivel1::setNumCarriles(int n) { numCarriles = n; }
void RivalNivel1::setDificultad(Dificultad d) { dificultad = d; }
