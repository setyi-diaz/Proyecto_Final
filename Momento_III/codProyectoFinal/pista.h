#ifndef PISTA_H
#define PISTA_H

#include <QGraphicsRectItem>

class Pista : public QGraphicsRectItem {
public:
    Pista(short numCarriles, short longitud, float anchoCarril = 100, QGraphicsItem *parent = nullptr);

    short getNumCarriles() const;

private:
    short numCarriles;
    short longitud;
};

#endif // PISTA_H
