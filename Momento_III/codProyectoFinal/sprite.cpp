#include "sprite.h"

Sprite::Sprite(const QString& rutaImagen, int columnas, int filas)
    : frameActual(0),
      columnas(columnas > 0 ? columnas : 1),
      filas(filas > 0 ? filas : 1)
{
    totalFrames = this->columnas * this->filas;
    pixmap = new QPixmap(rutaImagen);
    ancho = static_cast<float>(pixmap->width()) / this->columnas;
    alto  = static_cast<float>(pixmap->height()) / this->filas;
}

Sprite::~Sprite()
{
    delete pixmap;
}

void Sprite::setFrame(int index)
{
    if (index >= 0 && index < totalFrames)
        frameActual = index;
}

QRectF Sprite::getRectFrame() const
{
    int col = frameActual % columnas;
    int row = frameActual / columnas;
    return QRectF(col * ancho, row * alto, ancho, alto);
}

QPixmap Sprite::getFramePixmap() const
{
    QRectF rect = getRectFrame();
    return pixmap->copy(rect.toRect());
}

QPixmap Sprite::getFrameScaled(int targetW, int targetH) const
{
    return getFramePixmap().scaled(targetW, targetH,
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
}

const QPixmap* Sprite::getPixmap() const { return pixmap; }
float Sprite::getAncho() const { return ancho; }
float Sprite::getAlto() const { return alto; }
int Sprite::getFrameActual() const { return frameActual; }
int Sprite::getTotalFrames() const { return totalFrames; }
