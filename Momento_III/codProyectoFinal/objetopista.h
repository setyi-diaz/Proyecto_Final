#ifndef OBJETOPISTA_H
#define OBJETOPISTA_H

#include <QGraphicsEllipseItem>

class ObjetoPista : public QGraphicsEllipseItem {
public:
    // efecto > 0 = impulso (banana), efecto < 0 = frenada (lodo/obstaculo)
    // Si carril >= 0 se posiciona automaticamente en ese carril
    ObjetoPista(float efecto, float posY, int carril = -1,
                float margenX = 50, float anchoCarril = 100,
                QGraphicsItem *parent = nullptr);

    float getEfecto() const;
    bool esImpulso() const;
    int getCarril() const;

private:
    float efecto;
    int carril;
};

#endif // OBJETOPISTA_H
