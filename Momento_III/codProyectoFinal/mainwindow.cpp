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
    // tiempo
    timer = new QTimer(this);
    timer->setInterval(16);   // 16 ms
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);   //connect(emisor,señal,receptor,slot)
    nivel = new Nivel2();
    setEscena(2);

    //nivel = new Nivel2();


    // Conectar vista

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete nivel;
    delete ui;
}
void MainWindow::setEscena(short newNumEscena){
    numeroEscena = newNumEscena;
    if(newNumEscena != 0){
        delete escena;
    }
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, SCENE_W, SCENE_H);

    ui->graphicsView->setScene(escena);
    ui->graphicsView->setFixedSize(SCENE_W + 2*ui->graphicsView->frameWidth(),SCENE_H + 2*ui->graphicsView->frameWidth());

    switch (numeroEscena) {
    case 0:
    {       //crear escena de inicio
        break;
    }
    case 1:
    {       // nivel 1
        break;
    }
    case 2:
    {    // se agregan a la escena, todos los objetos del nivel 2
        reloj.start();
        timer->start();

        escena->setBackgroundBrush(QColor(30, 30, 40));

        // Plataforma
        escena->addRect(0, SUELO_PX, SCENE_W, 4,
                        QPen(Qt::NoPen),
                        QBrush(QColor(80, 200, 80)));

        // Sprite de la moto (rectángulo rojo)
        spriteMoto = escena->addRect(0, 0, MOTO_W_PX, MOTO_H_PX,
                                     QPen(Qt::NoPen),
                                     QBrush(QColor(220, 60, 60)));
        // obstaculos
        for (const Obstaculo* o : nivel->getObstaculos()) {

            float x_px = logicaAPantallaX(o->getPosX());
            float y_px = logicaAPantallaY(o->getPosY());
            float w_px = o->getAncho() * ESCALA;
            float h_px = o->getAlto()  * ESCALA;

            QGraphicsRectItem* rect =
                escena->addRect(
                    0, 0,
                    w_px,
                    h_px,
                    QPen(Qt::NoPen),
                    QBrush(Qt::gray)
                    );

            rect->setPos(x_px, y_px);
            obstaculosGraficos.push_back(rect);
        }
        break;
    }
    default:
        break;
    }
}
float MainWindow::logicaAPantallaX(float x) const {
    return x * ESCALA - camaraX;
}

float MainWindow::logicaAPantallaY(float y) const {
    // posY=0 → SUELO_PX; posY positivo → sube en pantalla (Y invertida en Qt)
    return SUELO_PX - MOTO_H_PX - (y * ESCALA);
}
void MainWindow::actualizar() {
    if(numeroEscena == 0){
        return;
    }
    else if(numeroEscena == 2){

        float dt = reloj.elapsed() / 1000.f;
        reloj.restart();
        if (dt > 0.1f) dt = 0.1f;

        // 1. Actualizar lógica (sin gráficos)
        nivel->dinamicaNivel(dt, acelerando, frenando, rotando);

        // 2. Leer estado lógico y convertir a gráficos
        const Jugador* j = nivel->getJugador();

        camaraX = j->getPosX() * ESCALA - SCENE_W * 0.33f;
        if (camaraX < 0) camaraX = 0;

        spriteMoto->setPos(logicaAPantallaX(j->getPosX()),
                           logicaAPantallaY(j->getPosY()));
        spriteMoto->setRotation(-j->getAnguloSprite()); // negativo: Qt rota horario

        const auto& obs = nivel->getObstaculos();

        for (size_t i = 0; i < obs.size(); ++i) {
            obstaculosGraficos[i]->setPos(
                logicaAPantallaX(obs[i]->getPosX()),
                logicaAPantallaY(obs[i]->getPosY())
                );
        }

        // 3. HUD
        ui->vel_label->setText(
            QString("Velocidad: %1 m/s").arg(j->getVelocidadX(), 0, 'f', 2));
        ui->pos_label->setText(
            QString("Posición:  %1 m").arg(j->getPosX(), 0, 'f', 2));
    }
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
    else if (event->key() == Qt::Key_W) {
        rotando = true;
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
