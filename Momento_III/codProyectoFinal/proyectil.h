#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil
{
private:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
    bool  activa;

public:
    Proyectil();
    Proyectil(float X, float Y, float velX, float velY);

    void calcularVelocidadInicial(float velxRival, float angulo);
    void actualizarPosicion(float dt, float g);

    float getPosX() const;
    void  setPosX(float newPosX);
    float getPosY() const;
    void  setPosY(float newPosY);
    bool  getActiva() const;
    void  setActiva(bool newActiva);
};

#endif // PROYECTIL_H
