#include "itemobstaculo.h"

ItemObstaculo::ItemObstaculo(float posX, float posY, int carril, Tipo tipo, float friccion)
    : Item(posX, posY, carril), tipoObstaculo(tipo), friccion(friccion)
{}
