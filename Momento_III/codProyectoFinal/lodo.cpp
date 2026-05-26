#include "lodo.h"

Lodo::Lodo(float f, float x, float y, float w, float h) {
    friccion = f;
    posX = x; posY = y; ancho = w; alto = h;
}

void Lodo::generarFriccion(float& vx, float dt){
    vx -= (friccion * dt);
}
