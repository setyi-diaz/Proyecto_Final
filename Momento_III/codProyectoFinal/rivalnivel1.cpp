#include "rivalnivel1.h"
#include "jugadornivel1.h"
#include "nivel1.h"
#include <QtMath>
#include <QRandomGenerator>

void RivalNivel1::decidir(float posJugador, float posRival, int carrilJugador,
                          int carrilRival, DificultadNivel1 dificultad) {
    acelerar_      = true;
    cambiarCarril_ = false;
    carrilObj_     = carrilRival;
    float distancia = qAbs(posJugador - posRival);
    if (dificultad == DificultadNivel1::DIFICIL && carrilJugador != carrilRival) {
        cambiarCarril_ = true;
        carrilObj_     = carrilJugador;
    }
    atacar_ = (carrilJugador == carrilRival) && (distancia < rangoAtaque_);
}

void RivalNivel1::registrarFallo() {
    if (++fallosConsecutivos_ >= 2) {
        rangoAtaque_ = qMin(rangoAtaque_ + 10.0f, 150.0f);
        fallosConsecutivos_ = 0;
    }
}

void RivalNivel1::registrarAcierto() {
    fallosConsecutivos_ = 0;
    rangoAtaque_ = qMax(rangoAtaque_ - 5.0f, 60.0f);
}

void RivalNivel1::evaluarAprendizaje(bool enRangoAhora) {
    if (enRangoAhora && !enRangoAnterior_) registrarFallo();
    enRangoAnterior_ = enRangoAhora;
}

float RivalNivel1::getRangoAtaque()    const { return rangoAtaque_; }
bool  RivalNivel1::debeAcelerar()      const { return acelerar_; }
bool  RivalNivel1::debeAtacar()        const { return atacar_; }
bool  RivalNivel1::debeCambiarCarril() const { return cambiarCarril_; }
int   RivalNivel1::carrilObjetivo()    const { return carrilObj_; }

RivalNivel1::RivalNivel1()
    : CorredorNivel1(),
      cooldownAtaque(3.0f),
      tiempoCooldown(0),
      tiempoCambioCarril(0),
      numCarriles(3),
      dificultad(DificultadNivel1::NORMAL)
{
    setColorBase(Qt::red);
    aceleracion  = 30.0f;
    velocidadMax = 120.0f;
}

void RivalNivel1::acelerar(float dt) {
    velocidadAvance += aceleracion * dt;
    if (velocidadAvance > velocidadMax) velocidadAvance = velocidadMax;
}

void RivalNivel1::frenar(float dt) {
    velocidadAvance -= aceleracion * 0.3f * dt;
    if (velocidadAvance < 0) velocidadAvance = 0;
}

bool RivalNivel1::tomarDecision(JugadorNivel1 *jugador, float dt) {
    decidir(jugador->getPosY(), posY,
                   jugador->getCarrilActual(), carrilActual, dificultad);

    float intervalo = (dificultad == DificultadNivel1::DIFICIL) ? 1.2f : 2.5f;
    tiempoCambioCarril += dt;
    if (tiempoCambioCarril > intervalo) {
        tiempoCambioCarril = 0;
        if (debeCambiarCarril())
            cambiarCarril(carrilObjetivo());
        else
            cambiarCarril(QRandomGenerator::global()->bounded(numCarriles));
    }

    if (debeAtacar() && tiempoCooldown <= 0) {
        atacarJugador(jugador);
        registrarAcierto();
        tiempoCooldown = cooldownAtaque;
        return true;
    }

    bool enRango = (qAbs(jugador->getPosY() - posY) < getRangoAtaque() &&
                    jugador->getCarrilActual() == carrilActual &&
                    tiempoCooldown <= 0);
    evaluarAprendizaje(enRango);
    return false;
}

void RivalNivel1::atacarJugador(JugadorNivel1 *jugador) {
    jugador->recibirGolpe(fuerzaGolpe);
}

void RivalNivel1::actualizarIA(float dt) {
    if (tiempoCooldown > 0) tiempoCooldown -= dt;
}

void RivalNivel1::setNumCarriles(int n) { numCarriles = n; }

void RivalNivel1::setDificultad(DificultadNivel1 d) {
    dificultad     = d;
    cooldownAtaque = (d == DificultadNivel1::DIFICIL) ? 1.5f : 3.0f;
}

QString RivalNivel1::rutaSprite() const {
    if (!hayPersonalidad) return ":/nivel1/SimioGolpeador.png";
    const QString &nombre = personalidadActual.nombre;
    if (nombre == "Simio Veloz")     return ":/nivel1/SimioVeloz.png";
    if (nombre == "Simio de Acero")  return ":/nivel1/SimioDeAcero.png";
    if (nombre == "Simio Golpeador") return ":/nivel1/SimioGolpeador.png";
    return ":/nivel1/SimioGolpeador.png";
}
