#include "corredornivel1.h"
#include <QBrush>
#include <QPen>
#include <QColor>

CorredorNivel1::CorredorNivel1(QGraphicsItem *parent)
    : Personaje(parent),
      carrilActual(1),
      resistenciaGolpe(1.0f),
      fuerzaGolpe(50.0f),
      tiempoDesaceleracion(0),
      tiempoGolpeRecibido(0),
      sprite(nullptr),
      spriteItem(nullptr),
      targetW(60), targetH(80),
      spriteGolpe(0)
{
    setZValue(0.0);
    setTransformOriginPoint(0, 0);
}

CorredorNivel1::~CorredorNivel1() { delete sprite; }

void CorredorNivel1::cargarSprite(const QString &ruta, int cols, int filas) {
    delete sprite;
    sprite = new Sprite(ruta, cols, filas);
    if (!sprite->esValido()) { delete sprite; sprite = nullptr; return; }
    if (!spriteItem) {
        setBrush(Qt::NoBrush);
        setPen(QPen(Qt::NoPen));
        spriteItem = new QGraphicsPixmapItem(this);
    }
    setSpriteIndex(0);
}

void CorredorNivel1::setSpriteTargetSize(int w, int h) {
    targetW = w; targetH = h;
    if (sprite && spriteItem) setSpriteIndex(sprite->getFrameActual());
}

void CorredorNivel1::setSpriteIndex(int index) {
    if (!sprite || !spriteItem) return;
    sprite->setFrame(index);
    QPixmap frame = sprite->getFrameScaled(targetW, targetH);
    spriteItem->setPixmap(frame);
    spriteItem->setOffset(-frame.width() / 2.0f, -frame.height() / 2.0f);
    setTransformOriginPoint(0, 0);
}

void CorredorNivel1::activarGolpeAnimacion(float duracion, int indiceSpriteGolpe) {
    tiempoGolpeRecibido = duracion;
    spriteGolpe = indiceSpriteGolpe;
    if (spriteItem) setSpriteIndex(indiceSpriteGolpe);
}

void CorredorNivel1::actualizarAnimaciones(float dt) {
    if (tiempoGolpeRecibido > 0) {
        tiempoGolpeRecibido -= dt;
        if (spriteItem) {
            int idx = (int)(tiempoGolpeRecibido * 10) % 2 == 0 ? spriteGolpe : 0;
            setSpriteIndex(idx);
        } else {
            setBrush(QBrush((int)(tiempoGolpeRecibido * 15) % 2 == 0
                            ? QColor(255, 200, 0) : colorBase));
        }
        if (tiempoGolpeRecibido <= 0) {
            if (!spriteItem) setBrush(QBrush(colorBase));
            else setSpriteIndex(0);
        }
        return;
    }
    if (tiempoDesaceleracion > 0) {
        tiempoDesaceleracion -= dt;
        if (!spriteItem)
            setBrush(QBrush((int)(tiempoDesaceleracion * 10) % 2 == 0
                            ? QColor(255, 100, 100) : colorBase));
        if (tiempoDesaceleracion <= 0 && !spriteItem)
            setBrush(QBrush(colorBase));
        return;
    }
    if (spriteItem) setSpriteIndex(0);
}

int  CorredorNivel1::getCarrilActual() const { return carrilActual; }
void CorredorNivel1::cambiarCarril(int n)    { carrilActual = n; }

void CorredorNivel1::recibirGolpe(float fuerza) {
    velocidadY -= fuerza / resistenciaGolpe;
    if (velocidadY < 0) velocidadY = 0;
}

void CorredorNivel1::aplicarImpulso(float impulso) {
    velocidadY += impulso;
    if (velocidadY > velocidadMax) velocidadY = velocidadMax;
}

bool CorredorNivel1::estaDesacelerado() const { return tiempoDesaceleracion > 0; }

void CorredorNivel1::activarDesaceleracion(float d) { tiempoDesaceleracion = d; }
void CorredorNivel1::activarGolpeRecibido(float d)  { tiempoGolpeRecibido = d; }

void CorredorNivel1::setColorBase(const QColor &color) {
    colorBase = color;
    setBrush(QBrush(color));
}

float CorredorNivel1::getAceleracion()      const { return aceleracion; }
float CorredorNivel1::getVelocidadMax()     const { return velocidadMax; }
float CorredorNivel1::getFuerzaGolpe()      const { return fuerzaGolpe; }
float CorredorNivel1::getResistenciaGolpe() const { return resistenciaGolpe; }
