#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Configurar jugador
    motoJugador.setAceleracion(20.f);
    motoJugador.setVelocidadMax(50.f);
    motoJugador.setPosX(0.f);

    // Escena gráfica
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, SCENE_W, SCENE_H);
    escena->setBackgroundBrush(QColor(30, 30, 40));

    // Plataforma
    plataforma = escena->addRect(0, SUELO_Y, SCENE_W, 4,
                           QPen(Qt::NoPen),
                           QBrush(QColor(80, 200, 80)));

    // Sprite de la moto (rectángulo rojo)
    spriteMovRect = escena->addRect(0, 0, MOTO_W, MOTO_H,
                                 QPen(Qt::NoPen),
                                 QBrush(QColor(220, 60, 60)));

    // Posición inicial: encima del suelo
    spriteMovRect->setPos(0, SUELO_Y - MOTO_H);

    // Conectar vista
    ui->graphicsView->setScene(escena);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);

    // Labels iniciales
    ui->vel_label->setText("Velocidad: 0.00 m/s");
    ui->pos_label->setText("Posición:  0.00 m");

    // Botones

    // tiempo
    timer = new QTimer(this);
    timer->setInterval(16);   // ~60 fps
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarFrame);
    reloj.start();
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
 // Crontoles
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    if (event->key() == Qt::Key_D) {
        acelerando = true;
    }
    else if (event->key() == Qt::Key_A) {
        frenando = true;
    }
    else
        QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    if (event->key() == Qt::Key_D) {
        acelerando = false;
    }
    else if (event->key() == Qt::Key_A) {
        frenando = false;
    }
    else
        QMainWindow::keyReleaseEvent(event);
}
// simulacion
void MainWindow::actualizarFrame()
{
    // dt en segundos
    float dt = reloj.elapsed() / 1000.f;
    reloj.restart();
    if (dt > 0.1f) dt = 0.1f;   // evitar saltos si la app se congela

    // Física
    if (acelerando)
        motoJugador.acelerar(dt);

    if (frenando)
        motoJugador.frenar(dt);// usa rozamiento del Jugador

    // Clampear velocidad mínima a 0
    if (motoJugador.getVelocidadX() < 0.f)
        motoJugador.setVelocidadX(0.f);

    // Actualizar posición lógica
    motoJugador.actualizarPosicion(dt);

    // Convertir posición lógica → posición en escena (píxeles)
    // La moto rebota al llegar al borde derecho (efecto de "pista circular")
    float posLogica = motoJugador.getPosX();
    float xEscena   = fmodf(posLogica * ESCALA, SCENE_W - MOTO_W);
    if (xEscena < 0) xEscena = 0;

    spriteMovRect->setPos(xEscena, SUELO_Y - MOTO_H);

    // Actualizar labels
    ui->vel_label->setText(
        QString("Velocidad: %1 m/s").arg(motoJugador.getVelocidadX(), 0, 'f', 2));
    ui->pos_label->setText(
        QString("Posición:  %1 m").arg(posLogica, 0, 'f', 2));
}
