#ifndef CORREDORNIVEL1_H
#define CORREDORNIVEL1_H

#include "personaje.h"
#include <QColor>

class CorredorNivel1 : public Personaje {
public:
    CorredorNivel1(QGraphicsItem *parent = nullptr);
    virtual ~CorredorNivel1();

    virtual void acelerar() = 0;
    virtual void frenar() {}
    void actualizarPosicion(float dt);
    void actualizarAnimaciones(float dt);

    int getCarrilActual() const;
    void cambiarCarril(int nuevoCarril);
    void recibirGolpe(float fuerza);
    void aplicarImpulso(float impulso);
    
    bool estaDesacelerado() const;

    void activarDesaceleracion(float duracion);
    void activarGolpeRecibido(float duracion);
    void setColorBase(const QColor &color);

    float getAceleracion() const;
    float getVelocidadMax() const;
    float getFuerzaGolpe() const;
    float getResistenciaGolpe() const;

protected:
    int carrilActual;
    float resistenciaGolpe;
    float fuerzaGolpe;
    QColor colorBase;

    // Animaciones
    float tiempoDesaceleracion;
    float tiempoGolpeRecibido;
};

#endif // CORREDORNIVEL1_H
