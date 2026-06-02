#include "menupersonalidades.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>

MenuPersonalidades::MenuPersonalidades(QObject *parent)
    : QGraphicsScene(parent),
      numCarriles(3),
      numRivales(2),
      dificultadSeleccionada(Dificultad::FACIL),
      btnFacil(nullptr), btnMedio(nullptr), btnDificil(nullptr),
      btnCarrilMenos(nullptr), btnCarrilMas(nullptr), textoCarriles(nullptr),
      btnRivalMenos(nullptr), btnRivalMas(nullptr), textoRivales(nullptr),
      textoLateral(nullptr),
      btnIniciar(nullptr), lblStart(nullptr)
{
    for (int i = 0; i < MAX_SLOTS; i++) indices[i] = 0;

    setSceneRect(0, 0, 680, 850);
    setBackgroundBrush(QBrush(Qt::black));

    QGraphicsTextItem *titulo = addText("CARRERA DE SIMIOS", QFont("Arial", 24, QFont::Bold));
    titulo->setDefaultTextColor(QColor(255, 220, 50));
    titulo->setPos(340 - titulo->boundingRect().width() / 2, 320);

    btnIniciar = new QGraphicsRectItem(220, 430, 240, 50);
    btnIniciar->setBrush(QBrush(QColor(50, 120, 50)));
    btnIniciar->setPen(QPen(QColor(80, 200, 80), 2));
    addItem(btnIniciar);

    lblStart = addText("INICIAR", QFont("Arial", 14, QFont::Bold));
    lblStart->setDefaultTextColor(Qt::white);
    lblStart->setPos(340 - lblStart->boundingRect().width() / 2, 443);
}

MenuPersonalidades::~MenuPersonalidades() {}

Personalidad* MenuPersonalidades::getPersonalidadJugador() const { return nullptr; }
QList<Personalidad*> MenuPersonalidades::getPersonalidadesRivales() const { return {}; }
int MenuPersonalidades::getNumCarriles() const { return numCarriles; }
int MenuPersonalidades::getNumRivales() const { return numRivales; }
Dificultad MenuPersonalidades::getDificultad() const { return dificultadSeleccionada; }

void MenuPersonalidades::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (btnIniciar && btnIniciar->rect().contains(event->scenePos())) {
        btnIniciar->setBrush(QBrush(QColor(30, 80, 30)));
        emit juegoIniciado();
        return;
    }
    QGraphicsScene::mousePressEvent(event);
}

void MenuPersonalidades::crearPersonalidades() {}
void MenuPersonalidades::reconstruirUI() {}
void MenuPersonalidades::construirConfigBar() {}
void MenuPersonalidades::crearTarjeta(int slot, float y, float tarjetaH) {}
void MenuPersonalidades::actualizarTarjeta(int slot) {}
void MenuPersonalidades::actualizarMensajeLateral() {}
