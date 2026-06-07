#ifndef RIVALNIVEL2_H
#define RIVALNIVEL2_H
#include "personaje.h"
#include "proyectil.h"
#include <deque>
using std::deque;

class RivalNivel2 : public Personaje
{
private:
    Proyectil* banana;
    float cooldownAtaque;

    float cooldownBase = 2.0f;
    static constexpr float UMBRAL_VENTAJA = 9.375f;

    float percepcionPosXJugador = 0.f;
    float percepcionPosYJugador = 0.f;
    float percepcionVelXJugador = 0.f;

    float anguloEstimado        = 0.f;
    float tiempoVueloEstimado   = 0.f;

    // Aprendizaje por zonas de distancia
    struct ZonaAprendizaje {
        deque<float> historial;
        float        correccion = 0.f;
    };
    int maxHistorial = 5;
    static constexpr float ZONA_CORTA_MAX  = 30.f;
    static constexpr float ZONA_MEDIA_MAX  = 60.f;

    ZonaAprendizaje zonaCorta;
    ZonaAprendizaje zonaMedia;
    ZonaAprendizaje zonaLarga;

    ZonaAprendizaje& zonaParaDistancia(float distancia);
    const ZonaAprendizaje& zonaParaDistancia(float distancia) const;

    float calcularAnguloOptimo(float distancia, float velBanana, float g) const;

public:
    // Atributos de física propios del Nivel2
    float velocidadX  = 0.f;
    float velocidadY  = 0.f;
    bool  enSuelo     = true;

    void acelerar(float dt) override;
    void actualizarPosicion(float dt);

    float getVelocidadX() const { return velocidadX; }
    void  setVelocidadX(float v){ velocidadX = v; }
    bool  getEnSuelo()    const { return enSuelo; }
    void  setEnSuelo(bool v)    { enSuelo = v; }

    RivalNivel2();
    ~RivalNivel2();

    void percibirJugador(float posXJ, float posYJ, float velXJ);
    bool estimarImpacto(float g = 9.8f);
    void lanzarBanana();
    void guardarErrorImpacto(float error);
    void ajustarVelocidad(float velxJ);
    void  actualizarCoolDown(float dt);
    bool  cooldownListo() const { return cooldownAtaque <= 0.f; }
    void  setCooldownBase(float c) { cooldownBase = c; }
    int   getMaxHistorial()        const { return maxHistorial; }
    void  setMaxHistorial(int n)         { maxHistorial = n; }

    Proyectil*       getBanana()       { return banana; }
    const Proyectil* getBanana() const { return banana; }

    float getAnguloEstimado()  const { return anguloEstimado; }

    // Animación de ataque
    float tiempoAnimacionAtaque = 0.f;
    bool  bananaLanzada = false;
    bool  estaAtacando()        const { return tiempoAnimacionAtaque > 0.f; }
    int   getFrameAtaque()      const;
    void  actualizarAnimacion(float dt);
    bool  debeLanzarBananaNow() const { return tiempoAnimacionAtaque <= 0.3f && !bananaLanzada; }
};

#endif // RIVALNIVEL2_H
