#ifndef PERSONAJE_H
#define PERSONAJE_H

class Personaje
{
private:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
    float aceleracion;
    float velocidadMax;
    bool enSuelo;
    float anguloSprite;
    float velAlDespegue;
public:
    virtual void acelerar() = 0; // funcion virtual pura
    void aplicarGravedad(float );
    void actualizarPosicion(float );
    Personaje();
};

#endif // PERSONAJE_H
