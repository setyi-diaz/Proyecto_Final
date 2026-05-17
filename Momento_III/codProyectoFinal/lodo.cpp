#include "lodo.h"

Lodo::Lodo() {}

void Lodo::generarFriccion(float& vx, float dt){
    vx -= (friccion * dt);
}
