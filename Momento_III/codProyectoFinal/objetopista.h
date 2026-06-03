#ifndef OBJETOPISTA_H
#define OBJETOPISTA_H

#include <QGraphicsEllipseItem>

class ObjetoPista : public QGraphicsEllipseItem {
public:
    ObjetoPista(float efecto, float posY, int carril = -1,
                float margenX = 50, float anchoCarril = 100,
                QGraphicsItem *parent = nullptr);

    float getEfecto() const;
    bool esImpulso() const;
    int getCarril() const;

    bool puedeColisionar() const;
    void activarCooldown(float duracion);
    void actualizarCooldown(float dt);

private:
    float efecto;
    int carril;
    float cooldown;
};

#endif // OBJETOPISTA_H
