#ifndef SPRITE_H
#define SPRITE_H

#include <QString>
#include <QPixmap>
#include <QRectF>

class Sprite
{
private:
    QPixmap* pixmap;
    int      frameActual;
    int      totalFrames;
    int      columnas;
    int      filas;
    float    ancho;
    float    alto;

public:
    Sprite(const QString& rutaImagen, int columnas, int filas = 1);
    ~Sprite();

    void setFrame(int index);

    QRectF getRectFrame() const;
    QPixmap getFramePixmap() const;
    QPixmap getFrameScaled(int targetW, int targetH) const;

    const QPixmap* getPixmap() const;
    float getAncho() const;
    float getAlto() const;
    int getFrameActual() const;
    int getTotalFrames() const;
};

#endif // SPRITE_H
