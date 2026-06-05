#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsRectItem>

// Clase base para todos los corredores.
// La gravedad NO se define aqui — se agrega en subclases de niveles que la necesiten.
class Personaje : public QGraphicsRectItem {
public:
    Personaje(QGraphicsItem *parent = nullptr);
    virtual ~Personaje();

    virtual void acelerar() = 0;

    float getPosX() const;
    float getPosY() const;
    void  setPosX(float x);
    void  setPosY(float y);

protected:
    float posX;
    float posY;
    float velocidadY;
    float aceleracion;
    float velocidadMax;
};

#endif // PERSONAJE_H
