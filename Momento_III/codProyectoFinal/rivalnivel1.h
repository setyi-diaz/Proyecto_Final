#ifndef RIVALNIVEL1_H
#define RIVALNIVEL1_H

#include "corredornivel1.h"

class JugadorNivel1;
class Personalidad;

class AgenteInteligente {
public:
    void decidir(float posJugador, float posRival, int carrilJugador, int carrilRival);
    bool debeAcelerar() const;
    bool debeAtacar() const;
    bool debeCambiarCarril() const;
    int carrilObjetivo() const;

private:
    bool acelerar_ = false;
    bool atacar_ = false;
    bool cambiarCarril_ = false;
    int carrilObj_ = 0;
};

class RivalNivel1 : public CorredorNivel1 {
public:
    RivalNivel1(QGraphicsItem *parent = nullptr);

    void acelerar() override;
    void frenar() override;
    bool tomarDecision(JugadorNivel1 *jugador); // true si golpeó al jugador
    void atacarJugador(JugadorNivel1 *jugador);
    void actualizarIA();
    
    void setPersonalidad(Personalidad *p);
    void aplicarPersonalidad();
    void setNumCarriles(int n);

private:
    AgenteInteligente agente;
    float cooldownAtaque;
    float tiempoCooldown;
    float tiempoCambioCarril;
    Personalidad *personalidad;
    int numCarriles;
};

#endif // RIVALNIVEL1_H
