#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "jugadornivel2.h"
#include "rivalnivel2.h"
#include "obstaculo.h"
#include <vector>
using std::vector;

enum class DificultadNivel2 { NORMAL, DIFICIL };

class Nivel2 : public Nivel
{
    Q_OBJECT
public:
    explicit Nivel2(QObject *parent = nullptr);
    ~Nivel2();

    void iniciar()                              override;
    void actualizar(float dt)                   override;
    void procesarTecla(int key, bool presionada) override;
    void setDificultad(DificultadNivel2 d);
    DificultadNivel2 getDificultad() const { return dificultad; }

    const JugadorNivel2*       getJugador()    const { return jugador; }
    const RivalNivel2*         getRival()      const { return rival; }
    const vector<Obstaculo*>&  getObstaculos() const { return obstaculos; }

    static constexpr float SUELO_Y         = 0.f;
    static constexpr float MOTO_W          = 3.75f;
    static constexpr float MOTO_H          = 1.875f;
    static constexpr float umbralEsquive   = 60.f;
    static constexpr float MARGEN_RECICLADO = 20.f;
    static constexpr float SCENE_W_LOGICA  = 80.f;
    static constexpr float umbralVentaja   = 30.f;
    static constexpr float META_X          = 5000.f;

signals:
    void sonidoRampa();
    void sonidoBanana();
    void sonidoLodo();
    void impactoBanana(float velocidadPerdida);
    void bonoVelocidad(float velocidadGanada);
    void caidaEspalda();

private:
    static constexpr float G               = 80.f;
    static constexpr float INTERVALO_AGENTE = 2.0f;

    JugadorNivel2*     jugador;
    RivalNivel2*       rival;
    vector<Obstaculo*> obstaculos;
    float              timerAgente = 0.f;

    bool acelerando    = false;
    bool frenando      = false;
    bool rotando       = false;
    bool enLodoAnterior = false;

    DificultadNivel2 dificultad;

    void actualizarBananas(float dt);
    void actualizarRival(float dt);
    void reciclarObstaculos(float camaraXLogica);
    void verificarVictoriaDerrota();
};

#endif // NIVEL2_H
