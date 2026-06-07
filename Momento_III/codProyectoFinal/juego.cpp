#include "juego.h"

Juego::Juego(QObject *parent)
    : QObject(parent)
    , menu(nullptr)
    , ventanaNivel1(nullptr)
    , ventanaNivel2(nullptr)
    , puntuacionTotal(0)
    , nivel1Completado(false)
    , nivel2Completado(false)
{
    menu          = new MenuWidget();
    ventanaNivel1 = new MainWindowNivel1();
    ventanaNivel2 = new MainWindowNivel2();

    conectarSignals();
}

Juego::~Juego()
{
    limpiarVentanas();
}

void Juego::iniciar()
{
    puntuacionTotal  = 0;
    nivel1Completado = false;
    nivel2Completado = false;
    mostrarMenu();
}

void Juego::mostrarMenu()
{
    if (ventanaNivel1) { ventanaNivel1->liberarRecursos(); ventanaNivel1->hide(); }
    if (ventanaNivel2) { ventanaNivel2->liberarRecursos(); ventanaNivel2->hide(); }
    if (menu) menu->showMaximized();
}

void Juego::mostrarNivel1()
{
    if (menu) menu->hide();
    if (ventanaNivel2) { ventanaNivel2->liberarRecursos(); ventanaNivel2->hide(); }
    if (ventanaNivel1) ventanaNivel1->showMaximized();
}

void Juego::mostrarNivel2()
{
    if (menu) menu->hide();
    if (ventanaNivel1) { ventanaNivel1->liberarRecursos(); ventanaNivel1->hide(); }
    if (ventanaNivel2) {
        ventanaNivel2->resetearAlMenu();
        ventanaNivel2->showMaximized();
    }
}

void Juego::conectarSignals()
{
    connect(menu, &MenuWidget::nivel1Seleccionado, this, &Juego::onNivel1Seleccionado);
    connect(menu, &MenuWidget::nivel2Seleccionado, this, &Juego::onNivel2Seleccionado);

    connect(ventanaNivel1, &MainWindowNivel1::volverAlMenu, this, &Juego::onVolverAlMenuDesdeNivel1);
    connect(ventanaNivel1, &MainWindowNivel1::irAlNivel2,   this, &Juego::onIrAlNivel2DesdeNivel1);
    connect(ventanaNivel2, &MainWindowNivel2::volverAlMenu,         this, &Juego::onVolverAlMenuDesdeNivel2);
    connect(ventanaNivel2, &MainWindowNivel2::volverAlMenuPrincipal, this, &Juego::onVolverAlMenuDesdeNivel2);
}

void Juego::limpiarVentanas()
{
    if (menu)          { menu->close();          delete menu;          menu          = nullptr; }
    if (ventanaNivel1) { ventanaNivel1->close();  delete ventanaNivel1; ventanaNivel1 = nullptr; }
    if (ventanaNivel2) { ventanaNivel2->close();  delete ventanaNivel2; ventanaNivel2 = nullptr; }
}

void Juego::onNivel1Seleccionado() { mostrarNivel1(); }
void Juego::onNivel2Seleccionado() { mostrarNivel2(); }
void Juego::onVolverAlMenuDesdeNivel1() { mostrarMenu(); }
void Juego::onVolverAlMenuDesdeNivel2() { mostrarMenu(); }
void Juego::onIrAlNivel2DesdeNivel1()  { mostrarNivel2(); }
