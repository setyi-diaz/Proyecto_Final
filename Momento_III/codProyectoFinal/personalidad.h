#ifndef PERSONALIDAD_H
#define PERSONALIDAD_H

#include <QString>

class Personalidad {
public:
    Personalidad(const QString &nombre, float modAceleracion, float modVelMax,
                 float resistenciaGolpe, float fuerzaGolpe);

    // Sobrecarga de operadores (requisito de diseño)
    Personalidad& operator=(const Personalidad &otro);
    bool operator==(const Personalidad &otro) const;
    bool operator!=(const Personalidad &otro) const;

    QString getNombre() const;
    float getModAceleracion() const;
    float getModVelMax() const;
    float getResistenciaGolpe() const;
    float getFuerzaGolpe() const;

private:
    QString nombre;
    float modAceleracion;
    float modVelMax;
    float resistenciaGolpe;
    float fuerzaGolpe;
};

#endif // PERSONALIDAD_H
