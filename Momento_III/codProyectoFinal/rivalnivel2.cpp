#include "rivalnivel2.h"
#include <cmath>
#include <numeric>

static constexpr float PI            = 3.14159265f;
static constexpr float DEG_A_RAD     = PI / 180.f;
static constexpr float IMPULSO_BANANA = 70.f;  // Reducido de 120 a 70 para parábola más pronunciada
static constexpr float IMPULSO_VERTICAL = 300.f;

RivalNivel2::RivalNivel2()
    : cooldownAtaque(5.f)  // cooldown inicial alto — no ataca en los primeros 5s
{
    banana = new Proyectil();
}

RivalNivel2::~RivalNivel2() {
    delete banana;
}

void RivalNivel2::acelerar(float dt) {
    velocidadX += aceleracion * dt;
    if (velocidadX > velocidadMax) velocidadX = velocidadMax;
}

void RivalNivel2::actualizarPosicion(float dt) {
    posX += velocidadX * dt;
    if (!enSuelo) posY += velocidadY * dt;
}

void RivalNivel2::percibirJugador(float posXJ, float posYJ, float velXJ) {
    percepcionPosXJugador = posXJ;
    percepcionPosYJugador = posYJ;
    percepcionVelXJugador = velXJ;
}

float RivalNivel2::calcularAnguloOptimo(float distancia, float velBanana, float g) const {
    if (velBanana <= 0.f) return 45.f * DEG_A_RAD;
    float sinDoble = (distancia * g) / (velBanana * velBanana);
    if (sinDoble < -1.f || sinDoble > 1.f) return 45.f * DEG_A_RAD;
    return std::asin(sinDoble) / 2.f;
}

RivalNivel2::ZonaAprendizaje& RivalNivel2::zonaParaDistancia(float distancia) {
    if (distancia < ZONA_CORTA_MAX) return zonaCorta;
    if (distancia < ZONA_MEDIA_MAX) return zonaMedia;
    return zonaLarga;
}

const RivalNivel2::ZonaAprendizaje& RivalNivel2::zonaParaDistancia(float distancia) const {
    if (distancia < ZONA_CORTA_MAX) return zonaCorta;
    if (distancia < ZONA_MEDIA_MAX) return zonaMedia;
    return zonaLarga;
}

bool RivalNivel2::estimarImpacto(float g) {
    float distancia = percepcionPosXJugador - posX;
    if (distancia < 0.f) return false;

    float velLanzamiento = velocidadX + IMPULSO_BANANA;
    if (velLanzamiento <= 0.f) return false;

    float t1 = distancia / (velLanzamiento * std::cos(45.f * DEG_A_RAD));
    float distanciaPredicha = distancia + percepcionVelXJugador * t1;
    if (distanciaPredicha <= 0.f) return false;

    float angulo = calcularAnguloOptimo(distanciaPredicha, velLanzamiento, g);

    // Usar la corrección específica de la zona de distancia actual
    angulo += zonaParaDistancia(distancia).correccion;

    const float MIN_ANG = 15.f * DEG_A_RAD;
    const float MAX_ANG = 75.f * DEG_A_RAD;
    if (angulo < MIN_ANG) angulo = MIN_ANG;
    if (angulo > MAX_ANG) angulo = MAX_ANG;

    anguloEstimado = angulo;
    float cosA = std::cos(anguloEstimado);
    tiempoVueloEstimado = (cosA > 0.001f)
        ? distanciaPredicha / (velLanzamiento * cosA) : 0.f;
    return true;
}

void RivalNivel2::lanzarBanana() {
    // Solo inicia la animación de ataque, la banana se lanzará después
    tiempoAnimacionAtaque = 0.6f;  // Duración total de animación (2 frames × 0.3s)
    bananaLanzada = false;
    cooldownAtaque = cooldownBase;
}

void RivalNivel2::actualizarAnimacion(float dt) {
    if (tiempoAnimacionAtaque > 0.f) {
        tiempoAnimacionAtaque -= dt;

        // Lanzar la banana cuando empieza el segundo frame (Frame 4)
        if (tiempoAnimacionAtaque <= 0.3f && !bananaLanzada) {
            banana->setPosX(posX);
            banana->setPosY(posY + 1.f);
            banana->calcularVelocidadInicial(velocidadX + IMPULSO_BANANA, anguloEstimado);
            banana->setActiva(true);
            bananaLanzada = true;
        }

        if (tiempoAnimacionAtaque < 0.f) tiempoAnimacionAtaque = 0.f;
    }
}

int RivalNivel2::getFrameAtaque() const {
    if (tiempoAnimacionAtaque <= 0.f) return -1;  // No está atacando

    // Primera mitad de la animación (0.6s a 0.3s) = Frame 3
    if (tiempoAnimacionAtaque > 0.3f) {
        return 3;  // Primer frame de ataque (preparando lanzamiento)
    }
    // Segunda mitad (0.3s a 0s) = Frame 4
    else {
        return 4;  // Segundo frame de ataque (lanzando)
    }
}

void RivalNivel2::guardarErrorImpacto(float error) {
    // Calcular distancia actual al momento del impacto
    float distancia = percepcionPosXJugador - posX;
    ZonaAprendizaje& zona = zonaParaDistancia(distancia);

    // Guardar error en el historial de la zona correspondiente
    zona.historial.push_back(error);
    if ((int)zona.historial.size() > maxHistorial)
        zona.historial.pop_front();

    // Ajustar corrección solo de esta zona
    float suma = std::accumulate(zona.historial.begin(), zona.historial.end(), 0.f);
    float promedio = suma / (float)zona.historial.size();

    // Escalar el ajuste por la distancia: errores en distancias largas
    // requieren correcciones de ángulo menores que en distancias cortas
    float factorEscala = (distancia < 1.f) ? 1.f : 10.f / distancia;
    zona.correccion -= (promedio * factorEscala) * DEG_A_RAD;

    const float MAX_CORR = 20.f * DEG_A_RAD;
    if (zona.correccion >  MAX_CORR) zona.correccion =  MAX_CORR;
    if (zona.correccion < -MAX_CORR) zona.correccion = -MAX_CORR;
}

void RivalNivel2::ajustarVelocidad(float velxJ) {
    velocidadX = velxJ;
}

void RivalNivel2::actualizarCoolDown(float dt) {
    if (cooldownAtaque > 0.f) cooldownAtaque -= dt;
    if (cooldownAtaque < 0.f) cooldownAtaque = 0.f;
}
