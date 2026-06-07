#include "mainwindownivel1.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QUrl>
#include <QtMath>
#include <QMessageBox>
#include <QDebug>
#include <stdexcept>

static const QColor COL_FONDO   (20,  20,  25);
static const QColor COL_BORDE   (80,  80, 110);
static const QColor COL_TITULO  (255, 220,  50);
static const QColor COL_TEXTO   (220, 220, 220);
static const QColor COL_BOTON   (60,  60,  80);
static const QColor COL_START   (50,  120,  50);
static const QColor COL_SLOT[8] = {
    Qt::cyan, Qt::red, QColor(255,140,0), QColor(180,0,180),
    QColor(0,200,100), QColor(80,180,255), QColor(255,255,100), QColor(255,100,150)
};
static const QString LABEL_SLOT[8] = {
    "JUGADOR","RIVAL 1","RIVAL 2","RIVAL 3","RIVAL 4","RIVAL 5","RIVAL 6","RIVAL 7"
};
static const QColor COL_BAR[4] = {
    QColor(80,200,120), QColor(80,160,255), QColor(255,180,50), QColor(220,80,80)
};
static const QString LABEL_BAR[4] = { "ACEL","BAN","RES","GOL" };
static const QString MENSAJES_LATERALES[] = {
    "4 carriles:\nÂ¡Espacio\njusto para\ncaos total!\n\nÂ¿Valiente\no imprudente?",
    "5 carriles:\nÂ¡Buen\nequilibrio!\n\nEspacio para\nesquivar\ny atacar.",
    "6 carriles...\nÂ¡Los rivales\ntendrÃ¡n donde\nesconderse!\n\nÂ¿PodrÃ¡s\nalcanzarlos?",
    "7 CARRILES!\n\nAquÃ­ la\nestrategia\nimporta mÃ¡s\nque la\nvelocidad.",
    "8 carriles...\nÂ¡Autopista\ndel caos!\n\nDemasiado\nespacio,\npoco control.",
    "9 carriles!\n\nEsto ya es\nuna locura.\n\nBuena\nsuerte...",
    "Â¡10 CARRILES!\n\nBienvenido\nal caos\nabsoluto.\n\nLa vas\na necesitar.",
};

// â”€â”€ Constructor â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

MainWindowNivel1::MainWindowNivel1(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Carrera de Simios");
    setFocusPolicy(Qt::StrongFocus);

    vista = new QGraphicsView(this);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setRenderHint(QPainter::SmoothPixmapTransform);
    vista->setBackgroundBrush(QBrush(QColor(15, 15, 20)));
    setCentralWidget(vista);
    vista->installEventFilter(this);

    sfxRotacion = new QSoundEffect(this);
    sfxRotacion->setSource(QUrl("qrc:/nivel1/AudioRotacion.wav"));
    sfxRotacion->setVolume(0.8f);

    sfxBoost = new QSoundEffect(this);
    sfxBoost->setSource(QUrl("qrc:/general/boost.wav"));
    sfxBoost->setVolume(0.8f);

    musicaFondo = new QSoundEffect(this);
    musicaFondo->setSource(QUrl("qrc:/nivel1/SonidoPartidaNivel1.wav"));
    musicaFondo->setLoopCount(QSoundEffect::Infinite);
    musicaFondo->setVolume(0.5f);

    personalidadesMenu = CorredorNivel1::Personalidad::disponibles();
    for (int i = 0; i < MAX_SLOTS_MENU; i++)
        indicesMenu[i] = i % personalidadesMenu.size();

    construirEscenaMenu();
    vista->setScene(escenaMenu);
    ajustarVista();
}

MainWindowNivel1::~MainWindowNivel1()
{
    limpiarEscenaNivel();
    delete nivel;
    delete escenaNivel;
    delete escenaMenu;
    delete jugadorGrafico.sprite;
    for (auto &ec : rivalesGrafico) delete ec.sprite;
}

// â”€â”€ MenÃº â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void MainWindowNivel1::construirEscenaMenu()
{
    if (escenaMenu) { delete escenaMenu; escenaMenu = nullptr; }
    escenaMenu = new QGraphicsScene(this);
    escenaMenu->setSceneRect(0, 0, 680, 850);
    escenaMenu->setBackgroundBrush(QBrush(COL_FONDO));
    for (int i = 0; i < MAX_SLOTS_MENU; i++) tarjetasMenu[i] = TarjetaMenu{};
    reconstruirMenuUI();
}

