#include "nivel1.h"
#include "itembanana.h"
#include "itemobstaculo.h"
#include <QRandomGenerator>
#include <QtMath>
#include <stdexcept>

static const float ESCENA_ANCHO        = 680.0f;
static const float ANCHO_PISTA         = 570.0f;
static const float ANCHO_CARRIL_MAX    = 95.0f;
static const float POS_Y_JUGADOR       = 700.0f;
static const float TILE_ALTO           = 850.0f;

static const float VEL_OBSTACULOS_INI  = 2.0f;
static const float VEL_OBSTACULOS_MAX  = 5.0f;
static const float VEL_OBSTACULOS_INCR = 0.01f;
static const float TIEMPO_TOTAL        = 180.0f;
static const float INTERVALO_BANANA    = 4.5f;
static const float COOLDOWN_GOLPE_C    = 3.0f;

static const float DIST_COLISION_Y     = 40.0f;
static const float DIST_SOLAPAMIENTO   = 45.0f;
static const float COOLDOWN_OBSTACULO  = 1.5f;
static const float DURACION_GOLPE_REC  = 0.6f;
static const float DURACION_DESACEL    = 0.5f;

static const float OMEGA_AGUA_INI      = 540.0f;
static const float K_AMORT_AGUA        = 2.5f;

static const float MU_BASE             = 2.5f;
static const float MU_LODO_NORMAL      = 6.5f;
static const float MU_LODO_DIFICIL     = 9.0f;
static const float DURACION_LODO       = 1.5f;
static const float DURACION_LODO_DIF   = 2.0f;
static const float FRAC_RETORNO_MU     = 0.25f;
static const float F_BOOST_NORMAL      = 2.0f;
static const float F_BOOST_DIFICIL     = 1.3f;
static const float DURACION_BOOST      = 2.5f;

static const QList<QColor> COLORES_RIVAL = {
    Qt::red, QColor(255,140,0), QColor(180,0,180),
    QColor(0,200,100), QColor(80,180,255)
};

Nivel1::Nivel1(int numCarriles_, int numRivales_, QObject *parent)
    : Nivel(parent),
      numCarriles(numCarriles_),
      numRivalesConfig(numRivales_),
      anchoCarril(qMin(ANCHO_CARRIL_MAX, ANCHO_PISTA / numCarriles_)),
      margenX((ESCENA_ANCHO - numCarriles_ * anchoCarril) / 2.0f),
      jugador(nullptr),
      personalidadJugador{},
      progresoJugador(0),
      velActualJugador(0.3f),
      velBaseJugador(0.3f),
      muExtraJugador(0),
      tiempoLodoJugador(0),
      durLodoJugador(0),
      fBoostJugador(0),
      tiempoBoostJugador(0),
      durBoostJugador(0),
      tiempoTotal(TIEMPO_TOTAL),
      tiempoRestante(TIEMPO_TOTAL),
      cuentaRegresiva(3),
      dtInterno(0.016f),
      tiempoObstaculo(0),
      intervaloObstaculos(2.8f - numCarriles_ * 0.2f),
      cooldownGolpe(0),
      velocidadObstaculos(VEL_OBSTACULOS_INI),
      fondoOffsetY(0),
      dificultad(DificultadNivel1::NORMAL)
{
    if (numCarriles_ < 3 || numCarriles_ > 10)
        throw std::invalid_argument("Numero de carriles fuera de rango (3-10)");
    if (numRivales_ < 1 || numRivales_ >= numCarriles_)
        throw std::invalid_argument("Numero de rivales invalido para los carriles dados");

    int carrilJugador = numCarriles / 2;
    jugador = new JugadorNivel1();
    jugador->setPosX(margenX + carrilJugador * anchoCarril + anchoCarril * 0.5f);
    jugador->setPosY(POS_Y_JUGADOR);
    jugador->cambiarCarril(carrilJugador);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Nivel1::tickInterno);

    timerRegresiva = new QTimer(this);
    connect(timerRegresiva, &QTimer::timeout, this, &Nivel1::contarRegresiva);
}

Nivel1::~Nivel1() {
    qDeleteAll(obstaculos);
    qDeleteAll(efectos);
    for (auto &er : rivales) delete er.rival;
    delete jugador;
}

