#ifndef CORREDORNIVEL1_H
#define CORREDORNIVEL1_H

#include "personaje.h"
#include "sprite.h"
#include <QColor>
#include <QGraphicsPixmapItem>

class CorredorNivel1 : public Personaje {
public:
    CorredorNivel1(QGraphicsItem *parent = nullptr);
    virtual ~CorredorNivel1();

    virtual void acelerar() = 0;
    virtual void frenar() {}
    void actualizarAnimaciones(float dt);

    int  getCarrilActual() const;
    void cambiarCarril(int nuevoCarril);
    void recibirGolpe(float fuerza);
    void aplicarImpulso(float impulso);
    bool estaDesacelerado() const;

    // Efectos visuales temporales
    void activarDesaceleracion(float duracion);
    void activarGolpeRecibido(float duracion);
    void activarGolpeAnimacion(float duracion, int indiceSpriteGolpe);

    void setColorBase(const QColor &color);
    void cargarSprite(const QString &ruta, int cols, int filas = 1);
    void setSpriteIndex(int index);
    void setSpriteTargetSize(int w, int h);

    float getAceleracion() const;
    float getVelocidadMax() const;
    float getFuerzaGolpe() const;
    float getResistenciaGolpe() const;

protected:
    int   carrilActual;
    float resistenciaGolpe;
    float fuerzaGolpe;
    QColor colorBase;

    float tiempoDesaceleracion;
    float tiempoGolpeRecibido;

    Sprite              *sprite;
    QGraphicsPixmapItem *spriteItem;
    int  targetW, targetH;
    int  spriteGolpe;
};

#endif // CORREDORNIVEL1_H