void MainWindowNivel1::reconstruirMenuUI()
{
    escenaMenu->clear();
    for (int i = 0; i < MAX_SLOTS_MENU; i++) tarjetasMenu[i] = TarjetaMenu{};
    btnNormalMenu = btnDificilMenu = nullptr;
    textoDificultadMenu = nullptr;
    btnCarrilMenosMenu = btnCarrilMasMenu = nullptr;
    btnRivalMenosMenu  = btnRivalMasMenu  = nullptr;
    textoCarrilesMenu  = textoRivalesMenu = nullptr;
    textoLateralMenu   = nullptr;
    btnIniciarMenu     = nullptr;
    lblStartMenu       = nullptr;
    btnVolverMenu      = nullptr;

    QPixmap fondoPix(":/nivel1/Fondo3Inicio.png");
    if (!fondoPix.isNull()) {
        fondoPix = fondoPix.scaled(680, 850, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        auto *fondo = escenaMenu->addPixmap(fondoPix); fondo->setZValue(-1);
        auto *overlay = new QGraphicsRectItem(0, 0, 680, 850);
        overlay->setBrush(QBrush(QColor(0,0,0,80))); overlay->setPen(Qt::NoPen); overlay->setZValue(-0.5);
        escenaMenu->addItem(overlay);
    }

    // BotÃ³n volver al menÃº principal
    btnVolverMenu = new QGraphicsRectItem(5, 5, 80, 26);
    btnVolverMenu->setBrush(QBrush(QColor(60,60,80)));
    btnVolverMenu->setPen(QPen(COL_BORDE, 1));
    escenaMenu->addItem(btnVolverMenu);
    auto *lblV = escenaMenu->addText("< Volver", QFont("Arial", 8));
    lblV->setDefaultTextColor(Qt::white); lblV->setPos(8, 8);

    auto *titulo = escenaMenu->addText("SELECCIONAR PERSONALIDADES", QFont("Arial", 13, QFont::Bold));
    titulo->setDefaultTextColor(COL_TITULO);
    titulo->setPos(340 - titulo->boundingRect().width() / 2, 8);

    construirPanelIzquierdoMenu();
    construirPanelDerechoMenu();

    int totalSlots = 1 + numRivalesMenu;
    float yStart = 35.0f, tarjetaGap = 4.0f, btnH_ini = 50.0f;
    float tarjetaH = qBound(60.0f, (850.0f - yStart - btnH_ini - tarjetaGap * (totalSlots-1)) / totalSlots, 155.0f);

    for (int i = 0; i < totalSlots; i++)
        crearTarjetaMenu(i, yStart + i * (tarjetaH + tarjetaGap), tarjetaH);

    float btnY = yStart + totalSlots * (tarjetaH + tarjetaGap) + 2;
    btnIniciarMenu = new QGraphicsRectItem(220, btnY, 240, 42);
    btnIniciarMenu->setBrush(QBrush(COL_START));
    btnIniciarMenu->setPen(QPen(QColor(80,180,80), 2));
    escenaMenu->addItem(btnIniciarMenu);

    lblStartMenu = escenaMenu->addText("INICIAR CARRERA", QFont("Arial", 13, QFont::Bold));
    lblStartMenu->setDefaultTextColor(Qt::white);
    lblStartMenu->setPos(340 - lblStartMenu->boundingRect().width() / 2, btnY + 10);
}

void MainWindowNivel1::construirPanelIzquierdoMenu()
{
    float x = 10.0f; float btnW = 24, btnH = 22;
    auto *panel = new QGraphicsRectItem(x, 35, 185, 120);
    panel->setBrush(QBrush(QColor(10,10,20,170))); panel->setPen(QPen(QColor(100,100,140,180),1));
    escenaMenu->addItem(panel);

    auto *lblC = escenaMenu->addText("CARRILES:", QFont("Arial",9,QFont::Bold));
    lblC->setDefaultTextColor(COL_TITULO); lblC->setPos(x+5, 42);

    btnCarrilMenosMenu = new QGraphicsRectItem(x+5, 60, btnW, btnH);
    btnCarrilMenosMenu->setBrush(QBrush(COL_BOTON)); btnCarrilMenosMenu->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(btnCarrilMenosMenu);
    auto *mC = escenaMenu->addText("-", QFont("Arial",11,QFont::Bold));
    mC->setDefaultTextColor(Qt::white); mC->setPos(x+11, 61);

    textoCarrilesMenu = escenaMenu->addText(QString::number(numCarrilesMenu), QFont("Arial",11,QFont::Bold));
    textoCarrilesMenu->setDefaultTextColor(Qt::white); textoCarrilesMenu->setPos(x+38, 62);

    btnCarrilMasMenu = new QGraphicsRectItem(x+60, 60, btnW, btnH);
    btnCarrilMasMenu->setBrush(QBrush(COL_BOTON)); btnCarrilMasMenu->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(btnCarrilMasMenu);
    auto *pC = escenaMenu->addText("+", QFont("Arial",11,QFont::Bold));
    pC->setDefaultTextColor(Qt::white); pC->setPos(x+66, 61);

    auto *lblR = escenaMenu->addText("RIVALES:", QFont("Arial",9,QFont::Bold));
    lblR->setDefaultTextColor(COL_TITULO); lblR->setPos(x+5, 92);

    btnRivalMenosMenu = new QGraphicsRectItem(x+5, 110, btnW, btnH);
    btnRivalMenosMenu->setBrush(QBrush(COL_BOTON)); btnRivalMenosMenu->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(btnRivalMenosMenu);
    auto *mR = escenaMenu->addText("-", QFont("Arial",11,QFont::Bold));
    mR->setDefaultTextColor(Qt::white); mR->setPos(x+11, 111);

    textoRivalesMenu = escenaMenu->addText(QString::number(numRivalesMenu), QFont("Arial",11,QFont::Bold));
    textoRivalesMenu->setDefaultTextColor(Qt::white); textoRivalesMenu->setPos(x+38, 112);

    btnRivalMasMenu = new QGraphicsRectItem(x+60, 110, btnW, btnH);
    btnRivalMasMenu->setBrush(QBrush(COL_BOTON)); btnRivalMasMenu->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(btnRivalMasMenu);
    auto *pR = escenaMenu->addText("+", QFont("Arial",11,QFont::Bold));
    pR->setDefaultTextColor(Qt::white); pR->setPos(x+66, 111);

    auto *hint = escenaMenu->addText("*Para mas rivales\nagrega carriles", QFont("Arial",7));
    hint->setDefaultTextColor(QColor(140,140,170)); hint->setPos(x+5, 138);

    textoLateralMenu = escenaMenu->addText("", QFont("Arial",9,QFont::Bold));
    textoLateralMenu->setDefaultTextColor(QColor(120,120,160)); textoLateralMenu->setPos(x+5, 200);
    actualizarMensajeLateralMenu();
}

void MainWindowNivel1::construirPanelDerechoMenu()
{
    float x = 490.0f;
    auto *panel = new QGraphicsRectItem(x, 35, 185, 160);
    panel->setBrush(QBrush(QColor(10,10,20,170))); panel->setPen(QPen(QColor(100,100,140,180),1));
    escenaMenu->addItem(panel);

    auto *lblD = escenaMenu->addText("DIFICULTAD:", QFont("Arial",9,QFont::Bold));
    lblD->setDefaultTextColor(COL_TITULO); lblD->setPos(x+5, 42);

    struct BtnDif { const char *label; DificultadNivel1 dif; QColor color; float y; };
    BtnDif bots[2] = {{"NORMAL",DificultadNivel1::NORMAL,QColor(50,150,50),62},{"DIFICIL",DificultadNivel1::DIFICIL,QColor(160,40,40),92}};
    QGraphicsRectItem **ptrs[2] = {&btnNormalMenu, &btnDificilMenu};
    for (int i = 0; i < 2; i++) {
        bool act = (dificultadMenu == bots[i].dif);
        *ptrs[i] = new QGraphicsRectItem(x+5, bots[i].y, 175, 22);
        (*ptrs[i])->setBrush(QBrush(act ? bots[i].color : COL_BOTON));
        (*ptrs[i])->setPen(QPen(act ? bots[i].color.lighter(150) : COL_BORDE, act ? 2 : 1));
        escenaMenu->addItem(*ptrs[i]);
        auto *lbl = escenaMenu->addText(bots[i].label, QFont("Arial",8,QFont::Bold));
        lbl->setDefaultTextColor(Qt::white);
        lbl->setPos(x+92 - lbl->boundingRect().width()/2, bots[i].y+5);
    }
    static const char* DESC[2] = {"Equilibrio entre\nvelocidad, golpes\ny bonus","Rivales agresivos\nGolpes fuertes\nBananas debiles"};
    int idx = (dificultadMenu == DificultadNivel1::DIFICIL) ? 1 : 0;
    textoDificultadMenu = escenaMenu->addText(DESC[idx], QFont("Arial",8));
    textoDificultadMenu->setDefaultTextColor(QColor(200,200,200)); textoDificultadMenu->setPos(x+5, 152);
}

void MainWindowNivel1::crearTarjetaMenu(int slot, float y, float tarjetaH)
{
    tarjetasMenu[slot].y = y;
    static const float CARD_X = 197.0f, CARD_W = 286.0f;

    auto *fondo = new QGraphicsRectItem(CARD_X, y, CARD_W, tarjetaH);
    fondo->setBrush(QBrush(QColor(35,35,45,180))); fondo->setPen(QPen(COL_SLOT[slot],2));
    escenaMenu->addItem(fondo);

    auto *lblSlot = escenaMenu->addText(LABEL_SLOT[slot], QFont("Arial",9,QFont::Bold));
    lblSlot->setDefaultTextColor(COL_SLOT[slot]); lblSlot->setPos(CARD_X+5, y+3);

    tarjetasMenu[slot].textoNombre = escenaMenu->addText("", QFont("Arial",10,QFont::Bold));
    tarjetasMenu[slot].textoNombre->setDefaultTextColor(Qt::white);

    float btnH = 22.0f, btnY2 = y + tarjetaH - btnH - 4;
    float barTop = y+22.0f, barH2 = btnY2-barTop-4.0f;
    float barStep = barH2/4.0f, barH = qMin(barStep*0.65f, 13.0f);
    float barX = CARD_X+38.0f, barMaxW = CARD_W-80.0f;

    for (int b = 0; b < 4; b++) {
        float barY = barTop + b*barStep + (barStep-barH)/2.0f;
        tarjetasMenu[slot].textosBarras[b] = escenaMenu->addText(LABEL_BAR[b], QFont("Consolas",7));
        tarjetasMenu[slot].textosBarras[b]->setDefaultTextColor(COL_TEXTO);
        tarjetasMenu[slot].textosBarras[b]->setPos(barX, barY);
        auto *bg = new QGraphicsRectItem(barX+34, barY+1, barMaxW, barH);
        bg->setBrush(QBrush(QColor(50,50,60,160))); bg->setPen(Qt::NoPen);
        escenaMenu->addItem(bg);
        tarjetasMenu[slot].barras[b] = new QGraphicsRectItem(barX+34, barY+1, 0, barH);
        tarjetasMenu[slot].barras[b]->setBrush(QBrush(COL_BAR[b])); tarjetasMenu[slot].barras[b]->setPen(Qt::NoPen);
        escenaMenu->addItem(tarjetasMenu[slot].barras[b]);
    }

    tarjetasMenu[slot].btnIzq = new QGraphicsRectItem(CARD_X, btnY2, 30, btnH);
    tarjetasMenu[slot].btnIzq->setBrush(QBrush(COL_BOTON)); tarjetasMenu[slot].btnIzq->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(tarjetasMenu[slot].btnIzq);
    auto *fi = escenaMenu->addText("<", QFont("Arial",11,QFont::Bold));
    fi->setDefaultTextColor(Qt::white); fi->setPos(CARD_X+7, btnY2+2);

    tarjetasMenu[slot].btnDer = new QGraphicsRectItem(CARD_X+CARD_W-30, btnY2, 30, btnH);
    tarjetasMenu[slot].btnDer->setBrush(QBrush(COL_BOTON)); tarjetasMenu[slot].btnDer->setPen(QPen(COL_BORDE,1));
    escenaMenu->addItem(tarjetasMenu[slot].btnDer);
    auto *fd = escenaMenu->addText(">", QFont("Arial",11,QFont::Bold));
    fd->setDefaultTextColor(Qt::white); fd->setPos(CARD_X+CARD_W-22, btnY2+2);

    actualizarTarjetaMenu(slot);
}

void MainWindowNivel1::actualizarTarjetaMenu(int slot)
{
    const auto &p = personalidadesMenu[indicesMenu[slot]];
    tarjetasMenu[slot].textoNombre->setPlainText(p.nombre);
    float cx = 340.0f - tarjetasMenu[slot].textoNombre->boundingRect().width() / 2.0f;
    tarjetasMenu[slot].textoNombre->setPos(cx, tarjetasMenu[slot].y + 4);
    float vals[4] = {p.modAceleracion, p.modVelMax, p.resistenciaGolpe, p.fuerzaGolpe};
    for (int b = 0; b < 4; b++) {
        float norm = qBound(0.0f, (vals[b]-0.6f)/(1.5f-0.6f), 1.0f);
        auto r = tarjetasMenu[slot].barras[b]->rect();
        tarjetasMenu[slot].barras[b]->setRect(r.x(), r.y(), norm*206.0f, r.height());
    }
}

void MainWindowNivel1::actualizarMensajeLateralMenu()
{
    if (!textoLateralMenu) return;
    textoLateralMenu->setPlainText(MENSAJES_LATERALES[qBound(0, numCarrilesMenu-4, 6)]);
}

void MainWindowNivel1::onMenuMousePress(QPointF pos)
{
    if (btnVolverMenu && btnVolverMenu->rect().contains(pos)) { onVolver(); return; }
    if (btnNormalMenu  && btnNormalMenu->rect().contains(pos))  { dificultadMenu = DificultadNivel1::NORMAL;  reconstruirMenuUI(); return; }
    if (btnDificilMenu && btnDificilMenu->rect().contains(pos)) { dificultadMenu = DificultadNivel1::DIFICIL; reconstruirMenuUI(); return; }
    if (btnIniciarMenu && btnIniciarMenu->rect().contains(pos)) { btnIniciarMenu->setBrush(QBrush(QColor(30,80,30))); iniciarNivel(); return; }
    if (btnCarrilMenosMenu && btnCarrilMenosMenu->rect().contains(pos)) {
        if (numCarrilesMenu > qMax(4, numRivalesMenu+3)) { numCarrilesMenu--; reconstruirMenuUI(); } return;
    }
    if (btnCarrilMasMenu && btnCarrilMasMenu->rect().contains(pos)) {
        if (numCarrilesMenu < 10) { numCarrilesMenu++; reconstruirMenuUI(); } return;
    }
    if (btnRivalMenosMenu && btnRivalMenosMenu->rect().contains(pos)) {
        if (numRivalesMenu > 2) { numRivalesMenu--; reconstruirMenuUI(); } return;
    }
    if (btnRivalMasMenu && btnRivalMasMenu->rect().contains(pos)) {
        if (numRivalesMenu < numCarrilesMenu-3 && numRivalesMenu < 7) { numRivalesMenu++; reconstruirMenuUI(); } return;
    }
    for (int i = 0; i < 1+numRivalesMenu; i++) {
        if (tarjetasMenu[i].btnIzq && tarjetasMenu[i].btnIzq->rect().contains(pos)) {
            indicesMenu[i] = (indicesMenu[i]-1+personalidadesMenu.size()) % personalidadesMenu.size();
            actualizarTarjetaMenu(i); return;
        }
        if (tarjetasMenu[i].btnDer && tarjetasMenu[i].btnDer->rect().contains(pos)) {
            indicesMenu[i] = (indicesMenu[i]+1) % personalidadesMenu.size();
            actualizarTarjetaMenu(i); return;
        }
    }
}

// â”€â”€ Nivel â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void MainWindowNivel1::iniciarNivel()
{
    try {
        limpiarEscenaNivel();
        delete nivel;

        nivel = new Nivel1(numCarrilesMenu, numRivalesMenu, this);
        nivel->setDificultad(dificultadMenu);

        CorredorNivel1::Personalidad pJug = personalidadesMenu[indicesMenu[0]];
        QList<CorredorNivel1::Personalidad> pRiv;
        for (int i = 1; i <= numRivalesMenu; i++)
            pRiv.append(personalidadesMenu[indicesMenu[i]]);
        nivel->setPersonalidades(pJug, pRiv);

        connect(nivel, &Nivel1::actualizado,           this, &MainWindowNivel1::onActualizado);
        connect(nivel, &Nivel1::cuentaRegresivaCambio, this, &MainWindowNivel1::onCuentaRegresiva);
        connect(nivel, &Nivel1::juegoIniciado,         this, &MainWindowNivel1::onJuegoIniciado);
        connect(nivel, &Nivel1::carreraFinalizada,     this, &MainWindowNivel1::onCarreraFinalizada);
        connect(nivel, &Nivel1::obstaculoCreado,       this, &MainWindowNivel1::onObstaculoCreado);
        connect(nivel, &Nivel1::efectoCreado,          this, &MainWindowNivel1::onEfectoCreado);
        connect(nivel, &Nivel1::textoFlotanteCreado,   this, &MainWindowNivel1::onTextoFlotante);
        connect(nivel, &Nivel1::sonidoRotacion,        this, &MainWindowNivel1::onSonidoRotacion);
        connect(nivel, &Nivel1::sonidoBoost,           this, &MainWindowNivel1::onSonidoBoost);

        escenaNivel = new QGraphicsScene(this);
        escenaNivel->setSceneRect(0, 0, ESCENA_ANCHO, ESCENA_ALTO);

        construirEscenaNivel();
        vista->setScene(escenaNivel);
        ajustarVista();
        nivel->iniciar();

    } catch (const std::exception &e) {
        QMessageBox::warning(nullptr, "Error", e.what());
    }
}

void MainWindowNivel1::reiniciarNivel() { iniciarNivel(); }

void MainWindowNivel1::liberarRecursos()
{
    // Libera toda la memoria del juego al salir del nivel
    if (nivel) {
        nivel->disconnect();  // desconectar señales antes de destruir
        limpiarEscenaNivel();
        delete nivel; nivel = nullptr;
    }
    musicaFondo->stop();
}

void MainWindowNivel1::onVolver()
{
    limpiarEscenaNivel();
    delete nivel; nivel = nullptr;
    musicaFondo->stop();
    emit volverAlMenu();
}

void MainWindowNivel1::construirEscenaNivel()
{
    float anchoCarril = nivel->getAnchoCarril();
    float margenX     = nivel->getMargenX();
    int   nCarriles   = nivel->getNumCarriles();
    float pistaW      = nCarriles * anchoCarril;
    float pistaR      = margenX + pistaW;

    escenaNivel->setBackgroundBrush(QBrush(QColor(20,20,25)));

    QPixmap fondoPix(":/nivel1/Pista.png");
    if (!fondoPix.isNull()) {
        QPixmap tile = fondoPix.scaled(ESCENA_ANCHO, TILE_ALTO, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        fondoTile1 = escenaNivel->addPixmap(tile); fondoTile1->setPos(0,0); fondoTile1->setZValue(-2);
        fondoTile2 = escenaNivel->addPixmap(tile); fondoTile2->setPos(0,-TILE_ALTO); fondoTile2->setZValue(-2);
    }

    rectPista = new QGraphicsRectItem(0, 0, nCarriles * anchoCarril, 900);
    rectPista->setPos(margenX, 0);
    rectPista->setBrush(QBrush(QColor(60,40,20,120)));
    rectPista->setPen(Qt::NoPen);
    escenaNivel->addItem(rectPista);

    for (int i = 1; i < nCarriles; i++)
        escenaNivel->addLine(margenX+i*anchoCarril, 0, margenX+i*anchoCarril, ESCENA_ALTO, QPen(QColor(150,150,150),1,Qt::DashLine));
    escenaNivel->addLine(margenX, 0, margenX, ESCENA_ALTO, QPen(Qt::white,2));
    escenaNivel->addLine(pistaR,  0, pistaR,  ESCENA_ALTO, QPen(Qt::white,2));

    textoInfo = escenaNivel->addText("", QFont("Consolas",9));
    textoInfo->setDefaultTextColor(Qt::white); textoInfo->setPos(margenX, 810); textoInfo->setVisible(false);

    textoTiempo = escenaNivel->addText("3:00", QFont("Arial",14,QFont::Bold));
    textoTiempo->setDefaultTextColor(QColor(255,220,50)); textoTiempo->setPos(pistaR+10, 10);

    textoCuenta = escenaNivel->addText("", QFont("Arial",48,QFont::Bold));
    textoCuenta->setDefaultTextColor(Qt::yellow); textoCuenta->setPos(margenX+pistaW/2-70, 380);

    textoControles = escenaNivel->addText(
        "W: Acelerar  S: Frenar\nA/D: Cambiar carril\nJ/K: Golpear rival",
        QFont("Consolas",9));
    textoControles->setDefaultTextColor(QColor(180,180,180)); textoControles->setPos(margenX, 5);

    int sw = (int)(anchoCarril * SPRITE_ESCALA_W);
    int sh = (int)(sw * SPRITE_ESCALA_H);
    JugadorNivel1 *jug = nivel->getJugador();
    QString rutaJug = jug->getRutaSprite();
    if (rutaJug.isEmpty()) rutaJug = ":/nivel1/SimioGolpeadorUsuario.png";
    jugadorGrafico = crearItemCorredor(sw, sh, rutaJug, jug->getPosX(), jug->getPosY());
    escenaNivel->addItem(jugadorGrafico.item);

    rivalesGrafico.clear();
    for (const auto &er : nivel->getRivales()) {
        QString rutaR = er.rival->getRutaSprite();
        if (rutaR.isEmpty()) rutaR = ":/nivel1/SimioGolpeador.png";
        EntradaCorredor ec = crearItemCorredor(sw, sh, rutaR, er.rival->getPosX(), er.rival->getPosY());
        escenaNivel->addItem(ec.item);
        rivalesGrafico.append(ec);
    }

    carreraTerminada = false;
    btnReiniciar = btnSiguiente = btnMenuFin = nullptr;
}

void MainWindowNivel1::limpiarEscenaNivel()
{
    if (!escenaNivel) return;
    delete jugadorGrafico.sprite; jugadorGrafico = {};
    for (auto &ec : rivalesGrafico) delete ec.sprite;
    rivalesGrafico.clear();
    for (auto *it : itemsObstaculos) delete it; itemsObstaculos.clear();
    for (auto *it : itemsEfectos) delete it; itemsEfectos.clear();
    for (auto &[Item, _] : textosFlotantes) delete Item;
    textosFlotantes.clear();
    delete rectPista; rectPista = nullptr;
    escenaNivel->clear();
    delete escenaNivel; escenaNivel = nullptr;
    fondoTile1 = fondoTile2 = nullptr;
    textoControles = textoInfo = textoCuenta = textoTiempo = nullptr;
    btnReiniciar = btnSiguiente = btnMenuFin = nullptr;
}

// â”€â”€ Slots â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void MainWindowNivel1::onActualizado()
{
    if (!nivel || !escenaNivel) return;
    if (fondoTile1 && fondoTile2) {
        float off = nivel->getFondoOffsetY();
        fondoTile1->setPos(0, off); fondoTile2->setPos(0, off-TILE_ALTO);
    }
    JugadorNivel1 *jug = nivel->getJugador();
    jugadorGrafico.item->setPos(jug->getPosX(), jug->getPosY());
    jugadorGrafico.setSpriteIndex(jug->getSpriteIndex());
    jugadorGrafico.item->setRotation(jug->getRotacionActual());

    const auto &rivales = nivel->getRivales();
    for (int i = 0; i < rivales.size() && i < rivalesGrafico.size(); i++) {
        auto &ec = rivalesGrafico[i]; RivalNivel1 *r = rivales[i].rival;
        ec.item->setPos(r->getPosX(), r->getPosY());
        ec.setSpriteIndex(r->getSpriteIndex());
        ec.item->setRotation(r->getRotacionActual());
    }
    const auto &obs = nivel->getObstaculos();
    for (int i = 0; i < obs.size() && i < itemsObstaculos.size(); i++)
        itemsObstaculos[i]->setPos(obs[i]->getPosX(), obs[i]->getPosY());
    const auto &efx = nivel->getEfectos();
    for (int i = 0; i < efx.size() && i < itemsEfectos.size(); i++)
        itemsEfectos[i]->setPos(efx[i]->getPosX(), efx[i]->getPosY());

    float t = nivel->getTiempoRestante();
    int mins = (int)t/60, secs = (int)t%60;
    textoTiempo->setPlainText(QString("%1:%2").arg(mins).arg(secs,2,10,QChar('0')));
    if (t < 30.0f) textoTiempo->setDefaultTextColor(QColor(255,80,80));
    actualizarTextosFlotantes();
}

void MainWindowNivel1::onCuentaRegresiva(int v) {
    if (!textoCuenta) return;
    if (v > 0) {
        textoCuenta->setPlainText(QString::number(v));
        if (v == 3) musicaFondo->play();
    }
    else if (v == 0) textoCuenta->setPlainText("GO!");
    else textoCuenta->setPlainText("");
}

void MainWindowNivel1::onJuegoIniciado() {
    if (textoControles) textoControles->setVisible(false);
    // MÃºsica ya iniciada en onCuentaRegresiva al mostrar el "3"
}

void MainWindowNivel1::onCarreraFinalizada(bool gano)
{
    carreraTerminada = true;
    musicaFondo->stop();
    QPixmap img(gano ? ":/nivel1/ImagenVictoria.png" : ":/nivel1/ImagenDerrota.png");
    if (!img.isNull()) {
        auto *f = escenaNivel->addPixmap(img.scaled(ESCENA_ANCHO, ESCENA_ALTO, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        f->setPos(0,0); f->setZValue(10);
        if (gano) {
            btnSiguiente = new QGraphicsRectItem(18,778,196,58);  btnSiguiente->setBrush(Qt::NoBrush); btnSiguiente->setPen(Qt::NoPen); btnSiguiente->setZValue(11); escenaNivel->addItem(btnSiguiente);
            btnReiniciar = new QGraphicsRectItem(236,778,208,58); btnReiniciar->setBrush(Qt::NoBrush); btnReiniciar->setPen(Qt::NoPen); btnReiniciar->setZValue(11); escenaNivel->addItem(btnReiniciar);
            btnMenuFin   = new QGraphicsRectItem(466,778,196,58); btnMenuFin->setBrush(Qt::NoBrush);   btnMenuFin->setPen(Qt::NoPen);   btnMenuFin->setZValue(11);   escenaNivel->addItem(btnMenuFin);
        } else {
            btnReiniciar = new QGraphicsRectItem(30,778,290,58);  btnReiniciar->setBrush(Qt::NoBrush); btnReiniciar->setPen(Qt::NoPen); btnReiniciar->setZValue(11); escenaNivel->addItem(btnReiniciar);
            btnMenuFin   = new QGraphicsRectItem(358,778,292,58); btnMenuFin->setBrush(Qt::NoBrush);   btnMenuFin->setPen(Qt::NoPen);   btnMenuFin->setZValue(11);   escenaNivel->addItem(btnMenuFin);
        }
    }
}

void MainWindowNivel1::onObstaculoCreado(int index) {
    const auto &obs = nivel->getObstaculos();
    if (index < 0 || index >= obs.size()) return;
    auto *item = crearItemObstaculo(obs[index]);
    escenaNivel->addItem(item); itemsObstaculos.append(item);
}

void MainWindowNivel1::onEfectoCreado(int index) {
    const auto &efx = nivel->getEfectos();
    if (index < 0 || index >= efx.size()) return;
    auto *item = crearItemObstaculo(efx[index]);
    escenaNivel->addItem(item); itemsEfectos.append(item);
}


void MainWindowNivel1::onTextoFlotante(const QString &texto, float x, float y, const QColor &color) {
    auto *item = escenaNivel->addText(texto, QFont("Arial",11,QFont::Bold));
    item->setDefaultTextColor(color); item->setPos(x, y);
    textosFlotantes.append({item, 0.8f});
}

void MainWindowNivel1::onSonidoRotacion() { sfxRotacion->play(); }
void MainWindowNivel1::onSonidoBoost()    { sfxBoost->play(); }

void MainWindowNivel1::actualizarTextosFlotantes()
{
    for (int i = textosFlotantes.size()-1; i >= 0; i--) {
        auto &[item, tiempo] = textosFlotantes[i];
        tiempo -= dt;
        item->setPos(item->x(), item->y() - 40.0f * dt);
        QColor c = item->defaultTextColor(); c.setAlphaF(qMax(0.0f, tiempo/0.8f));
        item->setDefaultTextColor(c);
        if (tiempo <= 0) { escenaNivel->removeItem(item); delete item; textosFlotantes.removeAt(i); }
    }
}

QGraphicsPixmapItem* MainWindowNivel1::crearItemObstaculo(const Item *obs)
{
    float anchoCarril = nivel->getAnchoCarril();
    QPixmap pix; int w, h;
    if (obs->esImpulso())         { pix = QPixmap(":/general/Banana.png"); w=55; h=40; }
    else if (obs->getTipo() == 1) { pix = QPixmap(":/nivel1/Tronco.png"); w=(int)(anchoCarril*0.9f); h=36; }
    else if (obs->getTipo() == 2) { pix = QPixmap(":/nivel1/Agua.png");   w=(int)(anchoCarril*0.9f); h=(int)(anchoCarril*0.7f); }
    else                          { pix = QPixmap(":/nivel1/Lodo.png");   w=(int)(anchoCarril*0.9f); h=32; }
    auto *item = new QGraphicsPixmapItem();
    if (!pix.isNull()) item->setPixmap(pix.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    item->setZValue(obs->esImpulso() ? 1.0 : -1.0);
    item->setPos(obs->getPosX(), obs->getPosY());
    return item;
}

EntradaCorredor MainWindowNivel1::crearItemCorredor(int tw, int th, const QString &ruta, float x, float y)
{
    EntradaCorredor ec;
    ec.targetW = tw; ec.targetH = th;
    ec.sprite  = new Sprite(ruta, 3, 1);
    ec.item    = new QGraphicsPixmapItem();
    ec.item->setZValue(1.0);
    ec.setSpriteIndex(0);
    ec.item->setPos(x, y);
    return ec;
}

// â”€â”€ Eventos â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool MainWindowNivel1::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == vista && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPointF pos = vista->mapToScene(me->pos());
        if (vista->scene() == escenaMenu) {
            onMenuMousePress(pos); return false;
        }
        if (vista->scene() == escenaNivel && carreraTerminada) {
            if (btnSiguiente && btnSiguiente->rect().contains(pos)) { musicaFondo->stop(); emit irAlNivel2(); return false; }
            if (btnReiniciar && btnReiniciar->rect().contains(pos)) { reiniciarNivel(); return false; }
            if (btnMenuFin   && btnMenuFin->rect().contains(pos))   {
                limpiarEscenaNivel(); delete nivel; nivel = nullptr;
                vista->setScene(escenaMenu); ajustarVista(); return false;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindowNivel1::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) { onVolver(); return; }
    if (event->key() == Qt::Key_F11) { isFullScreen() ? showMaximized() : showFullScreen(); return; }
    if (nivel) nivel->procesarTecla(event->key(), true);
}

void MainWindowNivel1::keyReleaseEvent(QKeyEvent *event)
{
    if (nivel) nivel->procesarTecla(event->key(), false);
    QMainWindow::keyReleaseEvent(event);
}

void MainWindowNivel1::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); ajustarVista();
}

void MainWindowNivel1::ajustarVista()
{
    if (vista->scene()) vista->fitInView(vista->scene()->sceneRect(), Qt::KeepAspectRatio);
}
