#include "personalidad.h"

Personalidad::Personalidad(const QString &nombre, float modAceleracion, float modVelMax,
                           float resistenciaGolpe, float fuerzaGolpe)
    : nombre(nombre),
      modAceleracion(modAceleracion),
      modVelMax(modVelMax),
      resistenciaGolpe(resistenciaGolpe),
      fuerzaGolpe(fuerzaGolpe)
{}

QString Personalidad::getNombre() const { return nombre; }
float Personalidad::getModAceleracion() const { return modAceleracion; }
float Personalidad::getModVelMax() const { return modVelMax; }
float Personalidad::getResistenciaGolpe() const { return resistenciaGolpe; }
float Personalidad::getFuerzaGolpe() const { return fuerzaGolpe; }
