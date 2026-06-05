#include "personalidad.h"

Personalidad::Personalidad(const QString &nombre, float modAceleracion, float modVelMax,
                           float resistenciaGolpe, float fuerzaGolpe)
    : nombre(nombre),
      modAceleracion(modAceleracion),
      modVelMax(modVelMax),
      resistenciaGolpe(resistenciaGolpe),
      fuerzaGolpe(fuerzaGolpe)
{}

Personalidad& Personalidad::operator=(const Personalidad &otro) {
    if (this != &otro) {
        nombre           = otro.nombre;
        modAceleracion   = otro.modAceleracion;
        modVelMax        = otro.modVelMax;
        resistenciaGolpe = otro.resistenciaGolpe;
        fuerzaGolpe      = otro.fuerzaGolpe;
    }
    return *this;
}

bool Personalidad::operator==(const Personalidad &otro) const {
    return nombre == otro.nombre;
}

bool Personalidad::operator!=(const Personalidad &otro) const {
    return !(*this == otro);
}

QString Personalidad::getNombre() const { return nombre; }
float Personalidad::getModAceleracion() const { return modAceleracion; }
float Personalidad::getModVelMax() const { return modVelMax; }
float Personalidad::getResistenciaGolpe() const { return resistenciaGolpe; }
float Personalidad::getFuerzaGolpe() const { return fuerzaGolpe; }
