#ifndef PERSONAJE_H
#define PERSONAJE_H

class Personaje
{
private:
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
    float aceleracion;
    float velocidadMax;
    bool enSuelo;
    float anguloSprite;
    float velAlDespegue;
public:
    Personaje();

    void acelerar(float dt);
    void aplicarGravedad(float dt,float g);
    void actualizarPosicion(float );
    void movParabolico(float dt,float g);

    float getPosX() const;
    void setPosX(float newPosX);
    float getPosY() const;
    void setPosY(float newPosY);
    float getVelocidadX() const;
    void setVelocidadX(float newVelocidadX);
    float getVelocidadY() const;
    void setVelocidadY(float newVelocidadY);
    float getAceleracion() const;

    float getVelocidadMax() const;
    void setVelocidadMax(float newVelocidadMax);
    float getAnguloSprite() const;
    void setAnguloSprite(float newAnguloSprite);
    bool getEnSuelo() const;
    void setEnSuelo(bool newEnSuelo);
    void setAceleracion(float newAceleracion);
};

#endif // PERSONAJE_H
