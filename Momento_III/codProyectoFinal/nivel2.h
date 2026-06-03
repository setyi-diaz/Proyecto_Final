#ifndef NIVEL2_H
#define NIVEL2_H
#include "jugador.h"
#include "rival.h"
#include "obstaculo.h"
#include <vector>
using std::vector;

class Nivel2
{
private:
    Jugador* jugador;
    Rival* rival;
    vector<Obstaculo*> obstaculos;

    // Temporizador acumulado para evaluar el agente cada 2 s
    float timerAgente;
    static constexpr float INTERVALO_AGENTE = 2.0f;

    static constexpr float G = 9.8f;

    void verificarColisiones(float dt);
    void verificarSuelo();
    void actualizarBananas(float dt);
    void actualizarRival(float dt);

public:
    static constexpr float SUELO_Y   = 0.f;  // Y del piso en escena
    static constexpr float MOTO_W    = 3.75f;
    static constexpr float MOTO_H    = 1.875f;
    static constexpr float umbralEsquive = 12.5;
    static constexpr float MARGEN_RECICLADO = 20.f;  // unidades lógicas fuera de pantalla
    static constexpr float SCENE_W_LOGICA   = 80.f;  // SCENE_W / ESCALA = 1280/16
    static constexpr float umbralVentaja = 9.375f;
    static constexpr float ajuste = 1.875f;

    Nivel2();
    ~Nivel2();

    void dinamicaNivel(float dt, bool acelerando, bool frenando, bool rotando);
    void reciclarObstaculos(float camaraXLogica);
    const Jugador* getJugador() const { return jugador; }
    const Rival* getRival() const { return rival; }
    const vector<Obstaculo*>& getObstaculos() const { return obstaculos;}
};

#endif // NIVEL2_H
