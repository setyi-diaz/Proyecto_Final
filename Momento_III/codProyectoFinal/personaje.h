#ifndef PERSONAJE_H
#define PERSONAJE_H

class Personaje {
public:
    Personaje();
    virtual ~Personaje() = default;

    virtual void acelerar(float dt) = 0;

    float getPosX() const;
    float getPosY() const;
    void  setPosX(float x);
    void  setPosY(float y);
    float getVelocidadMax() const;
    void  setVelocidadMax(float v);
    float getAceleracion() const;
    void  setAceleracion(float a);

protected:
    float posX        = 0.f;
    float posY        = 0.f;
    float aceleracion = 40.f;
    float velocidadMax = 150.f;
};

#endif // PERSONAJE_H
