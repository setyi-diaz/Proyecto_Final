#include "personalidad.h"

Personalidad::Personalidad(const QString &nombre, float modAceleracion, float modVelMax,
                           float resistenciaGolpe, float fuerzaGolpe)
    : nombre(nombre),
      modAceleracion(modAceleracion),
      modVelMax(modVelMax),
      resistenciaGolpe(resistenciaGolpe),
      fuerzaGolpe(fuerzaGolpe)
{}

void Personalidad::aplicarAtributos() {}

QString Personalidad::getNombre() const { return QString(); }
float Personalidad::getModAceleracion() const { return 0.0f; }
float Personalidad::getModVelMax() const { return 0.0f; }
float Personalidad::getResistenciaGolpe() const { return 0.0f; }
float Personalidad::getFuerzaGolpe() const { return 0.0f; }
