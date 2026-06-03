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
#include <QRandomGenerator>

static const float META = 5000.0f;

Nivel1::Nivel1(int numCarriles, int numRivales, QObject *parent)
    : QGraphicsScene(parent),
      numCarriles(numCarriles),
      numRivalesConfig(numRivales),
      anchoCarril(100.0f),
      margenX(140.0f),
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
      tiempoTotal(120.0f),
      tiempoRestante(120.0f),
      timerJuego(nullptr),
      timerRegresiva(nullptr),
      cuentaRegresiva(3),
      juegoActivo(false),
      dt(0.016f),
      tiempoObstaculo(0.0f),
      tiempoBanana(0.0f),
      intervaloObstaculos(2.5f),
      cooldownGolpe(0.0f),
      velocidadObstaculos(80.0f),
      dificultad(Dificultad::FACIL),
      btnReiniciar(nullptr),
      btnMenu(nullptr),
      carreraTerminada(false)
{
    setSceneRect(0, 0, 680, 850);
    setBackgroundBrush(QBrush(QColor(20, 20, 25)));

    pista = new Pista(numCarriles, 300, anchoCarril);
    pista->setPos(margenX, 0);
    addItem(pista);

    for (int i = 1; i < numCarriles; i++)
        addLine(margenX + i * anchoCarril, 0,
                margenX + i * anchoCarril, 850,
                QPen(QColor(150, 150, 150), 1, Qt::DashLine));
    addLine(margenX, 0, margenX, 850, QPen(Qt::white, 2));
    addLine(margenX + numCarriles * anchoCarril, 0,
            margenX + numCarriles * anchoCarril, 850, QPen(Qt::white, 2));

    jugador = new JugadorNivel1();
    jugador->setPos(margenX + 1 * anchoCarril + 25, 700);
    jugador->cambiarCarril(1);
    addItem(jugador);

    textoInfo = addText("", QFont("Consolas", 9));
    textoInfo->setDefaultTextColor(Qt::white);
    textoInfo->setPos(margenX, 810);

    textoTiempo = addText("2:00", QFont("Arial", 14, QFont::Bold));
    textoTiempo->setDefaultTextColor(QColor(255, 220, 50));
    textoTiempo->setPos(margenX + numCarriles * anchoCarril + 30, 10);

    textoCuenta = addText("", QFont("Arial", 48, QFont::Bold));
    textoCuenta->setDefaultTextColor(Qt::yellow);
    textoCuenta->setPos(300, 380);

    textoControles = addText("W: Acelerar  S: Frenar\nA/D: Cambiar carril\nJ/K: Golpear rival",
                             QFont("Consolas", 10));
    textoControles->setDefaultTextColor(QColor(180, 180, 180));
    textoControles->setPos(margenX, 10);

    crearRival(0, nullptr, Qt::red);
    crearRival(2, nullptr, QColor(255, 140, 0));

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel1::actualizar);

    timerRegresiva = new QTimer(this);
    connect(timerRegresiva, &QTimer::timeout, this, &Nivel1::contarRegresiva);
}

Nivel1::~Nivel1() {
    for (ObjetoPista *obs : obstaculos) { removeItem(obs); delete obs; }
    obstaculos.clear();
    for (ObjetoPista *ef : efectos) { removeItem(ef); delete ef; }
    efectos.clear();
}

void Nivel1::crearRival(int carril, Personalidad *personalidad, const QColor &color) {
    EstadoRival er;
    er.rival = new RivalNivel1();
    er.rival->setColorBase(color);
    er.rival->setPos(margenX + carril * anchoCarril + 25, 700);
    er.rival->cambiarCarril(carril);
    er.color = color;
    er.velBase = 50.0f + QRandomGenerator::global()->bounded(20);
    addItem(er.rival);
    rivales.append(er);
}

void Nivel1::iniciar() {
    cuentaRegresiva = 3;
    juegoActivo = false;
    textoCuenta->setPlainText(QString::number(cuentaRegresiva));
    timerRegresiva->start(1000);
}

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
        reloj.start();
        timerJuego->start(16);
        textoControles->setVisible(false);
    }
}

void Nivel1::aumentarVelocidad() {
    velocidadDrift += 8.0f;
    if (velocidadDrift > 150.0f) velocidadDrift = 150.0f;
    velBaseJugador = velocidadDrift;
}

