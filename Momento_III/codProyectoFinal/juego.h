#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include "menuwidget.h"
#include "mainwindownivel1.h"
#include "mainwindownivel2.h"

class Juego : public QObject
{
    Q_OBJECT
public:
    explicit Juego(QObject *parent = nullptr);
    ~Juego();

    void iniciar();
    void mostrarMenu();
    void mostrarNivel1();
    void mostrarNivel2();

    int  getPuntuacion()      const { return puntuacionTotal; }
    bool isNivel1Completado() const { return nivel1Completado; }
    bool isNivel2Completado() const { return nivel2Completado; }

private slots:
    void onNivel1Seleccionado();
    void onNivel2Seleccionado();
    void onVolverAlMenuDesdeNivel1();
    void onVolverAlMenuDesdeNivel2();
    void onIrAlNivel2DesdeNivel1();

private:
    void conectarSignals();
    void limpiarVentanas();

    MenuWidget        *menu;
    MainWindowNivel1  *ventanaNivel1;
    MainWindowNivel2  *ventanaNivel2;

    int  puntuacionTotal;
    bool nivel1Completado;
    bool nivel2Completado;
};

#endif // JUEGO_H
