#ifndef RIVALNIVEL1_H
#define RIVALNIVEL1_H

#include "corredornivel1.h"

class JugadorNivel1;
class Personalidad;

enum class Dificultad { FACIL, MEDIO, DIFICIL };

class AgenteInteligente {
public:
    void decidir(float posJugador, float posRival, int carrilJugador, int carrilRival,
                 Dificultad dificultad);
    bool debeAcelerar() const;
    bool debeAtacar() const;
    bool debeCambiarCarril() const;
    int carrilObjetivo() const;

    // Aprendizaje: ajusta rango de ataque según historial de fallos
    void registrarFallo();
    void registrarAcierto();
    void evaluarAprendizaje(bool enRangoAhora);
    float getRangoAtaque() const;

private:
    bool acelerar_ = false;
    bool atacar_ = false;
    bool cambiarCarril_ = false;
    int carrilObj_ = 0;

    float rangoAtaque_ = 80.0f;
    int fallosConsecutivos_ = 0;
    bool enRangoAnterior_ = false;
};

class RivalNivel1 : public CorredorNivel1 {
public:
    RivalNivel1(QGraphicsItem *parent = nullptr);

    void acelerar() override;
    void frenar() override;
    bool tomarDecision(JugadorNivel1 *jugador, float dt);
    void atacarJugador(JugadorNivel1 *jugador);
    void actualizarIA(float dt);
    
    void setPersonalidad(Personalidad *p);
    void aplicarPersonalidad();
    void setNumCarriles(int n);
    void setDificultad(Dificultad d);

private:
    AgenteInteligente agente;
    float cooldownAtaque;
    float tiempoCooldown;
    float tiempoCambioCarril;
    Personalidad *personalidad;
    int numCarriles;
    Dificultad dificultad;
};

#endif // RIVALNIVEL1_H
