#include "nivel1.h"
#include "jugadornivel1.h"
#include "rivalnivel1.h"
#include "pista.h"
#include "objetopista.h"
#include "personalidad.h"
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QtMath>

static const float MARGEN_X     = 140.0f;
static const float ANCHO_CARRIL = 100.0f;
static const float META          = 5000.0f;

Nivel1::Nivel1(int numCarriles, int numRivales, QObject *parent)
    : QGraphicsScene(parent),
      numCarriles(numCarriles),
      numRivalesConfig(numRivales),
      anchoCarril(ANCHO_CARRIL),
      margenX(MARGEN_X),
      jugador(nullptr),
      pista(nullptr),
      textoControles(nullptr),
      textoInfo(nullptr),
      textoCuenta(nullptr),
      textoTiempo(nullptr),
      personalidadJugador(nullptr),
      progresoJugador(0.0f),
      velocidadDrift(60.0f),
      velBaseJugador(60.0f),
      impulsoJugador(0.0f),
      tiempoImpulsoJugador(0.0f),
      frenadaJugador(0.0f),
      tiempoFrenadaJugador(0.0f),
      tiempoTotal(0.0f),
      tiempoRestante(0.0f),
      timerJuego(nullptr),
      timerRegresiva(nullptr),
      cuentaRegresiva(3),
      juegoActivo(false),
      dt(0.016f),
      tiempoObstaculo(0.0f),
      tiempoBanana(0.0f),
      intervaloObstaculos(3.0f),
      cooldownGolpe(0.0f),
      velocidadObstaculos(60.0f),
      dificultad(Dificultad::FACIL),
      btnReiniciar(nullptr),
      btnMenu(nullptr),
      carreraTerminada(false)
{
    setSceneRect(0, 0, 680, 850);
    setBackgroundBrush(QBrush(QColor(20, 20, 25)));

    // Track
    pista = new Pista(numCarriles, 300, anchoCarril);
    pista->setPos(margenX, 0);
    addItem(pista);

    // Lane dividers
    for (int i = 1; i < numCarriles; i++)
        addLine(margenX + i * anchoCarril, 0,
                margenX + i * anchoCarril, 850,
                QPen(QColor(150, 150, 150), 1, Qt::DashLine));
    addLine(margenX, 0, margenX, 850, QPen(Qt::white, 2));
    addLine(margenX + numCarriles * anchoCarril, 0,
            margenX + numCarriles * anchoCarril, 850, QPen(Qt::white, 2));

    // Player
    jugador = new JugadorNivel1();
    jugador->setPos(margenX + 1 * anchoCarril + 25, 700);
    jugador->cambiarCarril(1);
    addItem(jugador);

    // Progress bar background
    QGraphicsRectItem *barraFondo = new QGraphicsRectItem(
        margenX + numCarriles * anchoCarril + 10, 50, 20, 700);
    barraFondo->setBrush(QBrush(QColor(50, 50, 50)));
    barraFondo->setPen(QPen(Qt::white, 1));
    addItem(barraFondo);

    // Player progress bar
    QGraphicsRectItem *barraJugador = new QGraphicsRectItem(
        margenX + numCarriles * anchoCarril + 10, 745, 6, 5);
    barraJugador->setBrush(QBrush(Qt::cyan));
    barraJugador->setPen(Qt::NoPen);
    addItem(barraJugador);

    // Countdown text
    textoCuenta = addText("", QFont("Arial", 48, QFont::Bold));
    textoCuenta->setDefaultTextColor(Qt::yellow);
    textoCuenta->setPos(300, 380);

    // Controls hint
    textoControles = addText("W: Acelerar  S: Frenar\nA/D: Cambiar carril",
                             QFont("Consolas", 10));
    textoControles->setDefaultTextColor(QColor(180, 180, 180));
    textoControles->setPos(margenX, 10);

    // Create 2 fixed rivals: red at lane 0, orange at lane 2
    crearRival(0, nullptr, Qt::red);
    crearRival(2, nullptr, QColor(255, 140, 0));

    // Obstaculos estaticos en posiciones fijas
    float posicionesObs[][2] = {{0, 200}, {1, 400}, {2, 300}, {0, 600}, {1, 150}, {2, 500}};
    for (auto &pos : posicionesObs) {
        ObjetoPista *obs = new ObjetoPista(-0.7f, pos[1], (int)pos[0], margenX, anchoCarril);
        obstaculos.append(obs);
        addItem(obs);
    }

    // Timers
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel1::actualizar);

    timerRegresiva = new QTimer(this);
    connect(timerRegresiva, &QTimer::timeout, this, &Nivel1::contarRegresiva);
}

Nivel1::~Nivel1() {}

// ---- crearRival ----

void Nivel1::crearRival(int carril, Personalidad *personalidad, const QColor &color) {
    EstadoRival er;
    er.rival = new RivalNivel1();
    er.rival->setColorBase(color);
    er.rival->setPos(margenX + carril * anchoCarril + 25, 700);
    er.rival->cambiarCarril(carril);
    er.color = color;
    addItem(er.rival);
    rivales.append(er);
}

