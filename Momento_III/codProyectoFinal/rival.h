#ifndef RIVAL_H
#define RIVAL_H
#include "personaje.h"
#include "proyectil.h"
#include <list>
#include <deque>
using std::list;
using std::deque;

class Rival : public Personaje
{
private:
    Proyectil* banana;
    float cooldownAtaque;

    static constexpr float COOLDOWN_BASE  = 2.0f;
    static constexpr float UMBRAL_VENTAJA = 9.375f;

    float percepcionPosXJugador;
    float percepcionPosYJugador;
    float percepcionVelXJugador;

    float anguloEstimado;
    float tiempoVueloEstimado;

    deque<float> historialErrores;
    static constexpr int MAX_HISTORIAL = 5;
    float correccionAcumulada;

    float calcularAnguloOptimo(float distancia, float velBanana, float g) const;
public:
    Rival();
    ~Rival();
    // PERCEPCION
    void percibirJugador(float posXJ, float posYJ, float velXJ);
    // RAZONAMIENTO
    bool estimarImpacto(float g = 9.8f);
    // ACCION
    void lanzarBanana();
    // APRENDIZAJE
    void guardarErrorImpacto(float error);
    // ajustes del rival para que el juego sea jugable
    void ajustarVelocidad(float velxJ);
    void actualizarCoolDown(float dt);
    bool cooldownListo() const { return cooldownAtaque <= 0.f; }

    Proyectil* getBanana() { return banana; }
    const Proyectil* getBanana() const { return banana; }

    float getCorreccion() const { return correccionAcumulada; }
    float getAnguloEstimado() const { return anguloEstimado; }
};

#endif // RIVAL_H