void Nivel1::setDificultad(DificultadNivel1 d) {
    dificultad = d;
    for (auto &er : rivales) er.rival->setDificultad(d);
}

void Nivel1::crearRival(int carril, const CorredorNivel1::Personalidad &p, const QColor &color) {
    EstadoRival er;
    er.rival = new RivalNivel1();
    er.rival->setColorBase(color);
    er.rival->setNumCarriles(numCarriles);
    er.rival->setPersonalidad(p);
    er.rival->aplicarPersonalidad();
    er.rival->setPosX(margenX + carril * anchoCarril + anchoCarril * 0.5f);
    er.rival->setPosY(POS_Y_JUGADOR);
    er.rival->cambiarCarril(carril);
    er.color = color;
    er.progreso = 0;
    rivales.append(er);
}

void Nivel1::setPersonalidades(const CorredorNivel1::Personalidad &pJugador,
                               const QList<CorredorNivel1::Personalidad> &pRivales) {
    jugador->setPersonalidad(pJugador);
    jugador->aplicarPersonalidad();

    int carrilJugador = numCarriles / 2;
    QList<int> carrilesDisponibles;
    for (int i = 0; i < numCarriles; i++)
        if (i != carrilJugador) carrilesDisponibles.append(i);

    int step = qMax(1, carrilesDisponibles.size() / numRivalesConfig);
    for (int i = 0; i < numRivalesConfig; i++) {
        int idxCarril = (i * step) % carrilesDisponibles.size();
        auto p   = (i < pRivales.size()) ? pRivales[i] : CorredorNivel1::Personalidad::disponibles()[0];
        auto col = (i < COLORES_RIVAL.size()) ? COLORES_RIVAL[i] : Qt::magenta;
        crearRival(carrilesDisponibles[idxCarril], p, col);
    }
}

void Nivel1::iniciar() {
    cuentaRegresiva = 3;
    juegoActivo = false;
    inicializarPoolEfectos();  // aquí las señales ya están conectadas
    emit cuentaRegresivaCambio(cuentaRegresiva);
    timerRegresiva->start(1000);
}

void Nivel1::contarRegresiva() {
    cuentaRegresiva--;
    emit cuentaRegresivaCambio(cuentaRegresiva);
    if (cuentaRegresiva < 0) {
        timerRegresiva->stop();
        juegoActivo = true;
        reloj.start();
        timerJuego->start(16);
        emit juegoIniciado();
    }
}

// El timer interno llama a actualizar con el dt medido
void Nivel1::tickInterno() {
    float dt = qMin(reloj.elapsed() / 1000.0f, 0.05f);
    reloj.restart();
    actualizar(dt);
}

