#ifndef ITEMOBSTACULO_H
#define ITEMOBSTACULO_H
#include "item.h"

class ItemObstaculo : public Item {
public:
    enum Tipo { LODO = 0, TRONCO = 1, CHARCO = 2 };

    ItemObstaculo(float posX, float posY, int carril, Tipo tipo, float friccion);

    float getEfecto() const override { return -friccion; }
    bool  esImpulso() const override { return false; }
    bool  esAgua()    const override { return tipoObstaculo == CHARCO; }
    int   getTipo()   const override { return static_cast<int>(tipoObstaculo); }

private:
    Tipo  tipoObstaculo;
    float friccion;
};

#endif // ITEMOBSTACULO_H
