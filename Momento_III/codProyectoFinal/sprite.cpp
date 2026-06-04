#include "sprite.h"

Sprite::Sprite(const QString& rutaImagen, int cantidadFrames)
{
    frameActual = 0;
    totalFrames = (cantidadFrames > 0) ? cantidadFrames : 1;

    // La imagen contiene todos los cuadros uno junto al otro.
    pixmap = new QPixmap(rutaImagen);

    // Cada frame ocupa una fracción horizontal idéntica de la hoja.
    ancho = static_cast<float>(pixmap->width())  / totalFrames;
    alto  = static_cast<float>(pixmap->height());
}

Sprite::~Sprite()
{
    delete pixmap;
}

void Sprite::avanzarFrame()
{
    frameActual++;
    if (frameActual >= totalFrames)
        frameActual = 0;
}

void Sprite::reiniciar()
{
    frameActual = 0;
}

QRectF Sprite::getRectFrame() const
{
    // Devuelve el recorte del frame actual dentro de la hoja.
    return QRectF(frameActual * ancho, 0.f, ancho, alto);
}

const QPixmap* Sprite::getPixmap() const
{
    return pixmap;
}

float Sprite::getAncho() const { return ancho; }
float Sprite::getAlto()  const { return alto;  }

int Sprite::getFrameActual() const { return frameActual; }
int Sprite::getTotalFrames() const { return totalFrames; }
