#ifndef JUGADORNIVEL1_H
#define JUGADORNIVEL1_H

#include "corredornivel1.h"
#include "personalidad.h"

class JugadorNivel1 : public CorredorNivel1 {
public:
    JugadorNivel1(QGraphicsItem *parent = nullptr);

    void acelerar() override;
    void frenar() override;
    void aplicarPersonalidad();
    void setPersonalidad(Personalidad *p);

private:
    Personalidad *personalidad;
};

#endif // JUGADORNIVEL1_H