void Nivel1::actualizar(float dt) {
    if (!juegoActivo) return;

    tiempoRestante -= dt;
    if (tiempoRestante <= 0) tiempoRestante = 0;

    float muJugador = MU_BASE;
    if (tiempoLodoJugador > 0) {
        tiempoLodoJugador -= dt;
        if (tiempoLodoJugador > 0) {
            float frac = tiempoLodoJugador / durLodoJugador;
            muJugador = (frac > FRAC_RETORNO_MU)
                ? muExtraJugador
                : MU_BASE + (muExtraJugador - MU_BASE) * (frac / FRAC_RETORNO_MU);
        } else {
            muExtraJugador = 0;
        }
    }

    float fBoostActual = 0.0f;
    if (tiempoBoostJugador > 0) {
        tiempoBoostJugador -= dt;
        if (tiempoBoostJugador > 0) {
            float frac = tiempoBoostJugador / durBoostJugador;
            fBoostActual = (frac > FRAC_RETORNO_MU) ? fBoostJugador : fBoostJugador * (frac / FRAC_RETORNO_MU);
        } else {
            fBoostJugador = 0;
        }
    }

    float fMotor = MU_BASE * velBaseJugador + fBoostActual;
    velActualJugador += (fMotor - muJugador * velActualJugador) * dt;
    if (velActualJugador < 0.05f) velActualJugador = 0.05f;
    progresoJugador += velActualJugador * dt;

    for (auto &er : rivales) {
        er.velBase = 0.25f + er.rival->getVelocidadMax() * 0.002f;
        float muRival = MU_BASE;
        if (er.tiempoLodo > 0) {
            er.tiempoLodo -= dt;
            if (er.tiempoLodo > 0) {
                float frac = er.tiempoLodo / er.durLodo;
                muRival = (frac > FRAC_RETORNO_MU) ? er.muExtra : MU_BASE + (er.muExtra - MU_BASE) * (frac / FRAC_RETORNO_MU);
            } else { er.muExtra = 0; }
        }
        float fBoostR = 0.0f;
        if (er.tiempoBoost > 0) {
            er.tiempoBoost -= dt;
            if (er.tiempoBoost > 0) {
                float frac = er.tiempoBoost / er.durBoost;
                fBoostR = (frac > FRAC_RETORNO_MU) ? er.fBoost : er.fBoost * (frac / FRAC_RETORNO_MU);
            } else { er.fBoost = 0; }
        }
        float fMotorR = MU_BASE * er.velBase + fBoostR;
        er.velActual += (fMotorR - muRival * er.velActual) * dt;
        if (er.velActual < 0.0f) er.velActual = 0.0f;
        er.progreso += er.velActual * dt;

        er.rival->actualizarIA(dt);
        bool golpeo = er.rival->tomarDecision(jugador, dt);
        if (golpeo) {
            float mult = (dificultad == DificultadNivel1::DIFICIL) ? 1.6f : 1.0f;
            float pen  = 25.0f * er.rival->getFuerzaGolpe() / jugador->getResistenciaGolpe() * mult;
            progresoJugador -= pen;
            if (progresoJugador < 0) progresoJugador = 0;
            jugador->activarGolpeRecibido(0.5f);
            emit textoFlotanteCreado("GOLPE RIVAL!", jugador->getPosX(), jugador->getPosY() - 20, QColor(255,80,80));
        }
    }

    if (cooldownGolpe > 0) cooldownGolpe -= dt;

    velocidadObstaculos += VEL_OBSTACULOS_INCR * dt;
    if (velocidadObstaculos > VEL_OBSTACULOS_MAX) velocidadObstaculos = VEL_OBSTACULOS_MAX;

    fondoOffsetY += velocidadObstaculos;
    if (fondoOffsetY >= TILE_ALTO) fondoOffsetY -= TILE_ALTO;

    float yJugador = POS_Y_JUGADOR - progresoJugador * 8.0f;
    float xJugador = margenX + jugador->getCarrilActual() * anchoCarril + anchoCarril * 0.5f;
    jugador->setPosX(xJugador);
    jugador->setPosY(yJugador);

    for (auto &er : rivales) {
        float yR = POS_Y_JUGADOR - er.progreso * 8.0f;
        er.rival->setPosY(yR);
        if (er.rival->getCarrilActual() == jugador->getCarrilActual() &&
            qAbs(yJugador - yR) < DIST_SOLAPAMIENTO) {
            int c = er.rival->getCarrilActual();
            er.rival->cambiarCarril((c + 1 < numCarriles) ? c + 1 : c - 1);
        }
        for (int j = 0; j < rivales.size(); j++) {
            if (rivales[j].rival == er.rival) continue;
            if (rivales[j].rival->getCarrilActual() == er.rival->getCarrilActual() &&
                qAbs(rivales[j].rival->getPosY() - yR) < DIST_SOLAPAMIENTO) {
                int c = er.rival->getCarrilActual();
                er.rival->cambiarCarril((c + 1 < numCarriles) ? c + 1 : c - 1);
                break;
            }
        }
        float xR = margenX + er.rival->getCarrilActual() * anchoCarril + anchoCarril * 0.5f;
        er.rival->setPosX(xR);
        er.rival->setPosY(yR);
        er.rival->actualizarAnimaciones(dt);
    }

    jugador->actualizarAnimaciones(dt);

    for (auto *obs : obstaculos) {
        obs->actualizarCooldown(dt);
        float nuevaY = obs->getPosY() + velocidadObstaculos;
        obs->setPosY(nuevaY > 880 ? -30 : nuevaY);
    }
    for (int i = 0; i < efectos.size(); i++) {
        Item *ef = efectos[i];
        float nuevaY = ef->getPosY() + velocidadObstaculos;
        ef->setPosY(nuevaY);
        if (nuevaY > 880) {
            reciclarEfecto(i);  // reubicar fuera de pantalla en lugar de destruir
        }
    }

    tiempoObstaculo += dt;
    if (tiempoObstaculo > intervaloObstaculos) { tiempoObstaculo = 0; generarObstaculos(); }

    // Activar una banana del pool cada INTERVALO_BANANA si hay alguna en espera
    tiempoEsperaBanana += dt;
    if (tiempoEsperaBanana > INTERVALO_BANANA) {
        tiempoEsperaBanana = 0.f;
        for (int i = 0; i < efectos.size(); i++) {
            if (efectos[i]->getPosY() < -100.f) {
                efectos[i]->setPosY(-30.f);  // poner en juego
                break;
            }
        }
    }

    actualizarColisiones();
    verificarVictoria();
    emit actualizado();
}

