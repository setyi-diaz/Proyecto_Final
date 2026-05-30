#ifndef PISTA_H
#define PISTA_H

#include <QGraphicsRectItem>

class Pista : public QGraphicsRectItem {
public:
    Pista(short numCarriles, short longitud, QGraphicsItem *parent = nullptr);

    short obtenerCarril(int indice) const;
    bool carrilValido(int indice) const;
    short getNumCarriles() const;
    short getLongitud() const;

private:
    short numCarriles;
    short longitud;
};

#endif // PISTA_H
