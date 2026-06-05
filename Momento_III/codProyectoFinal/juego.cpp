#include "juego.h"
#include "nivel1.h"
#include "jugadornivel1.h"
#include "menupersonalidades.h"
#include <QMessageBox>
#include <stdexcept>

Juego::Juego(QGraphicsView *vista, QObject *parent)
    : QObject(parent), vista(vista), nivel(nullptr)
{
    menu = new MenuPersonalidades(this);
    connect(menu, &MenuPersonalidades::juegoIniciado, this, &Juego::iniciarNivel);
}

Juego::~Juego() { delete nivel; }

void Juego::mostrarMenu() {
    vista->setScene(menu);
    ajustarVista();
}

void Juego::iniciarNivel() {
    try {
        delete nivel;
        nivel = new Nivel1(3, 2, this);
        connect(nivel, &Nivel1::reiniciarSolicitado,  this, &Juego::reiniciarNivel);
        connect(nivel, &Nivel1::volverMenuSolicitado, this, &Juego::volverAlMenu);
        vista->setScene(nivel);
        ajustarVista();
        nivel->iniciar();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", e.what());
    }
}

void Juego::reiniciarNivel() { iniciarNivel(); }

void Juego::volverAlMenu() {
    delete nivel;
    nivel = nullptr;
    mostrarMenu();
}

void Juego::procesarTecla(int key) {
    if (nivel) nivel->procesarTecla(key);
}

void Juego::ajustarVista() {
    if (vista->scene())
        vista->fitInView(vista->scene()->sceneRect(), Qt::KeepAspectRatio);
}
