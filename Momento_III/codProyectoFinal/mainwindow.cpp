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
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarFrame);   //connect(detectorSeñal,señal,ventana,respuesta)
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

    if (!acelerando)
        motoJugador.desaceleracionAuto(dt);

    // Clampear velocidad mínima a 0
    if (motoJugador.getVelocidadX() < 0.f)
        motoJugador.setVelocidadX(0.f);

    // Actualizar posición lógica
    motoJugador.actualizarPosicion(dt);

    // Convertir posición lógica → posición en escena (píxeles)
    float posLogica = motoJugador.getPosX();
    // La cámara sigue al jugador, centrado en 1/3 de la pantalla
    camaraX   = (posLogica * ESCALA) - (SCENE_W*0.33f);
    if (camaraX  < 0) camaraX  = 0;
    // La moto se dibuja siempre relativa a la cámara
    float xEscena = posLogica * ESCALA - camaraX;
    spriteMovRect->setPos(xEscena, SUELO_Y - MOTO_H);

    float xObstEnPantalla = obstaculoPosX * ESCALA - camaraX;
    spriteObstaculo->setPos(xObstEnPantalla, yObstaculo);

    bool enLodo = (motoJugador.getPosX() + MOTO_W/ESCALA > lodo.getPosX()) &&
                  (motoJugador.getPosX() < lodo.getPosX() + lodo.getAncho()) &&
                  (motoJugador.getPosY() >= lodo.getPosY());
    if (enLodo) {
        float vx = motoJugador.getVelocidadX();
        lodo.generarFriccion(vx, dt);
        if (vx < 0) vx = 0;
        motoJugador.setVelocidadX(vx);
    }

    bool sobreRampa = (motoJugador.getPosX() + MOTO_W/ESCALA > lodo.getPosX()) &&
                      (motoJugador.getPosX() < lodo.getPosX() + lodo.getAncho()) &&
                      (motoJugador.getPosY() >= lodo.getPosY());

    if (sobreRampa && motoJugador.getEnSuelo()) {
        float vx = motoJugador.getVelocidadX();
        float velDespegue = vx;  // guardar para el bonus

        // La rampa convierte vx horizontal en impulso diagonal
        float vy = vx * rampa.getSenAngulo();   // componente vertical
        vx = vx * rampa.getCosAngulo();         // componente horizontal reducida

        motoJugador.setVelocidadX(vx);
        motoJugador.setVelocidadY(vy);
        motoJugador.setEnSuelo(false);
        // guardar velAlDespegue si quieres implementar el bonus de ángulo
    }
    if (!motoJugador.getEnSuelo()) {
        motoJugador.aplicarGravedad(dt, 9.8f);

        float newPosY = motoJugador.getPosY() + motoJugador.getVelocidadY() * dt;

        if (newPosY <= 0) {  // llegó al suelo (posY=0 es el suelo en coords lógicas)
            motoJugador.setPosY(0);
            motoJugador.setVelocidadY(0);
            motoJugador.setEnSuelo(true);
            motoJugador.evaluarAterrizaje();  // aplica bonus o penalización
        }
    }
    // Actualizar labels
    ui->vel_label->setText(
        QString("Velocidad: %1 m/s").arg(motoJugador.getVelocidadX(), 0, 'f', 2));
    ui->pos_label->setText(
        QString("Posición:  %1 m").arg(posLogica, 0, 'f', 2));
}
