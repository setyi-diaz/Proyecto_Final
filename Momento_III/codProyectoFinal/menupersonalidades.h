#ifndef MENUPERSONALIDADES_H
#define MENUPERSONALIDADES_H

#include <QGraphicsScene>
#include <QList>
#include <QColor>
#include "personalidad.h"
#include "rivalnivel1.h"

class QGraphicsTextItem;
class QGraphicsRectItem;

class MenuPersonalidades : public QGraphicsScene {
    Q_OBJECT
public:
    explicit MenuPersonalidades(QObject *parent = nullptr);
    ~MenuPersonalidades();

    Personalidad* getPersonalidadJugador() const;
    QList<Personalidad*> getPersonalidadesRivales() const;
    int getNumCarriles() const;
    int getNumRivales() const;
    Dificultad getDificultad() const;

signals:
    void juegoIniciado();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void crearPersonalidades();
    void reconstruirUI();
    void construirConfigBar();
    void crearTarjeta(int slot, float y, float tarjetaH);
    void actualizarTarjeta(int slot);
    void actualizarMensajeLateral();

    QList<Personalidad*> personalidades;

    int numCarriles;   // 4–10
    int numRivales;    // 2–7
    Dificultad dificultadSeleccionada;
    int indices[8];    // índice de personalidad por slot (0=jugador, 1-7=rivales)

    // Botones dificultad
    QGraphicsRectItem *btnFacil;
    QGraphicsRectItem *btnMedio;
    QGraphicsRectItem *btnDificil;

    // Config bar items
    QGraphicsRectItem *btnCarrilMenos;
    QGraphicsRectItem *btnCarrilMas;
    QGraphicsTextItem *textoCarriles;
    QGraphicsRectItem *btnRivalMenos;
    QGraphicsRectItem *btnRivalMas;
    QGraphicsTextItem *textoRivales;

    // Mensaje lateral (espacio libre cuando hay pocos carriles)
    QGraphicsTextItem *textoLateral;

    struct Tarjeta {
        float y = 0;
        QGraphicsTextItem *textoNombre = nullptr;
        QGraphicsRectItem *barras[4]   = {};
        QGraphicsTextItem *textosBarras[4] = {};
        QGraphicsRectItem *btnIzq = nullptr;
        QGraphicsRectItem *btnDer = nullptr;
    };
    static const int MAX_SLOTS = 8;
    Tarjeta tarjetas[MAX_SLOTS];

    QGraphicsRectItem *btnIniciar;
    QGraphicsTextItem *lblStart;
};

#endif // MENUPERSONALIDADES_H
