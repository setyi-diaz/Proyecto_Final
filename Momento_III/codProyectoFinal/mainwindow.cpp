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

    timer = new QTimer(this);
    timer->setInterval(16);   // ~60 fps
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);

    // ── Crear sprites del jugador ─────────────────────────────────────────────
    //  Segundo argumento = número de frames de la hoja.
    //  Ajusta ese número cuando conozcas cuántos frames tiene cada imagen.
    //
    //  GUÍA — si la imagen es estática (1 frame) pon 1.
    //         Si es una hoja horizontal de N cuadros pon N.
    // spriteJugadorNeutro  = new Sprite(":/imagenes/jugador_neutro.png",  1);
    // spriteJugadorAcelera = new Sprite(":/imagenes/jugador_acelera.png", 1);
    // spriteJugadorFrena   = new Sprite(":/imagenes/jugador_frena.png",   1);
    // spriteJugadorAire    = new Sprite(":/imagenes/jugador_aire_neutro.png", 1);
    spriteJugadorNeutro  = new Sprite(":/imagenes/SpritesJugador-sf.png", 1);
    spriteJugadorAcelera = new Sprite(":/imagenes/SpritesJugador-sf.png", 1);
    spriteJugadorFrena   = new Sprite(":/imagenes/SpritesJugador-sf.png", 1);
    spriteJugadorAire    = new Sprite(":/imagenes/SpritesJugador-sf.png", 1);

    // Estado inicial
    spriteJugadorActivo = spriteJugadorNeutro;
    fondoOrigin    = nullptr;
    fondoDesfasado = nullptr;
    fondoOffsetX   = 0;
    nivel = new Nivel2();
    setEscena(2);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete spriteJugador;
    delete spriteJugadorNeutro;
    delete spriteJugadorAcelera;
    delete spriteJugadorFrena;
    delete spriteJugadorAire;
    delete nivel;
    delete ui;
}
QPixmap MainWindow::frameEscalado(Sprite* s, int wPx, int hPx) const
{
    return s->getPixmap()
    ->copy(s->getRectFrame().toRect())
        .scaled(wPx, hPx, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}
void MainWindow::setEscena(short newNumEscena){
    numeroEscena = newNumEscena;
    if(newNumEscena != 0){
        delete escena;
    }
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, SCENE_W, SCENE_H);

    ui->graphicsView->setScene(escena);
    ui->graphicsView->setFixedSize(
        SCENE_W + 2 * ui->graphicsView->frameWidth(),
        SCENE_H + 2 * ui->graphicsView->frameWidth());

    switch (numeroEscena) {
    case 0: { /* escena inicio — pendiente */ break; }
    case 1: { /* nivel 1      — pendiente */ break; }

    case 2:
    {
        reloj.start();
        timer->start();
        // carga del fondo
        QPixmap fondoPix(":/imagenes/fondo.jpeg");
        if(!fondoPix.isNull()){
            QPixmap tile = fondoPix.scaled(SCENE_W, SCENE_H,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            fondoOrigin = escena->addPixmap(tile);
            fondoOrigin->setPos(0, 0);
            fondoOrigin->setZValue(-2);
            fondoDesfasado = escena->addPixmap(tile);
            fondoDesfasado->setPos(SCENE_W, 0);  // derecha, fuera de pantalla
            fondoDesfasado->setZValue(-2);
        }

        // Plataforma
        escena->addRect(0, SUELO_PX, SCENE_W, 4,
                        QPen(Qt::NoPen), QBrush(QColor(80, 200, 80)));

        // ── Jugador: QGraphicsPixmapItem con imagen ───────────────────────
        QPixmap pmMoto = frameEscalado(spriteJugadorActivo, MOTO_W_PX, MOTO_H_PX);
        itemMoto = escena->addPixmap(pmMoto);

        // ── Rival: aún sin imagen → rectángulo azul ───────────────────────
        //  GUÍA — cuando tengas rival_neutro.png:
        //    spriteRivalNeutro = new Sprite(":/imagenes/rival_neutro.png", N);
        //    QPixmap pmRival   = frameEscalado(spriteRivalNeutro, MOTO_W_PX, MOTO_H_PX);
        //    itemRival = escena->addPixmap(pmRival);
        //  Por ahora usamos un pixmap de color sólido:
        {
            QPixmap pmRival(MOTO_W_PX, MOTO_H_PX);
            pmRival.fill(QColor(60, 120, 220));
            itemRival = escena->addPixmap(pmRival);
        }

        // ── Banana: aún sin imagen → pixmap amarillo ──────────────────────
        //  GUÍA — cuando tengas banana.png:
        //    spriteBananaNeutro = new Sprite(":/imagenes/banana.png", N);
        //    QPixmap pmBanana   = frameEscalado(spriteBananaNeutro, 20, 20);
        //    itemBanana = escena->addPixmap(pmBanana);
        {
            QPixmap pmBanana(20, 20);
            pmBanana.fill(QColor(255, 220, 0));
            itemBanana = escena->addPixmap(pmBanana);
        }
        itemBanana->setVisible(false);

        // ── Obstáculos: aún sin imagen → rectángulos de color ────────────
        //  GUÍA — cuando tengas imagenes para rampa/lodo:
        //    Cambia obstaculosGraficos a vector<QGraphicsPixmapItem*>,
        //    crea un Sprite* por tipo de obstáculo y usa addPixmap().
        const QColor coloresObs[] = { QColor(200, 120, 30), QColor(90, 55, 20) };
        int idx = 0;
        for (const Obstaculo* o : nivel->getObstaculos()) {
            float w_px = o->getAncho() * ESCALA;
            float h_px = o->getAlto()  * ESCALA;
            QGraphicsRectItem* rect =
                escena->addRect(0, 0, w_px, h_px,
                                QPen(Qt::NoPen),
                                QBrush(coloresObs[idx % 2]));
            rect->setPos(logicaAPantallaX(o->getPosX()),
                         logicaAPantallaY(o->getPosY() + o->getAlto()));
            obstaculosGraficos.push_back(rect);
            ++idx;
        }
        break;
    }
    default: break;
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
    if (numeroEscena == 0) return;
    if (numeroEscena != 2) return;

    float dt = reloj.elapsed() / 1000.f;
    reloj.restart();
    if (dt > 0.1f) dt = 0.1f;

    // 1. Lógica
    nivel->dinamicaNivel(dt, acelerando, frenando, rotando);
    // scroll de fondo
    if (fondoOrigin && fondoDesfasado) {
        fondoOffsetX -= velocidadObstaculos;
        if (fondoOffsetX >= SCENE_W) fondoOffsetX -= SCENE_W;
        fondoOrigin->setPos(fondoOffsetX,0);
        fondoDesfasado->setPos(fondoOffsetX - SCENE_W, 0);
    }
    // 2. Estado del jugador
    const Jugador* j = nivel->getJugador();

    // ── Selección del sprite activo según estado ──────────────────────────
    Sprite* nuevoSprite = spriteJugadorNeutro;   // fallback
    if (!j->getEnSuelo())
        nuevoSprite = spriteJugadorAire;
    else if (acelerando)
        nuevoSprite = spriteJugadorAcelera;
    else if (frenando)
        nuevoSprite = spriteJugadorFrena;

    // Si cambió de estado, reinicia la animación del nuevo sprite
    if (nuevoSprite != spriteJugadorActivo) {
        nuevoSprite->reiniciar();
        spriteJugadorActivo = nuevoSprite;
    }

    // Avanza el frame del sprite activo (timer global lo controla)
    spriteJugadorActivo->avanzarFrame();

    // Actualiza el pixmap del ítem
    itemMoto->setPixmap(frameEscalado(spriteJugadorActivo, MOTO_W_PX, MOTO_H_PX));

    // 3. Cámara
    camaraX = j->getPosX() * ESCALA - SCENE_W * 0.33f;
    if (camaraX < 0) camaraX = 0;

    // 4. Posición y rotación del jugador
    itemMoto->setPos(logicaAPantallaX(j->getPosX()),
                     logicaAPantallaY(j->getPosY()));
    itemMoto->setRotation(-j->getAnguloSprite());

    // 5. Rival
    //  GUÍA — cuando tengas imagen del rival, agrega aquí:
    //    spriteRivalNeutro->avanzarFrame();
    //    itemRival->setPixmap(frameEscalado(spriteRivalNeutro, MOTO_W_PX, MOTO_H_PX));
    const Rival* r = nivel->getRival();
    itemRival->setPos(logicaAPantallaX(r->getPosX()),
                      logicaAPantallaY(r->getPosY()));

    // 6. Banana
    //  GUÍA — cuando tengas imagen de la banana, agrega aquí:
    //    spriteBananaNeutro->avanzarFrame();
    //    itemBanana->setPixmap(frameEscalado(spriteBananaNeutro, 20, 20));
    const Proyectil* b = r->getBanana();
    if (b->getActiva()) {
        itemBanana->setPos(logicaAPantallaX(b->getPosX()),
                           logicaAPantallaY(b->getPosY()));
        itemBanana->setVisible(true);
    } else {
        itemBanana->setVisible(false);
    }

    // 7. Obstáculos
    //  GUÍA — cuando tengas imágenes de obstáculos, itera sobre
    //  un vector<QGraphicsPixmapItem*> y llama setPixmap() + setPos().
    const auto& obs = nivel->getObstaculos();
    for (size_t i = 0; i < obs.size(); ++i) {
        obstaculosGraficos[i]->setPos(
            logicaAPantallaX(obs[i]->getPosX()),
            logicaAPantallaY(obs[i]->getPosY() + obs[i]->getAlto()));
    }

    // 8. HUD
    ui->vel_label->setText(
        QString("Velocidad: %1 m/s").arg(j->getVelocidadX(), 0, 'f', 2));
    ui->pos_label->setText(
        QString("Posición:  %1 m").arg(j->getPosX(), 0, 'f', 2));
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
