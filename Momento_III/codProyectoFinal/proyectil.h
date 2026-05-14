#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil
{
private:
    short velocidadX;
    short velocidadY;
    short impulso;
    short impulsoVertical;

public:
    Proyectil();
    virtual void calcularVelnicial() = 0;
    virtual void calcularPosVuelo() = 0;
};

#endif // PROYECTIL_H
