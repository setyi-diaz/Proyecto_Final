#include "rival.h"
#include "banana.h"
#include <cmath>
#include <numeric>   // std::accumulate
static constexpr float PI = 3.14159265f;
static constexpr float DEG_A_RAD = PI / 180.f;
static constexpr float VEL_BANANA = 15.f;

Rival::Rival()
    : cooldownAtaque(0.f), percepcionPosXJugador(0.f), percepcionPosYJugador(0.f)
    , percepcionVelXJugador(0.f), anguloEstimado(45.f * DEG_A_RAD)
    , tiempoVueloEstimado(0.f), correccionAcumulada(0.f)
{
    banana = new Banana();
}

Rival::~Rival() {
    delete banana;          // un solo delete en el destructor
}

void Rival::percibirJugador(float posXJ, float posYJ, float velXJ) {
    percepcionPosXJugador = posXJ;
    percepcionPosYJugador = posYJ;
    percepcionVelXJugador = velXJ;
}

float Rival::calcularAnguloOptimo(float distancia, float velBanana, float g) const {
    if (velBanana <= 0.f) return 45.f * DEG_A_RAD;

    float sinDoble = (distancia * g) / (velBanana * velBanana);
    if ((sinDoble < -1.f) || (sinDoble > 1.f))
        return 45.f * DEG_A_RAD;   // fuera de rango alcanzable

    // El ángulo menor (más bajo y rápido) favorece que la banana llegue pronto
    float dobleAngulo = std::asin(sinDoble);
    return dobleAngulo / 2.f;      // θ = arcsin(sin2θ) / 2
}

bool Rival::estimarImpacto(float g) {
    // --- condición de ventaja: el rival está adelante (o muy cerca) ---
    float distancia = percepcionPosXJugador - posX;
    if (distancia >= UMBRAL_VENTAJA)    // jugador demasiado lejos adelante → no atacar
        return false;
    if (distancia < 0.f)                // rival ya adelante → tampoco (empuja hacia atrás)
        return false;

    // --- distancia con predicción: el jugador sigue moviéndose ---
    // Estimamos el tiempo de vuelo con la distancia actual como primera aproximación
    float velLanzamiento = velocidadX + VEL_BANANA;
    if (velLanzamiento <= 0.f) return false;

    // Primera estimación del tiempo de vuelo (tiro a 45°)
    float t1 = distancia / (velLanzamiento * std::cos(45.f * DEG_A_RAD));
    // Posición predicha del jugador al llegar la banana
    float distanciaPredicha = distancia + percepcionVelXJugador * t1;
    if (distanciaPredicha <= 0.f) return false;

    // Ángulo óptimo para alcanzar la posición predicha
    float angulo = calcularAnguloOptimo(distanciaPredicha, velLanzamiento, g);

    // Aplica la corrección aprendida (en radianes)
    angulo += correccionAcumulada;
    // Clamp: 15° – 75°
    const float MIN_ANG = 15.f * DEG_A_RAD;
    const float MAX_ANG = 75.f * DEG_A_RAD;
    if (angulo < MIN_ANG) angulo = MIN_ANG;
    if (angulo > MAX_ANG) angulo = MAX_ANG;

    anguloEstimado = angulo;

    // Tiempo de vuelo con el ángulo final (para diagnóstico y para guardar)
    float cosA = std::cos(anguloEstimado);
    tiempoVueloEstimado = (cosA > 0.001f)
                              ? distanciaPredicha / (velLanzamiento * cosA)
                              : 0.f;
    return true;
}

void Rival::lanzarBanana() {
    banana->setPosX(posX);
    banana->setPosY(posY + 1.f);
    float velLanzamiento = velocidadX + VEL_BANANA;
    banana->calcularVelnicial(velLanzamiento, anguloEstimado);
    banana->setActiva(true);

    cooldownAtaque = COOLDOWN_BASE;
}

void Rival::guardarErrorImpacto(float error) {
    historialErrores.push_back(error);
    if ((int)historialErrores.size() > MAX_HISTORIAL)
        historialErrores.pop_front();

    // Promedio de errores recientes
    float suma = std::accumulate(historialErrores.begin(), historialErrores.end(), 0.f);
    float promedio = suma / (float)historialErrores.size();

    correccionAcumulada -= (promedio / 10.f) * DEG_A_RAD;

    // Clamp de la corrección para evitar desviaciones extremas (±20°)
    const float MAX_CORR = 20.f * DEG_A_RAD;
    if (correccionAcumulada >  MAX_CORR) correccionAcumulada =  MAX_CORR;
    if (correccionAcumulada < -MAX_CORR) correccionAcumulada = -MAX_CORR;
}

void Rival::ajustarVelocidad(float velxJ) {
    velocidadX = velxJ;
}

void Rival::actualizarCoolDown(float dt) {
    if (cooldownAtaque > 0.f)
        cooldownAtaque -= dt;
    if (cooldownAtaque < 0.f)
        cooldownAtaque = 0.f;
}
