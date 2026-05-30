#include "mainwindow.h"
#include "nivel1.h"
#include "jugadornivel1.h"
#include "menupersonalidades.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QScreen>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), nivel(nullptr)
{
    setWindowTitle("Carrera de Simios");

    vista = new QGraphicsView(this);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setRenderHint(QPainter::SmoothPixmapTransform);
    vista->setBackgroundBrush(QBrush(QColor(15, 15, 20)));
    setCentralWidget(vista);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    menu = new MenuPersonalidades(this);
    connect(menu, &MenuPersonalidades::juegoIniciado, this, &MainWindow::iniciarJuego);
    vista->setScene(menu);

    showMaximized();
}

void MainWindow::iniciarJuego() {}

void MainWindow::volverAlMenu() {}

void MainWindow::keyPressEvent(QKeyEvent *event) {}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {}

void MainWindow::resizeEvent(QResizeEvent *event) {}

void MainWindow::ajustarVista() {}
