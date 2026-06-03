#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil
{
protected:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;

public:
    Proyectil();
    Proyectil(float X,float Y,float velX,float velY);
    virtual ~Proyectil() = default;
    virtual void calcularVelnicial(float velxRival, float angulo) = 0;
    virtual void calcularPosVuelo(float dt,float g) = 0;
};

#endif // PROYECTIL_H