void Nivel1::procesarTecla(int key, bool presionada) {
    if (!juegoActivo || !presionada) return;
    switch (key) {
    case Qt::Key_W: aumentarVelocidad();  break;
    case Qt::Key_S: disminuirVelocidad(); break;
    case Qt::Key_A:
        if (!jugador->estaRotandoAgua() && jugador->getCarrilActual() > 0 &&
            carrilLibre(jugador->getCarrilActual() - 1))
            jugador->cambiarCarril(jugador->getCarrilActual() - 1);
        break;
    case Qt::Key_D:
        if (!jugador->estaRotandoAgua() && jugador->getCarrilActual() < numCarriles - 1 &&
            carrilLibre(jugador->getCarrilActual() + 1))
            jugador->cambiarCarril(jugador->getCarrilActual() + 1);
        break;
    case Qt::Key_J: golpearRival(-1); break;
    case Qt::Key_K: golpearRival(+1); break;
    default: break;
    }
}

void Nivel1::aumentarVelocidad() {
    float tope = jugador->getVelocidadMax();
    velBaseJugador += jugador->getAceleracion() * 0.015f;
    if (velBaseJugador > tope * 0.02f) velBaseJugador = tope * 0.02f;
}

void Nivel1::disminuirVelocidad() {
    velBaseJugador -= jugador->getAceleracion() * 0.01f;
    if (velBaseJugador < 0.1f) velBaseJugador = 0.1f;
}

void Nivel1::golpearRival(int direccion) {
    jugador->activarGolpeAnimacion(0.5f, direccion < 0 ? 2 : 1);
    if (cooldownGolpe > 0) return;
    int carrilJ = jugador->getCarrilActual();
    int carrilObjetivo = carrilJ + direccion;
    float yJug = jugador->getPosY();
    bool golpeo = false;
    for (auto &er : rivales) {
        if (er.rival->getCarrilActual() == carrilObjetivo && qAbs(yJug - er.rival->getPosY()) < 80) {
            float pen = 30.0f * jugador->getFuerzaGolpe() / er.rival->getResistenciaGolpe();
            er.progreso -= pen;
            if (er.progreso < 0) er.progreso = 0;
            er.rival->activarGolpeRecibido(DURACION_GOLPE_REC);
            emit textoFlotanteCreado("GOLPE!", er.rival->getPosX(), er.rival->getPosY() - 20, QColor(255,220,0));
            golpeo = true;
        }
    }
    cooldownGolpe = COOLDOWN_GOLPE_C;
    if (!golpeo)
        emit textoFlotanteCreado("FALLO", jugador->getPosX(), jugador->getPosY() - 20, QColor(180,180,180));
}

bool Nivel1::carrilLibre(int carril) const {
    float yJug = jugador->getPosY();
    for (const auto &er : rivales)
        if (er.rival->getCarrilActual() == carril && qAbs(er.rival->getPosY() - yJug) < 45)
            return false;
    return true;
}

