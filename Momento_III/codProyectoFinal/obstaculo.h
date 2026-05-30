#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QBrush>

class Obstaculo : public QGraphicsRectItem {
public:
    Obstaculo(int carril, float posY, float margenX = 50, float anchoCarril = 100, QGraphicsItem *parent = nullptr);

    int getCarril() const;
    float getPenalizacion() const;

private:
    int carril;
    float penalizacion;
};

#endif // OBSTACULO_H
