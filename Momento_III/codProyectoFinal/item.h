#ifndef ITEM_H
#define ITEM_H

class Item {
public:
    Item(float posX, float posY, int carril);
    virtual ~Item() = default;

    virtual float getEfecto() const = 0;
    virtual bool  esImpulso() const = 0;
    virtual bool  esAgua()    const = 0;
    virtual int   getTipo()   const = 0;

    int   getCarril()  const { return carril; }
    float getPosX()    const { return posX; }
    float getPosY()    const { return posY; }
    void  setPosX(float x)   { posX = x; }
    void  setPosY(float y)   { posY = y; }

    bool puedeColisionar() const { return cooldown <= 0; }
    void activarCooldown(float duracion) { cooldown = duracion; }
    void actualizarCooldown(float dt) {
        if (cooldown > 0) cooldown -= dt;
    }

protected:
    float posX;
    float posY;
    int   carril;
    float cooldown;
};

#endif // ITEM_H
