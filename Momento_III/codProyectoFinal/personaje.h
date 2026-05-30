#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsRectItem>

class Personaje : public QGraphicsRectItem {
public:
    Personaje(QGraphicsItem *parent = nullptr);
    virtual ~Personaje();

    virtual void acelerar() = 0;
    void aplicarGravedad(float dt);
    void actualizarPosicion(float dt);

    float getPosX() const;
    float getPosY() const;
    void setPosX(float x);
    void setPosY(float y);
    float getVelocidadX() const;
    float getVelocidadY() const;

protected:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
    float aceleracion;
    float velocidadMax;
    bool enSuelo;
    float anguloGente;
    float velocidadAlDespegue;
};

#endif // PERSONAJE_H

