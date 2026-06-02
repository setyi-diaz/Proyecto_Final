#include "mainwindow.h"
#include "nivel1.h"
#include "jugadornivel1.h"
#include "menupersonalidades.h"
#include <QKeyEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      nivel(nullptr),
      menu(nullptr)
{
    setWindowTitle("Carrera de Simios");
    setFixedSize(720, 880);

    vista = new QGraphicsView(this);
    vista->setFixedSize(700, 860);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(vista);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    menu = new MenuPersonalidades(this);
    connect(menu, &MenuPersonalidades::juegoIniciado, this, &MainWindow::iniciarJuego);
    vista->setScene(menu);
}

void MainWindow::iniciarJuego() {
    nivel = new Nivel1(3, 2, this);
    vista->setScene(nivel);
    nivel->iniciar();
    setFocus();
}

void MainWindow::volverAlMenu() {}

void MainWindow::ajustarVista() {}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!nivel) return;

    JugadorNivel1 *j = nivel->getJugador();
    if (!j) return;

    switch (event->key()) {
    case Qt::Key_W: nivel->aumentarVelocidad(); break;
    case Qt::Key_S: nivel->disminuirVelocidad(); break;
    case Qt::Key_A:
        if (j->getCarrilActual() > 0)
            j->cambiarCarril(j->getCarrilActual() - 1);
        break;
    case Qt::Key_D:
        if (j->getCarrilActual() < nivel->getNumCarriles() - 1)
            j->cambiarCarril(j->getCarrilActual() + 1);
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
}
