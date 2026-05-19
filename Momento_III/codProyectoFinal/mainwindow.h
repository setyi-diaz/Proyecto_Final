#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QElapsedTimer>
#include "jugador.h"

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

private slots:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void actualizarFrame();

private:
    Ui::MainWindow *ui;
    Jugador motoJugador;
    QTimer *timer;
    QElapsedTimer reloj;
    QGraphicsScene *escena;
    QGraphicsRectItem *spriteMovRect;
    QGraphicsRectItem * plataforma;
    bool acelerando = false;
    bool frenando = false;
    static constexpr float SCENE_W   = 800.f;  // ancho lógico de la escena
    static constexpr float SCENE_H   = 200.f;
    static constexpr float MOTO_W    = 60.f;
    static constexpr float MOTO_H    = 30.f;
    static constexpr float SUELO_Y   = 150.f;  // Y del piso en escena
    static constexpr float ESCALA    = 4.f;    // píxeles por unidad de juego
};
#endif // MAINWINDOW_H
