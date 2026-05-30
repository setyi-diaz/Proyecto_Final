#ifndef JUGADORNIVEL1_H
#define JUGADORNIVEL1_H

#include "corredornivel1.h"
#include "personalidad.h"

class JugadorNivel1 : public CorredorNivel1 {
public:
    JugadorNivel1(QGraphicsItem *parent = nullptr);

    void acelerar() override;
    void frenar() override;
    void golpearIzquierda();
    void golpearDerecha();
    void aplicarPersonalidad();

    void setPersonalidad(Personalidad *p);
    bool isGolpeDisponible() const;

private:
    Personalidad *personalidad;
    bool golpeDisponible;
};

#endif // JUGADORNIVEL1_H
