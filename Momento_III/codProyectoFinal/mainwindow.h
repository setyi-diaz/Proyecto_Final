#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QElapsedTimer>
#include <vector>
#include <nivel2.h>

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
    QGraphicsScene *escena;
    QGraphicsRectItem *spriteMoto;
    QGraphicsRectItem *spriteRival;
    QGraphicsRectItem *spriteBanana;

    Nivel2* nivel;
    vector<QGraphicsRectItem*> obstaculosGraficos;
    bool acelerando = false;
    bool frenando = false;
    bool rotando = false;
    QTimer* timer;
    QElapsedTimer reloj;

    static constexpr float ESCALA  = 16.f;  // px por unidad lógica
    static constexpr float SCENE_W = 1280.f;
    static constexpr float SCENE_H = 720.f;
    static constexpr float SUELO_PX = 160.f; // Y del piso en píxeles
    static constexpr float MOTO_W_PX = 60.f;
    static constexpr float MOTO_H_PX = 30.f;

    float camaraX = 0.f;
    short numeroEscena;

    float logicaAPantallaX(float posLogicaX) const;
    float logicaAPantallaY(float posLogicaY) const;
};
#endif // MAINWINDOW_H
