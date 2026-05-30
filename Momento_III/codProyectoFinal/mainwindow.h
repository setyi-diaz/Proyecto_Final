#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

class Nivel1;
class MenuPersonalidades;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void iniciarJuego();
    void volverAlMenu();

private:
    void ajustarVista();

    QGraphicsView *vista;
    Nivel1 *nivel;
    MenuPersonalidades *menu;
};

#endif // MAINWINDOW_H