void Nivel1::disminuirVelocidad() {
    velocidadDrift -= 8.0f;
    if (velocidadDrift < 20.0f) velocidadDrift = 20.0f;
    velBaseJugador = velocidadDrift;
}

void Nivel1::golpearRival(int direccion) {
    if (cooldownGolpe > 0) return;

    int carrilJ = jugador->getCarrilActual();
    int carrilObjetivo = carrilJ + direccion;

    bool golpeo = false;
    for (auto &er : rivales) {
        float diffY = qAbs(jugador->y() - er.rival->y());
        if (er.rival->getCarrilActual() == carrilObjetivo && diffY < 80) {
            float penalizacion = 30.0f * jugador->getFuerzaGolpe() / er.rival->getResistenciaGolpe();
            er.progreso -= penalizacion;
            if (er.progreso < 0) er.progreso = 0;
            er.rival->activarGolpeRecibido(0.6f);
            spawnTextoFlotante("GOLPE!", er.rival->x(), er.rival->y() - 20, QColor(255, 220, 0));
            golpeo = true;
        }
    }

    cooldownGolpe = 3.0f;
    if (!golpeo)
        spawnTextoFlotante("FALLO", jugador->x(), jugador->y() - 20, QColor(180, 180, 180));
}

void Nivel1::actualizar() {
    if (!juegoActivo) return;

    dt = qMin(reloj.elapsed() / 1000.0f, 0.05f);
    reloj.restart();

    tiempoRestante -= dt;
    if (tiempoRestante <= 0) tiempoRestante = 0;

    int mins = (int)tiempoRestante / 60;
    int secs = (int)tiempoRestante % 60;
    textoTiempo->setPlainText(QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0')));
    if (tiempoRestante < 30.0f)
        textoTiempo->setDefaultTextColor(QColor(255, 80, 80));

    // Impulso y frenada con degradado temporal
    float deltaImpulso = 0.0f;
    float deltaFrenada = 0.0f;

    if (tiempoImpulsoJugador > 0) {
        tiempoImpulsoJugador -= dt;
        if (tiempoImpulsoJugador > 0)
            deltaImpulso = impulsoJugador * (tiempoImpulsoJugador / 2.5f);
        else
            impulsoJugador = 0;
    }
    if (tiempoFrenadaJugador > 0) {
        tiempoFrenadaJugador -= dt;
        if (tiempoFrenadaJugador > 0)
            deltaFrenada = frenadaJugador * (tiempoFrenadaJugador / 1.5f);
        else
            frenadaJugador = 0;
    }

    float velEfectiva = velBaseJugador + deltaImpulso - deltaFrenada;
    if (velEfectiva < 10.0f) velEfectiva = 10.0f;

    progresoJugador += velEfectiva * dt;

    for (auto &er : rivales) {
        float vel = er.velBase;
        if (er.tiempoImpulsoVel > 0) {
            er.tiempoImpulsoVel -= dt;
            vel += er.impulsoVel * (er.tiempoImpulsoVel / 2.0f);
            if (er.tiempoImpulsoVel <= 0) er.impulsoVel = 0;
        }
        er.progreso += vel * dt;
    }

    if (cooldownGolpe > 0) cooldownGolpe -= dt;

    // Posicion Y del jugador
    float yJugador = 720.0f - (progresoJugador / META) * 620.0f;
    yJugador = qBound(80.0f, yJugador, 720.0f);
    jugador->setPos(margenX + jugador->getCarrilActual() * anchoCarril + 25, yJugador);

    for (auto &er : rivales) {
        float yR = 720.0f - (er.progreso / META) * 620.0f;
        yR = qBound(80.0f, yR, 720.0f);
        er.rival->setPos(margenX + er.rival->getCarrilActual() * anchoCarril + 25, yR);
        er.rival->actualizarAnimaciones(dt);
    }

    jugador->actualizarAnimaciones(dt);

    // Mover obstaculos
    for (ObjetoPista *obs : obstaculos) {
        obs->actualizarCooldown(dt);
        float nuevaY = obs->y() + velocidadObstaculos * dt;
        if (nuevaY > 880) nuevaY = -30;
        obs->setPos(obs->x(), nuevaY);
    }

    // Mover bananas/efectos
    for (int i = efectos.size() - 1; i >= 0; i--) {
        ObjetoPista *ef = efectos[i];
        float nuevaY = ef->y() + velocidadObstaculos * dt;
        ef->setPos(ef->x(), nuevaY);
        if (nuevaY > 880) {
            removeItem(ef);
            efectos.removeAt(i);
            delete ef;
        }
    }

    // Generar obstaculos
    tiempoObstaculo += dt;
    if (tiempoObstaculo > intervaloObstaculos) {
        tiempoObstaculo = 0;
        generarObstaculos();
    }

    // Generar bananas
    tiempoBanana += dt;
    if (tiempoBanana > 4.0f) {
        tiempoBanana = 0;
        generarEfecto(0.5f);
    }

    actualizarColisiones();
    actualizarTextosFlotantes();
    verificarVictoria();

    QString info = QString("Vel: %1 | Progreso: %2")
                    .arg(QString::number(velEfectiva, 'f', 1))
                    .arg(QString::number(progresoJugador, 'f', 0));
    if (cooldownGolpe > 0)
        info += QString(" | Golpe: %1s").arg(QString::number(cooldownGolpe, 'f', 1));
    textoInfo->setPlainText(info);
}

void Nivel1::generarObstaculos() {
    int carril = QRandomGenerator::global()->bounded(numCarriles);
    ObjetoPista *obs = new ObjetoPista(-0.7f, -30, carril, margenX, anchoCarril);
    obstaculos.append(obs);
    addItem(obs);
}

void Nivel1::generarEfecto(float efecto) {
    int carril = QRandomGenerator::global()->bounded(numCarriles);
    ObjetoPista *ef = new ObjetoPista(efecto, -30, carril, margenX, anchoCarril);
    efectos.append(ef);
    addItem(ef);
}

void Nivel1::actualizarColisiones() {
    int carrilJ = jugador->getCarrilActual();
    float xJug = margenX + carrilJ * anchoCarril;
    float yJug = jugador->y();

    // Obstaculos: frenan al jugador
    for (ObjetoPista *obs : obstaculos) {
        if (!obs->puedeColisionar()) continue;
        float xObs = obs->x();
        float yObs = obs->y();
        if (xObs >= xJug && xObs <= xJug + anchoCarril && qAbs(yObs - yJug) < 40) {
            frenadaJugador = velBaseJugador * 0.7f;
            tiempoFrenadaJugador = 1.5f;
            jugador->activarDesaceleracion(0.5f);
            obs->activarCooldown(1.5f);
            spawnTextoFlotante("CHOQUE!", jugador->x(), jugador->y() - 20, QColor(255, 120, 0));
        }

        // Colision con rivales
        for (auto &er : rivales) {
            float xR = margenX + er.rival->getCarrilActual() * anchoCarril;
            float yR = er.rival->y();
            if (obs->puedeColisionar() &&
                xObs >= xR && xObs <= xR + anchoCarril && qAbs(yObs - yR) < 40) {
                er.impulsoVel = -(er.velBase * 0.5f);
                er.tiempoImpulsoVel = 1.5f;
                er.rival->activarDesaceleracion(0.5f);
                obs->activarCooldown(1.5f);
            }
        }
    }

    // Efectos (bananas): impulsan y desaparecen
    for (int i = efectos.size() - 1; i >= 0; i--) {
        ObjetoPista *ef = efectos[i];
        float xEf = ef->x();
        float yEf = ef->y();

        if (xEf >= xJug && xEf <= xJug + anchoCarril && qAbs(yEf - yJug) < 40) {
            impulsoJugador = velBaseJugador * 1.2f;
            tiempoImpulsoJugador = 2.5f;
            spawnTextoFlotante("BANANA!", jugador->x(), jugador->y() - 20, QColor(255, 255, 0));
            removeItem(ef);
            efectos.removeAt(i);
            delete ef;
            continue;
        }

        // Banana colision con rival
        for (auto &er : rivales) {
            float xR = margenX + er.rival->getCarrilActual() * anchoCarril;
            float yR = er.rival->y();
            if (xEf >= xR && xEf <= xR + anchoCarril && qAbs(yEf - yR) < 40) {
                er.impulsoVel = er.velBase * 0.8f;
                er.tiempoImpulsoVel = 2.0f;
                removeItem(ef);
                efectos.removeAt(i);
                delete ef;
                break;
            }
        }
    }
}

void Nivel1::verificarVictoria() {
    if (progresoJugador >= META) {
        finalizarCarrera("GANASTE!", Qt::green);
        return;
    }
    for (const auto &er : rivales) {
        if (er.progreso >= META) {
            finalizarCarrera("PERDISTE", Qt::red);
            return;
        }
    }
    if (tiempoRestante <= 0) {
        float mejorRival = 0;
        for (const auto &er : rivales)
            if (er.progreso > mejorRival) mejorRival = er.progreso;
        if (progresoJugador >= mejorRival)
            finalizarCarrera("GANASTE!\n(Tiempo)", Qt::green);
        else
            finalizarCarrera("PERDISTE\n(Tiempo)", Qt::red);
    }
}

void Nivel1::finalizarCarrera(const QString &mensaje, const QColor &color) {
    juegoActivo = false;
    carreraTerminada = true;
    timerJuego->stop();

    textoCuenta->setPlainText(mensaje);
    textoCuenta->setDefaultTextColor(color);
    textoCuenta->setFont(QFont("Arial", 32, QFont::Bold));
    textoCuenta->setPos(250, 380);

    btnReiniciar = new QGraphicsRectItem(220, 440, 240, 40);
    btnReiniciar->setBrush(QBrush(QColor(50, 120, 50)));
    btnReiniciar->setPen(QPen(QColor(80, 200, 80), 2));
    addItem(btnReiniciar);
    QGraphicsTextItem *lblR = addText("REINICIAR", QFont("Arial", 13, QFont::Bold));
    lblR->setDefaultTextColor(Qt::white);
    lblR->setPos(290, 447);

    btnMenu = new QGraphicsRectItem(220, 490, 240, 40);
    btnMenu->setBrush(QBrush(QColor(60, 60, 120)));
    btnMenu->setPen(QPen(QColor(100, 100, 200), 2));
    addItem(btnMenu);
    QGraphicsTextItem *lblM = addText("VOLVER AL MENU", QFont("Arial", 13, QFont::Bold));
    lblM->setDefaultTextColor(Qt::white);
    lblM->setPos(270, 497);
}

void Nivel1::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (!carreraTerminada) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }
    QPointF pos = event->scenePos();
    if (btnReiniciar && btnReiniciar->rect().contains(pos)) {
        emit reiniciarSolicitado();
        return;
    }
    if (btnMenu && btnMenu->rect().contains(pos)) {
        emit volverMenuSolicitado();
        return;
    }
    QGraphicsScene::mousePressEvent(event);
}

