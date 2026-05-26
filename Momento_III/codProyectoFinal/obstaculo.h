#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <cmath>

class Obstaculo
{
private:
    float posX;
    float posY;
    float ancho;
    float alto;
public:
    Obstaculo();
    float getPosX() const;
    float getPosY() const;
    float getAncho() const;
    float getAlto() const;
    void setPosX(float newPosX);
    void setPosY(float newPosY);
};

#endif // OBSTACULO_H
