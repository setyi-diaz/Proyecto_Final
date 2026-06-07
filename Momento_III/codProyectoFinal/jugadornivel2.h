#ifndef JUGADORNIVEL2_H
#define JUGADORNIVEL2_H
#include "personaje.h"

class JugadorNivel2 : public Personaje
{
private:
    float friccionFreno = 50.f;
    float friccionSuelo = 20.f;
    float velocidadRot  = 180.f;
    float anguloRot     = 0.f;

    void aplicarBonus();

public:
    JugadorNivel2();
    JugadorNivel2(float fricFreno, float fricSuelo, float velRot, float angRot);

    void acelerar(float dt) override;
    void frenar(float dt);
    void desaceleracionAuto(float dt);
    void inclinar(float dt);
    void reiniciarRotacion();

    // Atributos de física 2D propios del Nivel2
    float velocidadX    = 0.f;
    float velocidadY    = 0.f;
    bool  enSuelo       = true;
    float anguloSprite  = 0.f;
    float velAlDespegue = 0.f;

    void  actualizarPosicion(float dt);
    void  aplicarGravedad(float dt, float g);

    enum class ResultadoAterrizaje { BONUS, PENALIZACION_LEVE, PENALIZACION_ESPALDA };
    ResultadoAterrizaje evaluarAterrizaje();

    float getVelocidadX()    const { return velocidadX; }
    void  setVelocidadX(float v)   { velocidadX = v; }
    float getVelocidadY()    const { return velocidadY; }
    void  setVelocidadY(float v)   { velocidadY = v; }
    bool  getEnSuelo()       const { return enSuelo; }
    void  setEnSuelo(bool v)       { enSuelo = v; }
    float getAnguloSprite()  const { return anguloSprite; }
    void  setAnguloSprite(float v) { anguloSprite = v; }
    float getVelAlDespegue() const { return velAlDespegue; }
    void  setVelAlDespegue(float v){ velAlDespegue = v; }
};

#endif // JUGADORNIVEL2_H
