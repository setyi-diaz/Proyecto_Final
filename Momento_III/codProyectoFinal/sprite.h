#ifndef SPRITE_H
#define SPRITE_H

#include <QString>
#include <QPixmap>
#include <QRectF>

// Gestiona una hoja de sprites de N columnas x M filas.
// No hereda de ninguna clase Qt — clase de datos pura.
// El dibujado lo realiza el item que la contiene.
class Sprite
{
private:
    QPixmap *pixmap;
    int      frameActual;
    int      totalFrames;
    int      columnas;
    int      filas;
    float    ancho;
    float    alto;

public:
    Sprite(const QString &rutaImagen, int columnas, int filas = 1);
    ~Sprite();

    void avanzarFrame();
    void reiniciar();
    void setFrame(int index);

    QRectF  getRectFrame() const;
    QPixmap getFramePixmap() const;
    QPixmap getFrameScaled(int targetW, int targetH) const;

    const QPixmap *getPixmap() const;
    float getAncho() const;
    float getAlto()  const;
    int   getFrameActual() const;
    int   getTotalFrames() const;
    bool  esValido() const;
};

#endif // SPRITE_H
