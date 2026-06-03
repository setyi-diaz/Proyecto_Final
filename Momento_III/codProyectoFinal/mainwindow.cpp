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
    connect(nivel, &Nivel1::reiniciarSolicitado, this, [this]() {
        delete nivel;
        nivel = nullptr;
        iniciarJuego();
    });
    connect(nivel, &Nivel1::volverMenuSolicitado, this, &MainWindow::volverAlMenu);
    vista->setScene(nivel);
    nivel->iniciar();
    setFocus();
}

void MainWindow::volverAlMenu() {
    if (nivel) {
        delete nivel;
        nivel = nullptr;
    }
    vista->setScene(menu);
    setFocus();
}

void MainWindow::ajustarVista() {}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!nivel) return;

    JugadorNivel1 *j = nivel->getJugador();
    if (!j) return;

    switch (event->key()) {
    case Qt::Key_W: nivel->aumentarVelocidad(); break;
    case Qt::Key_S: nivel->disminuirVelocidad(); break;
    case Qt::Key_A:
        if (j->getCarrilActual() > 0 && nivel->carrilLibre(j->getCarrilActual() - 1))
            j->cambiarCarril(j->getCarrilActual() - 1);
        break;
    case Qt::Key_D:
        if (j->getCarrilActual() < nivel->getNumCarriles() - 1 &&
            nivel->carrilLibre(j->getCarrilActual() + 1))
            j->cambiarCarril(j->getCarrilActual() + 1);
        break;
    case Qt::Key_J:
        nivel->golpearRival(-1);
        break;
    case Qt::Key_K:
        nivel->golpearRival(+1);
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
