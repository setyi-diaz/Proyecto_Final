#ifndef SPRITE_H
#define SPRITE_H

#include <QString>
#include <QPixmap>
#include <QRectF>

/*
    Clase Sprite — clase de datos pura (sin herencia Qt)

    Gestiona la hoja de sprites y el avance de frames.
    No posee timer propio: el timer global de MainWindow llama
    avanzarFrame() en cada tick, evitando un QTimer por instancia.
    El dibujado lo realiza MainWindow con getPixmap() y getRectFrame().
*/
class Sprite
{
private:
    QPixmap* pixmap;
    int      frameActual;
    int      totalFrames;
    float    ancho;   // ancho de un frame = pixmap->width() / totalFrames
    float    alto;    // alto  de un frame = pixmap->height()

public:
    // Carga la imagen y calcula el tamaño de cada frame.
    Sprite(const QString& rutaImagen, int cantidadFrames);

    // Libera el pixmap reservado en heap.
    ~Sprite();

    // Avanza al siguiente frame; vuelve a 0 al terminar la secuencia.
    // Debe llamarse desde el timer global (MainWindow::actualizar).
    void avanzarFrame();

    // Reinicia la animación al frame 0.
    void reiniciar();

    // Devuelve el recorte (x, y, ancho, alto) del frame actual
    // dentro de la hoja, listo para usar en drawPixmap / drawTiledPixmap.
    QRectF getRectFrame() const;

    // Acceso al pixmap completo para pasarlo a QPainter.
    const QPixmap* getPixmap() const;

    // Dimensiones de un frame individual.
    float getAncho() const;
    float getAlto()  const;

    int getFrameActual() const;
    int getTotalFrames() const;
};

#endif // SPRITE_H
