#ifndef JUGADORNIVEL1_H
#define JUGADORNIVEL1_H

#include "corredornivel1.h"

class JugadorNivel1 : public CorredorNivel1 {
public:
    JugadorNivel1();
    void acelerar(float dt) override;
    void frenar(float dt)   override;

protected:
    QString rutaSprite() const override;
};

#endif // JUGADORNIVEL1_H
