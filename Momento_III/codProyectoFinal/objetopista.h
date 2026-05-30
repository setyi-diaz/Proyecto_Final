#ifndef OBJETOPISTA_H
#define OBJETOPISTA_H

#include <QGraphicsEllipseItem>

class ObjetoPista : public QGraphicsEllipseItem {
public:
    ObjetoPista(unsigned short posX, unsigned short posY, QGraphicsItem *parent = nullptr);
    virtual ~ObjetoPista();

    unsigned short getPosX() const;
    unsigned short getPosY() const;

protected:
    unsigned short posX;
    unsigned short posY;
};

#endif // OBJETOPISTA_H
