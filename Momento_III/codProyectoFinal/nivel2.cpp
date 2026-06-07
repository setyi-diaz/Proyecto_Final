#include "nivel2.h"
#include "rampa.h"
#include "obstaculofriccion.h"

Nivel2::Nivel2(QObject *parent) : Nivel(parent), dificultad(DificultadNivel2::NORMAL) {
    jugador = new JugadorNivel2(35.f, 8.f, 420.f, 0.f);
    jugador->setAceleracion(18.f);
    jugador->setVelocidadMax(65.f);
    jugador->setPosX(20.f);
    jugador->setPosY(SUELO_Y);
    jugador->setVelocidadX(20.f);  // velocidad inicial para que no empiece desde 0

    rival = new RivalNivel2();
    rival->setAceleracion(17.f);
    rival->setVelocidadMax(70.f);
    rival->setPosX(0.f);
    rival->setPosY(SUELO_Y);
    rival->setVelocidadX(0.f);  // rival empieza desde 0

    obstaculos.push_back(new Rampa(30.f, 400.f, SUELO_Y, 40.f, 8.f));
    obstaculos.push_back(new ObstaculoFriccion(ObstaculoFriccion::LODO, 50.f, 440.f, SUELO_Y, 60.f, 2.f));
}

Nivel2::~Nivel2() {
    delete jugador;
    delete rival;
    for (Obstaculo* o : obstaculos) delete o;
}

void Nivel2::iniciar() {
    juegoActivo      = true;
    carreraTerminada = false;
    gano             = false;
}

void Nivel2::setDificultad(DificultadNivel2 d) {
    dificultad = d;

    if (d == DificultadNivel2::NORMAL) {
        jugador->setPosX(20.f);
        jugador->setVelocidadMax(65.f);
        rival->setAceleracion(17.f);
        rival->setVelocidadMax(70.f);
        rival->setCooldownBase(2.0f);
        rival->setMaxHistorial(5);
    } else {
        jugador->setPosX(0.f);
        jugador->setVelocidadMax(65.f);
        rival->setAceleracion(24.f);
        rival->setVelocidadMax(80.f);
        rival->setCooldownBase(1.2f);
        rival->setMaxHistorial(3);
    }
}

void Nivel2::procesarTecla(int key, bool presionada) {
    switch (key) {
    case Qt::Key_D: acelerando = presionada; break;
    case Qt::Key_A: frenando   = presionada; break;
    case Qt::Key_W: rotando    = presionada; break;
    default: break;
    }
}

