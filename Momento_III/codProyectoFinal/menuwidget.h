#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class MenuWidget : public QMainWindow {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget() = default;

signals:
    void nivel1Seleccionado();
    void nivel2Seleccionado();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QGraphicsView  *vista  = nullptr;
    QGraphicsScene *escena = nullptr;

    QGraphicsRectItem *btnNivel1 = nullptr;
    QGraphicsRectItem *btnNivel2 = nullptr;

    void construirEscena();
    void ajustarVista();
};

#endif // MENUWIDGET_H