void Nivel1::spawnTextoFlotante(const QString &texto, float x, float y, const QColor &color) {
    QGraphicsTextItem *item = addText(texto, QFont("Arial", 11, QFont::Bold));
    item->setDefaultTextColor(color);
    item->setPos(x, y);
    textosFlotantes.append({item, 0.8f});
}

void Nivel1::actualizarTextosFlotantes() {
    for (int i = textosFlotantes.size() - 1; i >= 0; i--) {
        auto &[item, tiempo] = textosFlotantes[i];
        tiempo -= dt;
        item->setPos(item->x(), item->y() - 40.0f * dt);
        float alpha = qMax(0.0f, tiempo / 0.8f);
        QColor c = item->defaultTextColor();
        c.setAlphaF(alpha);
        item->setDefaultTextColor(c);
        if (tiempo <= 0) {
            removeItem(item);
            delete item;
            textosFlotantes.removeAt(i);
        }
    }
}

// ---- getters ----

JugadorNivel1* Nivel1::getJugador() const { return jugador; }
int Nivel1::getNumCarriles() const { return numCarriles; }

bool Nivel1::carrilLibre(int carril) const {
    float yJug = jugador->y();
    for (const auto &er : rivales) {
        if (er.rival->getCarrilActual() == carril && qAbs(er.rival->y() - yJug) < 60)
            return false;
    }
    return true;
}

// ---- stubs aun no implementados ----

void Nivel1::setDificultad(Dificultad d) { dificultad = d; }

void Nivel1::setPersonalidades(Personalidad *pJugador,
                               QList<Personalidad*> personalidadesRivales) {}

void Nivel1::iniciarCuentaRegresiva() {}

void Nivel1::actualizarBarraProgreso() {}
