#ifndef ITEMBANANA_H
#define ITEMBANANA_H
#include "item.h"

class ItemBanana : public Item {
public:
    ItemBanana(float posX, float posY, int carril, float impulso);

    float getEfecto() const override { return impulso; }
    bool  esImpulso() const override { return impulso > 0.f; }
    bool  esAgua()    const override { return false; }
    int   getTipo()   const override { return 3; }
    void  setImpulso(float v)        { impulso = v; }

private:
    float impulso;
};

#endif // ITEMBANANA_H