void Nivel2::actualizar(float dt) {
    if (!juegoActivo) return;

    Rampa*     rampa = dynamic_cast<Rampa*>(obstaculos.at(0));
    Obstaculo* lodo  = obstaculos.at(1);

    float rampaPosX   = rampa->getPosX();
    float rampaAncho  = rampa->getAncho();
    float rampaAlto   = rampa->getAlto();
    float anguloRampa = rampa->getAngulo();

    float jx = jugador->getPosX();

    bool sobreRampa  = (jx + MOTO_W) >= rampaPosX
                    && (jx + MOTO_W) <= (rampaPosX + rampaAncho)
                    && jugador->getEnSuelo();
    bool enTopeRampa = jugador->getEnSuelo()
                    && (jx + MOTO_W) > (rampaPosX + rampaAncho - MOTO_W * 0.5f)
                    && (jx + MOTO_W) <= (rampaPosX + rampaAncho + MOTO_W * 0.5f);
    bool enVuelo     = !jugador->getEnSuelo();
    bool sobreLodo   = jx >= lodo->getPosX()
                    && jx <= (lodo->getPosX() + lodo->getAncho());

    // Controles del jugador en suelo (siempre que esté en suelo y no en vuelo)
    if (jugador->getEnSuelo() && !enVuelo) {
        if (acelerando)       jugador->acelerar(dt);
        else if (frenando)    jugador->frenar(dt);
        else                  jugador->desaceleracionAuto(dt);

        if (!sobreRampa) {
            // En suelo plano
            jugador->setPosY(SUELO_Y);
        } else {
            // Sobre rampa - seguir la pendiente
            float distEnRampa = (jx + MOTO_W) - rampaPosX;
            float progreso    = distEnRampa / rampaAncho;
            jugador->setPosY(SUELO_Y + progreso * rampaAlto);
            jugador->setAnguloSprite(anguloRampa * 180.f / M_PI);
        }
    }

    if (enTopeRampa) {
        jugador->setPosY(SUELO_Y + rampaAlto);
        jugador->reiniciarRotacion();
        jugador->setVelAlDespegue(jugador->getVelocidadX());
        rampa->aplicarEfecto(jugador, dt);
        emit sonidoRampa();
    }

    if (enVuelo) {
        jugador->aplicarGravedad(dt, G);
        if (rotando) jugador->inclinar(dt);
    }

    if (enVuelo && jugador->getPosY() <= SUELO_Y) {
        jugador->setPosY(SUELO_Y);
        jugador->setEnSuelo(true);
        float vMaxAntes   = jugador->getVelocidadMax();
        float velAntes    = jugador->getVelocidadX();
        auto resultado    = jugador->evaluarAterrizaje();
        float vMaxDespues = jugador->getVelocidadMax();
        float velDespues  = jugador->getVelocidadX();
        jugador->setVelAlDespegue(velDespues);

        using R = JugadorNivel2::ResultadoAterrizaje;
        if (resultado == R::BONUS)
            emit bonoVelocidad(vMaxDespues - vMaxAntes);
        else if (resultado == R::PENALIZACION_ESPALDA) {
            emit impactoBanana(velAntes - velDespues);
            emit caidaEspalda();
        }
    }

    if (sobreLodo && jugador->getEnSuelo()
            && jugador->getVelAlDespegue() <= umbralEsquive) {
        if (jugador->getVelocidadX() > 10.f) {
            lodo->aplicarEfecto(jugador, dt);
        }
        if (!enLodoAnterior) emit sonidoLodo();  // solo al entrar, no cada frame
    }
    enLodoAnterior = sobreLodo && jugador->getEnSuelo()
                     && jugador->getVelAlDespegue() <= umbralEsquive;

    jugador->actualizarPosicion(dt);

    actualizarRival(dt);

    float camaraLogica = jugador->getPosX() - SCENE_W_LOGICA * 0.33f;
    reciclarObstaculos(camaraLogica);

    verificarVictoriaDerrota();
    emit actualizado();
}

void Nivel2::actualizarRival(float dt) {
    Rampa*     rampa = dynamic_cast<Rampa*>(obstaculos.at(0));
    Obstaculo* lodo  = obstaculos.at(1);

    float rampaPosX  = rampa->getPosX();
    float rampaAncho = rampa->getAncho();
    float rampaAlto  = rampa->getAlto();

    float rx = rival->getPosX();

    bool rivalSobreRampa  = (rx + MOTO_W) >= rampaPosX
                         && (rx + MOTO_W) <= (rampaPosX + rampaAncho)
                         && rival->getEnSuelo();
    bool rivalEnTopeRampa = rival->getEnSuelo()
                         && (rx + MOTO_W) > (rampaPosX + rampaAncho - MOTO_W * 0.5f)
                         && (rx + MOTO_W) <= (rampaPosX + rampaAncho + MOTO_W * 0.5f);
    bool rivalSobreLodo   = rx >= lodo->getPosX()
                         && rx <= (lodo->getPosX() + lodo->getAncho());

    float velActual = rival->getVelocidadX();

    if (rivalSobreLodo && rival->getEnSuelo()) {
        // En el lodo: fuerte fricción, rival frena
        float friccionRival = 80.f;
        float nuevaVel = velActual - friccionRival * dt;
        if (nuevaVel < 5.f) nuevaVel = 5.f;
        rival->setVelocidadX(nuevaVel);
    } else if (rival->getEnSuelo()) {
        // Fuera del lodo en suelo: acelera con su propia aceleración hasta su propio máximo
        rival->acelerar(dt);
    }

    if (rival->getEnSuelo()) {
        if (!rivalSobreRampa) {
            rival->setPosY(SUELO_Y);
        } else {
            float distEnRampa = (rx + MOTO_W) - rampaPosX;
            float progreso = distEnRampa / rampaAncho;
            rival->setPosY(SUELO_Y + progreso * rampaAlto);
        }
    }

    // Rival llega al tope de la rampa - inicia vuelo
    if (rivalEnTopeRampa) {
        rival->setPosY(SUELO_Y + rampaAlto);
        rampa->aplicarEfecto(rival, dt);
    }

    // Física en el aire
    if (!rival->getEnSuelo()) {
        rival->velocidadY -= G * dt;
        rival->actualizarPosicion(dt);

        if (rival->getPosY() <= SUELO_Y) {
            rival->setPosY(SUELO_Y);
            rival->setEnSuelo(true);
            rival->velocidadY = 0.f;
        }
    } else {
        rival->actualizarPosicion(dt);
    }

    // El lodo ya se aplica arriba en la lógica de velocidad

    rival->actualizarCoolDown(dt);
    rival->actualizarAnimacion(dt);

    actualizarBananas(dt);

    timerAgente += dt;
    if (timerAgente < INTERVALO_AGENTE) return;
    timerAgente = 0.f;

    // El rival corre de forma independiente y lanza bananas cuando va atrás
    float distRival  = rival->getPosX();
    float distJugador = jugador->getPosX();
    float diferencia = distJugador - distRival;

    // Lanza bananas si el jugador va adelante (incluso estando en pantalla)
    if (diferencia > 10.f && rival->cooldownListo()) {
        rival->percibirJugador(jugador->getPosX(),
                               jugador->getPosY(),
                               jugador->getVelocidadX());
        if (rival->estimarImpacto(G)) {
            rival->lanzarBanana();
            emit sonidoBanana();
        }
    }

    // El rival siempre corre a su máxima velocidad natural, sin ajustes artificiales
    // Su velocidad solo se ve afectada por la física (rampa, lodo, gravedad)
}