void Nivel1::generarObstaculos() {
    float yJug = jugador->getPosY();
    QList<int> carrilesLibres;
    for (int c = 0; c < numCarriles; c++) {
        bool ocupado = false;
        for (auto *obs : obstaculos) {
            int carrilObs = (int)((obs->getPosX() - margenX) / anchoCarril);
            if (carrilObs == c && qAbs(obs->getPosY() - yJug) < 120) { ocupado = true; break; }
        }
        if (!ocupado) carrilesLibres.append(c);
    }
    if (carrilesLibres.size() <= 1) return;
    for (int i = carrilesLibres.size() - 1; i > 0; i--) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        carrilesLibres.swapItemsAt(i, j);
    }
    int carril = carrilesLibres[0];
    int r = QRandomGenerator::global()->bounded(7);
    int tipo = (r <= 2) ? 0 : (r <= 5) ? 1 : 2;
    float friccion = (tipo == 0) ? 0.7f : (tipo == 1) ? 1.0f : 0.5f;
    obstaculos.append(new ItemObstaculo(margenX + carril * anchoCarril + anchoCarril * 0.05f, -30, carril,
                                         static_cast<ItemObstaculo::Tipo>(tipo), friccion));
    emit obstaculoCreado(obstaculos.size() - 1);
}

void Nivel1::inicializarPoolEfectos() {
    // Crea el pool fijo — todos inician fuera de pantalla (Y muy negativo)
    float ef = 0.5f;
    for (int i = 0; i < POOL_BANANAS; i++) {
        int carril = i % numCarriles;
        float x = margenX + carril * anchoCarril + anchoCarril * 0.05f;
        efectos.append(new ItemBanana(x, -2000.f, carril, ef));
        emit efectoCreado(efectos.size() - 1);
    }
}

void Nivel1::reciclarEfecto(int index) {
    // En lugar de destruir, reubica a Y=-30 con nuevo carril y efecto aleatorio
    if (index < 0 || index >= efectos.size()) return;
    Item *ef = efectos[index];
    int nuevoCarril = QRandomGenerator::global()->bounded(numCarriles);
    float nuevoEfecto = QRandomGenerator::global()->bounded(100) < 70 ? 0.5f : -0.7f;
    ef->setPosX(margenX + nuevoCarril * anchoCarril + anchoCarril * 0.05f);
    ef->setPosY(-2000.f);  // espera fuera de pantalla hasta ser "lanzada"
    // Actualiza el efecto mediante cast
    if (auto *banana = dynamic_cast<ItemBanana*>(ef)) {
        banana->setImpulso(nuevoEfecto);
    }
}

