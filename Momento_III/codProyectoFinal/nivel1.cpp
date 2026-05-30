#include "nivel1.h"
#include "jugadornivel1.h"
#include "rivalnivel1.h"
#include "pista.h"
#include "personalidad.h"
#include "obstaculo.h"
#include "bananaimpulso.h"
#include <QGraphicsView>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QRandomGenerator>
#include <QtMath>

Nivel1::Nivel1(int numCarriles_, int numRivales_, QObject *parent)
    : QGraphicsScene(parent),
      numCarriles(numCarriles_),
      numRivalesConfig(numRivales_),
      anchoCarril(0),
      margenX(0),
      jugador(nullptr),
      pista(nullptr),
      textoControles(nullptr),
      textoInfo(nullptr),
      textoCuenta(nullptr),
      textoTiempo(nullptr),
      personalidadJugador(nullptr),
      progresoJugador(0),
      velocidadDrift(0),
      velBaseJugador(0),
      scrollOffset(0),
      impulsoJugador(0),
      tiempoImpulsoJugador(0),
      frenadaJugador(0),
      tiempoFrenadaJugador(0),
      tiempoTotal(180.0f),
      tiempoRestante(180.0f),
      cuentaRegresiva(3),
      juegoActivo(false),
      dt(0.016f),
      tiempoObstaculo(0),
      tiempoBanana(0),
      intervaloObstaculos(0),
      cooldownGolpe(0),
      btnReiniciar(nullptr),
      btnMenu(nullptr),
      carreraTerminada(false)
{}

Nivel1::~Nivel1() {}

int Nivel1::getNumCarriles() const { return 0; }

void Nivel1::crearRival(int carril, Personalidad *personalidad, const QColor &color) {}

void Nivel1::setPersonalidades(Personalidad *pJugador, QList<Personalidad*> pRivales) {}

void Nivel1::iniciar() {}

void Nivel1::iniciarCuentaRegresiva() {}

void Nivel1::contarRegresiva() {}

void Nivel1::aumentarVelocidad() {}

void Nivel1::disminuirVelocidad() {}

void Nivel1::golpearRival(int direccion) {}

void Nivel1::actualizar() {}

void Nivel1::generarObstaculos() {}

void Nivel1::generarBanana() {}

void Nivel1::actualizarColisiones() {}

void Nivel1::verificarVictoria() {}

void Nivel1::finalizarCarrera(const QString &mensaje, const QColor &color) {}

void Nivel1::mousePressEvent(QGraphicsSceneMouseEvent *event) {}

JugadorNivel1* Nivel1::getJugador() const { return nullptr; }

bool Nivel1::carrilLibre(int carril) const { return false; }

void Nivel1::spawnTextoFlotante(const QString &texto, float x, float y, const QColor &color) {}

void Nivel1::actualizarTextosFlotantes() {}
