#ifndef RIVALNIVEL1_H
#define RIVALNIVEL1_H

#include "corredornivel1.h"

// Forward declaration - la definición está en nivel1.h
enum class DificultadNivel1;

class JugadorNivel1;

class RivalNivel1 : public CorredorNivel1 {
public:
    RivalNivel1();
    void acelerar(float dt) override;
    void frenar(float dt)   override;
    bool tomarDecision(JugadorNivel1 *jugador, float dt);
    void atacarJugador(JugadorNivel1 *jugador);
    void actualizarIA(float dt);
    void setNumCarriles(int n);
    void setDificultad(DificultadNivel1 d);

protected:
    QString rutaSprite() const override;

private:
    // Métodos de IA
    void decidir(float posJugador, float posRival, int carrilJugador, int carrilRival,
                 DificultadNivel1 dificultad);
    bool  debeAcelerar()      const;
    bool  debeAtacar()        const;
    bool  debeCambiarCarril() const;
    int   carrilObjetivo()    const;
    void  registrarFallo();
    void  registrarAcierto();
    void  evaluarAprendizaje(bool enRangoAhora);
    float getRangoAtaque()    const;

    // Atributos de IA
    bool  acelerar_           = false;
    bool  atacar_             = false;
    bool  cambiarCarril_      = false;
    int   carrilObj_          = 0;
    float rangoAtaque_        = 80.0f;
    int   fallosConsecutivos_ = 0;
    bool  enRangoAnterior_    = false;

    // Atributos del rival
    float      cooldownAtaque;
    float      tiempoCooldown;
    float      tiempoCambioCarril;
    int        numCarriles;
    DificultadNivel1 dificultad;
};

#endif // RIVALNIVEL1_H