void Nivel1::actualizarColisiones() {
    int   carrilJ = jugador->getCarrilActual();
    float yJug    = jugador->getPosY();
    float xJug    = margenX + carrilJ * anchoCarril;
    float muLodo  = (dificultad == DificultadNivel1::DIFICIL) ? MU_LODO_DIFICIL  : MU_LODO_NORMAL;
    float durLodo = (dificultad == DificultadNivel1::DIFICIL) ? DURACION_LODO_DIF : DURACION_LODO;
    float fBoost  = (dificultad == DificultadNivel1::DIFICIL) ? F_BOOST_DIFICIL   : F_BOOST_NORMAL;

    for (auto *obs : obstaculos) {
        float xObs = obs->getPosX(), yObs = obs->getPosY();
        if (obs->puedeColisionar() && xObs >= xJug && xObs <= xJug + anchoCarril && qAbs(yObs - yJug) < DIST_COLISION_Y) {
            if (obs->esAgua()) {
                if (!jugador->estaRotandoAgua()) {
                    jugador->activarRotacionAgua(OMEGA_AGUA_INI, K_AMORT_AGUA);
                    obs->activarCooldown(COOLDOWN_OBSTACULO);
                    emit sonidoRotacion();
                    emit textoFlotanteCreado("CHARCO!", jugador->getPosX(), jugador->getPosY() - 20, QColor(100,180,255));
                }
            } else {
                muExtraJugador = muLodo; tiempoLodoJugador = durLodo; durLodoJugador = durLodo;
                jugador->activarDesaceleracion(DURACION_DESACEL);
                obs->activarCooldown(COOLDOWN_OBSTACULO);
                emit textoFlotanteCreado("CHOQUE!", jugador->getPosX(), jugador->getPosY() - 20, QColor(255,120,0));
            }
        }
        for (auto &er : rivales) {
            if (er.rival->estaDesacelerado() || er.rival->estaRotandoAgua()) continue;
            float xR = margenX + er.rival->getCarrilActual() * anchoCarril, yR = er.rival->getPosY();
            if (xObs >= xR && xObs <= xR + anchoCarril && qAbs(yObs - yR) < DIST_COLISION_Y) {
                if (obs->esAgua()) { er.rival->activarRotacionAgua(OMEGA_AGUA_INI, K_AMORT_AGUA); obs->activarCooldown(COOLDOWN_OBSTACULO); }
                else { er.muExtra = muLodo; er.tiempoLodo = durLodo; er.durLodo = durLodo; er.rival->activarDesaceleracion(DURACION_DESACEL); }
                break;
            }
        }
    }
    for (int i = 0; i < efectos.size(); i++) {
        Item *ef = efectos[i];
        float xEf = ef->getPosX(), yEf = ef->getPosY();
        if (yEf < -100.f) continue;  // en espera, saltar

        if (xEf >= xJug && xEf <= xJug + anchoCarril && qAbs(yEf - yJug) < DIST_COLISION_Y) {
            if (ef->esImpulso()) {
                fBoostJugador = fBoost; tiempoBoostJugador = DURACION_BOOST; durBoostJugador = DURACION_BOOST;
                emit sonidoBoost();
                emit textoFlotanteCreado("BANANA!", jugador->getPosX(), jugador->getPosY() - 20, QColor(255,255,0));
            } else {
                muExtraJugador = muLodo; tiempoLodoJugador = durLodo; durLodoJugador = durLodo;
                emit textoFlotanteCreado("LODO!", jugador->getPosX(), jugador->getPosY() - 20, QColor(150,100,50));
            }
            reciclarEfecto(i);  // reubica en lugar de destruir
            continue;
        }
        for (auto &er : rivales) {
            float xR = margenX + er.rival->getCarrilActual() * anchoCarril, yR = er.rival->getPosY();
            if (xEf >= xR && xEf <= xR + anchoCarril && qAbs(yEf - yR) < DIST_COLISION_Y) {
                if (ef->esImpulso()) { er.fBoost = fBoost; er.tiempoBoost = DURACION_BOOST; er.durBoost = DURACION_BOOST; }
                else { er.muExtra = muLodo; er.tiempoLodo = durLodo; er.durLodo = durLodo; }
                reciclarEfecto(i);  // reubica en lugar de destruir
                break;
            }
        }
    }
}

void Nivel1::verificarVictoria() {
    bool jugGano = jugador->getPosY() < -20;
    bool algRivalGano = false;
    for (const auto &er : rivales)
        if (er.rival->getPosY() < -20) { algRivalGano = true; break; }

    if (jugGano || algRivalGano) {
        juegoActivo = false; carreraTerminada = true; gano = jugGano;
        timerJuego->stop(); emit carreraFinalizada(gano); return;
    }
    if (tiempoRestante <= 0) {
        float mejorRival = 0;
        for (const auto &er : rivales) if (er.progreso > mejorRival) mejorRival = er.progreso;
        juegoActivo = false; carreraTerminada = true;
        gano = (progresoJugador >= mejorRival);
        timerJuego->stop(); emit carreraFinalizada(gano);
    }
}

JugadorNivel1*             Nivel1::getJugador()            const { return jugador; }
const QList<EstadoRival>&  Nivel1::getRivales()            const { return rivales; }
const QList<Item*>& Nivel1::getObstaculos()         const { return obstaculos; }
const QList<Item*>& Nivel1::getEfectos()            const { return efectos; }
float  Nivel1::getProgresoJugador()     const { return progresoJugador; }
float  Nivel1::getVelActualJugador()    const { return velActualJugador; }
float  Nivel1::getTiempoRestante()      const { return tiempoRestante; }
float  Nivel1::getCooldownGolpe()       const { return cooldownGolpe; }
float  Nivel1::getVelocidadObstaculos() const { return velocidadObstaculos; }
float  Nivel1::getFondoOffsetY()        const { return fondoOffsetY; }
int    Nivel1::getNumCarriles()         const { return numCarriles; }
float  Nivel1::getAnchoCarril()         const { return anchoCarril; }
float  Nivel1::getMargenX()             const { return margenX; }
int    Nivel1::getCuentaRegresiva()     const { return cuentaRegresiva; }