// ---- iniciar ----

void Nivel1::iniciar() {
    cuentaRegresiva = 3;
    juegoActivo     = false;
    textoCuenta->setPlainText(QString::number(cuentaRegresiva));
    timerRegresiva->start(1000);
}

// ---- contarRegresiva ----

void Nivel1::contarRegresiva() {
    cuentaRegresiva--;
    if (cuentaRegresiva > 0) {
        textoCuenta->setPlainText(QString::number(cuentaRegresiva));
    } else if (cuentaRegresiva == 0) {
        textoCuenta->setPlainText("GO!");
    } else {
        textoCuenta->setPlainText("");
        timerRegresiva->stop();
        juegoActivo = true;
        timerJuego->start(16);
        textoControles->setVisible(false);
    }
}

// ---- aumentarVelocidad / disminuirVelocidad ----

void Nivel1::aumentarVelocidad() {
    velocidadDrift += 8.0f;
    if (velocidadDrift > 150.0f) velocidadDrift = 150.0f;
}

void Nivel1::disminuirVelocidad() {
    velocidadDrift -= 8.0f;
    if (velocidadDrift < 20.0f) velocidadDrift = 20.0f;
}

// ---- actualizar (game loop) ----

void Nivel1::actualizar() {
    if (!juegoActivo) return;

    progresoJugador += velocidadDrift * dt;

    for (auto &er : rivales)
        er.progreso += er.velBase * dt;

    // Update Y positions from progress
    float yJugador = 720.0f - (progresoJugador / META) * 620.0f;
    yJugador = qBound(80.0f, yJugador, 720.0f);
    jugador->setPos(margenX + jugador->getCarrilActual() * anchoCarril + 25, yJugador);

    for (auto &er : rivales) {
        float yR = 720.0f - (er.progreso / META) * 620.0f;
        yR = qBound(80.0f, yR, 720.0f);
        er.rival->setPos(margenX + er.rival->getCarrilActual() * anchoCarril + 25, yR);
    }

    // Mover obstaculos hacia abajo simulando scroll
    for (ObjetoPista *obs : obstaculos) {
        float nuevaY = obs->y() + velocidadDrift * dt;
        if (nuevaY > 880) nuevaY = -30;
        obs->setPos(obs->x(), nuevaY);
    }

    // Colision jugador con obstaculos
    float xJug = margenX + jugador->getCarrilActual() * anchoCarril;
    float yJug = jugador->y();
    for (ObjetoPista *obs : obstaculos) {
        if (obs->x() >= xJug && obs->x() <= xJug + anchoCarril &&
            qAbs(obs->y() - yJug) < 40) {
            velocidadDrift -= 15.0f;
            if (velocidadDrift < 20.0f) velocidadDrift = 20.0f;
            obs->setPos(obs->x(), -30);  // recicla el obstaculo
        }
    }

    actualizarBarraProgreso();
    verificarVictoria();
}

// ---- actualizarBarraProgreso ----

void Nivel1::actualizarBarraProgreso() {
    // barras are owned as anonymous items; nothing to update in stage 2
}

// ---- verificarVictoria ----

void Nivel1::verificarVictoria() {
    if (progresoJugador >= META) {
        juegoActivo = false;
        timerJuego->stop();
        textoCuenta->setPlainText("GANASTE!");
        textoCuenta->setDefaultTextColor(Qt::green);
        textoCuenta->setFont(QFont("Arial", 32, QFont::Bold));
        return;
    }
    for (const auto &er : rivales) {
        if (er.progreso >= META) {
            juegoActivo = false;
            timerJuego->stop();
            textoCuenta->setPlainText("PERDISTE");
            textoCuenta->setDefaultTextColor(Qt::red);
            textoCuenta->setFont(QFont("Arial", 32, QFont::Bold));
            return;
        }
    }
}

// ---- simple getters ----

JugadorNivel1* Nivel1::getJugador() const { return jugador; }
int Nivel1::getNumCarriles() const { return numCarriles; }

// ---- stubs ----

void Nivel1::setDificultad(Dificultad d) { dificultad = d; }

void Nivel1::setPersonalidades(Personalidad *personalidadJugador,
                               QList<Personalidad*> personalidadesRivales) {}

bool Nivel1::carrilLibre(int carril) const { return true; }

void Nivel1::golpearRival(int direccion) {}

void Nivel1::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
}

void Nivel1::iniciarCuentaRegresiva() {}

void Nivel1::generarObstaculos() {}

void Nivel1::generarEfecto(float efecto) {}

void Nivel1::actualizarColisiones() {}

void Nivel1::actualizarTextosFlotantes() {}

void Nivel1::spawnTextoFlotante(const QString &texto, float x, float y, const QColor &color) {}

void Nivel1::finalizarCarrera(const QString &mensaje, const QColor &color) {}
