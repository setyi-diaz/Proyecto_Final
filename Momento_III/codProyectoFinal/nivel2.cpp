#include "nivel2.h"
#include "lodo.h"
#include "rampa.h"

Nivel2::Nivel2() {
    jugador = new Jugador();
    jugador->setAceleracion(20.f);
    jugador->setVelocidadMax(50.f);
    jugador->setPosX(0.f);
    jugador->setPosY(SUELO_Y);

    rival = new Rival();
    rival->setAceleracion(20.f);
    rival->setVelocidadMax(50.f);
    rival->setPosX(0.f);
    rival->setPosY(SUELO_Y);

    obstaculos.push_back(new Rampa(30.f, 400.f, SUELO_Y, 40.f, 8.f));
    obstaculos.push_back(new Lodo (20.f, 600.f, SUELO_Y, 60.f, 2.f));
}
Nivel2::~Nivel2() {
    delete jugador;
    for (Obstaculo* o : obstaculos) delete o;
}
void Nivel2::dinamicaNivel(float dt, bool acelerando, bool frenando,bool rotando){
    {
        // interaccion Jugador <-> Terreno plano
        if (jugador->getPosY() >= SUELO_Y){
            jugador->setEnSuelo(true);
            if (acelerando)
                jugador->acelerar(dt);

            if (frenando)
                jugador->frenar(dt);

            if (!acelerando && !frenando)
                jugador->desaceleracionAuto(dt);
        }
        // interaccion rival <-> Terreno plano
        if(rival->getPosY() >= SUELO_Y){
            rival->acelerar(dt);
        }

        bool sobreRampa = (jugador->getPosX() + MOTO_W) >= obstaculos.at(0)->getPosX()
                          && (jugador->getPosX() + MOTO_W) <= (obstaculos.at(0)->getPosX()+ obstaculos.at(0)->getAncho());

        bool enVuelo = !jugador->getEnSuelo();

        bool sobreLodo = jugador->getPosX() >= obstaculos.at(1)->getPosX()
                         && jugador->getPosX() <= (obstaculos.at(1)->getPosX()+obstaculos.at(1)->getAncho());

        // interaccion jugador <-> rampa
        if(sobreRampa){
            // el jugador sigue el engulo de la rampa
            jugador->setAnguloSprite(dynamic_cast<Rampa*>(obstaculos.at(0))->getAngulo());
        }
        // al llegar el extremo superior de la rampa:
        else if(enVuelo){
            obstaculos.at(0)->aplicarEfecto(jugador,dt);
            if(rotando)
                jugador->inclinar(dt);
        }
        // interaccion jugador <-> Lodo
        if(sobreLodo && jugador->getVelAlDespegue() <= umbralEsquive){
            obstaculos.at(1)->aplicarEfecto(jugador,dt);
        }
        // interaccion jugador <-> aterrizaje tras vuelo
        if(jugador->getEnSuelo() == false && jugador->getPosY() <= SUELO_Y){
            jugador->evaluarAterrizaje();
        }

        jugador->actualizarPosicion(dt);

        actualizarRival(dt);

        float camaraLogica = jugador->getPosX() - SCENE_W_LOGICA * 0.33f;
        reciclarObstaculos(camaraLogica);
    }
}
void Nivel2::actualizarRival(float dt) {

    // El rival intenta igualar la velocidad del jugador (conducción básica)
    if (rival->getPosY() >= SUELO_Y) {
        rival->setEnSuelo(true);
        rival->ajustarVelocidad(jugador->getVelocidadX());
    }
    rival->actualizarPosicion(dt);
    rival->actualizarCoolDown(dt);

    // ── Actualizar bananas en vuelo ───────────────────────────────────────────
    actualizarBananas(dt);

    // ── Ciclo del agente (cada INTERVALO_AGENTE segundos) ─────────────────────
    timerAgente += dt;
    if (timerAgente < INTERVALO_AGENTE) return;
    timerAgente = 0.f;

    if (!rival->cooldownListo()) return;

    // 1. PERCEPCIÓN
    rival->percibirJugador(jugador->getPosX(),
                           jugador->getPosY(),
                           jugador->getVelocidadX());

    // 2. RAZONAMIENTO — solo ataca si tiene sentido
    if (!rival->estimarImpacto(G)) return;

    // 3. ACCIÓN
    rival->lanzarBanana();
}
void Nivel2::actualizarBananas(float dt) {
    for (Proyectil* p : rival->getBananas()) {
        if (!p->estaActiva()) continue;

        p->calcularPosVuelo(dt, G);

        // ── La banana tocó el suelo ────────────────────────────────────────
        if (p->getPosY() <= SUELO_Y) {
            // 4. APRENDIZAJE: error = donde cayó la banana - donde está el jugador
            float error = p->getPosX() - jugador->getPosX();
            rival->guardarErrorImpacto(error);
            p->setActiva(false);
            continue;
        }

        // ── Colisión banana → jugador (AABB simplificada) ─────────────────
        float dx = std::abs(p->getPosX() - jugador->getPosX());
        float dy = std::abs(p->getPosY() - jugador->getPosY());
        if (dx < MOTO_W && dy < MOTO_H) {
            // Penalización: reducir velocidad máxima del jugador 10%
            float nuevaVMax = jugador->getVelocidadMax() * 0.90f;
            if (nuevaVMax < 5.f) nuevaVMax = 5.f;   // mínimo para que pueda moverse
            jugador->setVelocidadMax(nuevaVMax);

            // El error aquí es 0 (impacto exacto)
            rival->guardarErrorImpacto(0.f);
            p->setActiva(false);
        }
    }
}
void Nivel2::reciclarObstaculos(float camaraXLogica){
    // Borde izquierdo visible + margen de gracia
    float bordeIzq = camaraXLogica - MARGEN_RECICLADO;

    // Cuánto hay delante de la cámara para reposicionar
    float bordeDerechoVisible = camaraXLogica + SCENE_W_LOGICA;

    for (Obstaculo* o : obstaculos) {
        // Si el obstáculo salió completamente por la izquierda
        if ((o->getPosX() + o->getAncho()) < bordeIzq) {
            o->setPosX(bordeDerechoVisible + SCENE_W_LOGICA*0.85f);
        }
    }
}
void Nivel2::verificarColisiones(float dt) {
    for (Obstaculo* o : obstaculos) {
        if (o->colisiona(jugador, MOTO_W, MOTO_H))
            o->aplicarEfecto(jugador, dt);
    }
}
void Nivel2::verificarSuelo() {
    if (!jugador->getEnSuelo() && jugador->getPosY() <= SUELO_Y) {
        jugador->setPosY(SUELO_Y);
        jugador->evaluarAterrizaje();
    }
}
