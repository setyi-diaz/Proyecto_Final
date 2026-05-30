#include "menupersonalidades.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>

MenuPersonalidades::MenuPersonalidades(QObject *parent)
    : QGraphicsScene(parent),
      numCarriles(5),
      numRivales(2),
      btnCarrilMenos(nullptr), btnCarrilMas(nullptr), textoCarriles(nullptr),
      btnRivalMenos(nullptr),  btnRivalMas(nullptr),  textoRivales(nullptr),
      textoLateral(nullptr),
      btnIniciar(nullptr), lblStart(nullptr)
{}

void MenuPersonalidades::crearPersonalidades() {}

void MenuPersonalidades::reconstruirUI() {}

void MenuPersonalidades::construirConfigBar() {}

void MenuPersonalidades::crearTarjeta(int slot, float y, float tarjetaH) {}

void MenuPersonalidades::actualizarTarjeta(int slot) {}

void MenuPersonalidades::actualizarMensajeLateral() {}

void MenuPersonalidades::mousePressEvent(QGraphicsSceneMouseEvent *event) {}

Personalidad* MenuPersonalidades::getPersonalidadJugador() const { return nullptr; }

QList<Personalidad*> MenuPersonalidades::getPersonalidadesRivales() const { return QList<Personalidad*>(); }

int MenuPersonalidades::getNumCarriles() const { return 0; }
int MenuPersonalidades::getNumRivales()  const { return 0; }