void Nivel2::actualizarBananas(float dt) {
    Proyectil* b = rival->getBanana();
    if (!b->getActiva()) return;

    b->actualizarPosicion(dt, G);

    if (b->getPosY() <= SUELO_Y) {
        rival->guardarErrorImpacto(b->getPosX() - jugador->getPosX());
        b->setActiva(false);
        return;
    }

    float dx = std::abs(b->getPosX() - jugador->getPosX());
    float dy = std::abs(b->getPosY() - jugador->getPosY());
    if (dx < MOTO_W * 2.f && dy < MOTO_H * 3.f) {
        float velAntes = jugador->getVelocidadX();

        // Daño diferenciado por dificultad — reducido para no ser tan castigador
        float factorVMax = (dificultad == DificultadNivel2::DIFICIL) ? 0.93f : 0.97f;
        float factorVel  = (dificultad == DificultadNivel2::DIFICIL) ? 0.75f : 0.88f;

        float nuevaVMax = jugador->getVelocidadMax() * factorVMax;
        if (nuevaVMax < 40.f) nuevaVMax = 40.f;
        jugador->setVelocidadMax(nuevaVMax);

        float nuevaVel = velAntes * factorVel;
        jugador->setVelocidadX(nuevaVel);

        rival->guardarErrorImpacto(0.f);
        b->setActiva(false);
        emit sonidoBanana();
        emit impactoBanana(velAntes - nuevaVel);
    }
}

void Nivel2::reciclarObstaculos(float camaraXLogica) {
    float bordeIzq            = camaraXLogica - MARGEN_RECICLADO;
    float bordeDerechoVisible = camaraXLogica + SCENE_W_LOGICA;

    Obstaculo* rampa = obstaculos.at(0);
    Obstaculo* lodo  = obstaculos.at(1);

    if ((rampa->getPosX() + rampa->getAncho()) < bordeIzq) {
        rampa->setPosX(bordeDerechoVisible + SCENE_W_LOGICA * 0.85f);
        lodo->setPosX(rampa->getPosX() + rampa->getAncho());
    }
}

void Nivel2::verificarVictoriaDerrota() {
    if (carreraTerminada) return;

    if (jugador->getPosX() >= META_X) {
        juegoActivo      = false;
        carreraTerminada = true;
        gano             = true;
        emit carreraFinalizada(true);
        return;
    }

    if (rival->getPosX() >= META_X) {
        juegoActivo      = false;
        carreraTerminada = true;
        gano             = false;
        emit carreraFinalizada(false);
    }
}
