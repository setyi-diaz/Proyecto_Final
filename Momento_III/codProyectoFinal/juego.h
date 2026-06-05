#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include <QGraphicsView>

class Nivel1;
class MenuPersonalidades;

// Clase control: coordina el flujo entre el menu y los niveles.
// No dibuja nada ni conoce la logica del juego.
class Juego : public QObject {
    Q_OBJECT
public:
    explicit Juego(QGraphicsView *vista, QObject *parent = nullptr);
    ~Juego();

    void mostrarMenu();
    void procesarTecla(int key);

private slots:
    void iniciarNivel();
    void reiniciarNivel();
    void volverAlMenu();

private:
    void ajustarVista();

    QGraphicsView      *vista;
    Nivel1             *nivel;
    MenuPersonalidades *menu;
};

#endif // JUEGO_H
