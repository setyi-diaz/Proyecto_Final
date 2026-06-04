#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QElapsedTimer>
#include <vector>
#include <nivel2.h>
#include "sprite.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setEscena(short numEscena);
private slots:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void actualizar();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escena = nullptr;
    // objetos graficos
    QGraphicsPixmapItem* itemMoto    = nullptr;   // jugador
    QGraphicsPixmapItem* itemRival   = nullptr;   // rival  (sin imagen aún → ver guía)
    QGraphicsPixmapItem* itemBanana  = nullptr;   // banana (sin imagen aún → ver guía)
    std::vector<QGraphicsRectItem*> obstaculosGraficos;  // obstáculos (sin imagen aún)
    // sprites
    //  JUGADOR — estados disponibles en pictures.qrc:
    //    ":/imagenes/jugador_neutro.png"       1 frame  → en suelo, sin input
    //    ":/imagenes/jugador_acelera.png"      ? frames → tecla D presionada
    //    ":/imagenes/jugador_frena.png"        ? frames → tecla A presionada
    //    ":/imagenes/jugador_aire_neutro.png"  ? frames → en vuelo sin rotar
    //
    //  El puntero spriteJugador apunta al Sprite activo en cada momento;
    //  se reasigna en actualizar() según el estado del jugador.
    //
    Sprite* spriteJugador = nullptr;
    Sprite* spriteJugadorActivo   = nullptr;
    Sprite* spriteJugadorNeutro   = nullptr;
    Sprite* spriteJugadorAcelera  = nullptr;
    Sprite* spriteJugadorFrena    = nullptr;
    Sprite* spriteJugadorAire     = nullptr;
    // ── GUÍA — cómo agregar sprites de nuevos objetos ────────────────────────
    //
    //  1. Agrega la imagen al archivo pictures.qrc en Qt Creator
    //     (clic derecho sobre el prefijo "/" → "Add Files").
    //     Ruta recomendada: imagenes/<objeto>_<estado>.png
    //
    //  2. Declara aquí el/los puntero(s) Sprite* para ese objeto, por ejemplo:
    //       Sprite* spriteRivalNeutro  = nullptr;
    //       Sprite* spriteBananaNeutro = nullptr;
    //       Sprite* spriteLodoNeutro   = nullptr;
    //       Sprite* spriteRampaNeutro  = nullptr;
    //
    //  3. Declara el QGraphicsPixmapItem* correspondiente
    //     (reemplaza el QGraphicsRectItem* que hay actualmente):
    //       QGraphicsPixmapItem* itemRival  = nullptr;   // ya declarado arriba
    //       QGraphicsPixmapItem* itemBanana = nullptr;   // ya declarado arriba
    //     Para obstáculos en el vector, cambia el tipo de obstaculosGraficos.
    //
    //  4. En setEscena() (case 2):
    //       spriteRivalNeutro = new Sprite(":/imagenes/rival_neutro.png", N_FRAMES);
    //       QPixmap pmRival   = spriteRivalNeutro->getPixmap()->copy(
    //                               spriteRivalNeutro->getRectFrame().toRect());
    //       itemRival = escena->addPixmap(pmRival.scaled(MOTO_W_PX, MOTO_H_PX));
    //
    //  5. En actualizar(), sección del objeto:
    //       spriteRivalNeutro->avanzarFrame();
    //       QPixmap pmR = spriteRivalNeutro->getPixmap()->copy(
    //                         spriteRivalNeutro->getRectFrame().toRect())
    //                         .scaled(MOTO_W_PX, MOTO_H_PX);
    //       itemRival->setPixmap(pmR);
    //       itemRival->setPos(logicaAPantallaX(r->getPosX()),
    //                         logicaAPantallaY(r->getPosY()));
    //
    //  6. En el destructor (~MainWindow) agrega:
    //       delete spriteRivalNeutro;
    // lógica
    Nivel2* nivel;
    //vector<QGraphicsRectItem*> obstaculosGraficos;
    QGraphicsPixmapItem *fondoOrigin;
    QGraphicsPixmapItem *fondoDesfasado;
    float fondoOffsetX;
    bool acelerando = false;
    bool frenando = false;
    bool rotando = false;
    QTimer* timer = nullptr;
    QElapsedTimer reloj;

    static constexpr float ESCALA  = 16.f;  // px por unidad lógica
    static constexpr float SCENE_W = 1600.f;
    static constexpr float SCENE_H = 900.f;
    static constexpr float SUELO_PX = 160.f; // Y del piso en píxeles
    static constexpr float MOTO_W_PX = 60.f;
    static constexpr float MOTO_H_PX = 30.f;
    static constexpr float velocidadObstaculos = 30.f;

    float camaraX = 0.f;
    short numeroEscena;

    float logicaAPantallaX(float posLogicaX) const;
    float logicaAPantallaY(float posLogicaY) const;
    QPixmap frameEscalado(Sprite* s, int wPx, int hPx) const;
};
#endif // MAINWINDOW_H
