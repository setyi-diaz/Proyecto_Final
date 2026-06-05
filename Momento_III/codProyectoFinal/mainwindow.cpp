#include "mainwindow.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Carrera de Simios");
    setFixedSize(720, 880);

    vista = new QGraphicsView(this);
    vista->setFixedSize(700, 860);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->setRenderHint(QPainter::SmoothPixmapTransform);
    setCentralWidget(vista);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    juego = new Juego(vista, this);
    juego->mostrarMenu();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Escape: close(); return;
    default: break;
    }
    juego->procesarTecla(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (vista->scene())
        vista->fitInView(vista->scene()->sceneRect(), Qt::KeepAspectRatio);
}
