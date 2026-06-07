#include "menuwidget.h"
#include <QResizeEvent>
#include <QMouseEvent>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QPixmap>

MenuWidget::MenuWidget(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Moto Race");
    setFocusPolicy(Qt::StrongFocus);

    vista = new QGraphicsView(this);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setBackgroundBrush(QBrush(QColor(15, 15, 25)));
    setCentralWidget(vista);
    vista->installEventFilter(this);

    construirEscena();
    showMaximized();
}

void MenuWidget::construirEscena()
{
    // Scene más ancha para que los paneles queden FUERA de la imagen central
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1200, 850);
    escena->setBackgroundBrush(QBrush(QColor(15, 15, 25)));
    vista->setScene(escena);

    // Imagen centrada en el scene (680px de ancho, dejando 260px a cada lado)
    QPixmap fondo(":/general/FondoMenuInicial.png");
    if (!fondo.isNull()) {
        auto *itemFondo = escena->addPixmap(
            fondo.scaled(680, 850, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        itemFondo->setPos(260, 0);
        itemFondo->setZValue(-1);
    }

    // Centro del scene: 600px
    static const float CX = 600.f;

    // Título (sobre la imagen del fondo)
    auto *titulo = escena->addText("MOTO RACE", QFont("Arial", 46, QFont::Bold));
    titulo->setDefaultTextColor(QColor(255, 220, 50));
    titulo->setPos(CX - titulo->boundingRect().width() / 2, 60);

    auto *subtitulo = escena->addText("Selecciona un nivel", QFont("Arial", 13));
    subtitulo->setDefaultTextColor(QColor(180, 180, 180));
    subtitulo->setPos(CX - subtitulo->boundingRect().width() / 2, 140);

    // ── Panel izquierdo: Nivel 1 (dentro de los 260px izquierdos) ─────────
    auto *panelN1 = new QGraphicsRectItem(10, 200, 240, 430);
    panelN1->setBrush(QBrush(QColor(10, 40, 10, 220)));
    panelN1->setPen(QPen(QColor(60, 160, 60), 2));
    escena->addItem(panelN1);

    auto *tN1 = escena->addText("NIVEL 1", QFont("Arial", 13, QFont::Bold));
    tN1->setDefaultTextColor(QColor(80, 200, 80));
    tN1->setPos(130 - tN1->boundingRect().width()/2, 210);

    auto *subN1 = escena->addText("Carrera de Simios", QFont("Arial", 9));
    subN1->setDefaultTextColor(QColor(150, 220, 150));
    subN1->setPos(130 - subN1->boundingRect().width()/2, 234);

    auto *descN1 = escena->addText(
        "Carrera lateral por\ncarriles entre simios.\n\n"
        "• Múltiples rivales\n"
        "• Selección de\n  personalidades\n"
        "• Obstáculos: lodo,\n  tronco y charco\n"
        "• Banana de impulso\n"
        "• Dificultad Normal\n  o Difícil\n\n"
        "Controles:\n"
        "W Acelerar\n"
        "S Frenar\n"
        "A/D Cambiar carril\n"
        "J/K Atacar rival",
        QFont("Arial", 9));
    descN1->setDefaultTextColor(QColor(200, 230, 200));
    descN1->setPos(18, 262);

    // ── Panel derecho: Nivel 2 (dentro de los 260px derechos) ─────────────
    auto *panelN2 = new QGraphicsRectItem(950, 200, 240, 430);
    panelN2->setBrush(QBrush(QColor(40, 10, 10, 220)));
    panelN2->setPen(QPen(QColor(180, 60, 60), 2));
    escena->addItem(panelN2);

    auto *tN2 = escena->addText("NIVEL 2", QFont("Arial", 13, QFont::Bold));
    tN2->setDefaultTextColor(QColor(220, 80, 80));
    tN2->setPos(1070 - tN2->boundingRect().width()/2, 210);

    auto *subN2 = escena->addText("Moto Race", QFont("Arial", 9));
    subN2->setDefaultTextColor(QColor(220, 150, 150));
    subN2->setPos(1070 - subN2->boundingRect().width()/2, 234);

    auto *descN2 = escena->addText(
        "Motocross 2D con\nfísica de saltos.\n\n"
        "• Rival con IA\n  adaptativa\n"
        "• Rampas y lodo\n"
        "• Tiros parabólicos\n  de banana\n"
        "• Bonus por vuelta\n  en el aire\n"
        "• Dificultad Normal\n  o Difícil\n\n"
        "Controles:\n"
        "D Acelerar\n"
        "A Frenar\n"
        "W Rotar en vuelo",
        QFont("Arial", 9));
    descN2->setDefaultTextColor(QColor(230, 200, 200));
    descN2->setPos(958, 262);

    // ── Botones centrados sobre la imagen ─────────────────────────────────
    btnNivel1 = new QGraphicsRectItem(CX - 150, 340, 300, 65);
    btnNivel1->setBrush(QBrush(QColor(50, 120, 50, 220)));
    btnNivel1->setPen(QPen(QColor(80, 180, 80), 2));
    btnNivel1->setZValue(1);
    escena->addItem(btnNivel1);
    auto *lbl1 = escena->addText("▶  JUGAR NIVEL 1", QFont("Arial", 14, QFont::Bold));
    lbl1->setDefaultTextColor(Qt::white);
    lbl1->setZValue(2);
    lbl1->setPos(CX - lbl1->boundingRect().width()/2, 358);

    btnNivel2 = new QGraphicsRectItem(CX - 150, 435, 300, 65);
    btnNivel2->setBrush(QBrush(QColor(150, 40, 40, 220)));
    btnNivel2->setPen(QPen(QColor(220, 80, 80), 2));
    btnNivel2->setZValue(1);
    escena->addItem(btnNivel2);
    auto *lbl2 = escena->addText("▶  JUGAR NIVEL 2", QFont("Arial", 14, QFont::Bold));
    lbl2->setDefaultTextColor(Qt::white);
    lbl2->setZValue(2);
    lbl2->setPos(CX - lbl2->boundingRect().width()/2, 453);
}

bool MenuWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == vista && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPointF pos = vista->mapToScene(me->pos());
        if (btnNivel1 && btnNivel1->rect().contains(pos)) { emit nivel1Seleccionado(); return false; }
        if (btnNivel2 && btnNivel2->rect().contains(pos)) { emit nivel2Seleccionado(); return false; }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MenuWidget::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ajustarVista();
}

void MenuWidget::ajustarVista()
{
    if (escena) vista->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);
}
