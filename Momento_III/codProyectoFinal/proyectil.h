#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil
{
protected:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
    bool  activa;

public:
    Proyectil();
    Proyectil(float X,float Y,float velX,float velY);
    virtual ~Proyectil() = default;

    virtual void calcularVelnicial(float velxRival, float angulo) = 0;
    virtual void calcularPosVuelo(float dt,float g) = 0;
    float getPosX() const;
    void setPosX(float newPosX);
    float getPosY() const;
    void setPosY(float newPosY);
    bool getActiva() const;
    void setActiva(bool newActiva);
};

#endif // PROYECTIL_H
