#include "mainwindownivel2.h"
#include "rampa.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QFont>
#include <QBrush>
#include <QPen>

static constexpr float MENU_W = 680.f;
static constexpr float MENU_H = 850.f;

MainWindowNivel2::MainWindowNivel2(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Moto Race - Nivel 2");
    setFocusPolicy(Qt::StrongFocus);

    vista = new QGraphicsView(this);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setRenderHint(QPainter::SmoothPixmapTransform);
    vista->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    vista->setBackgroundBrush(QBrush(QColor(15,15,20)));
    setCentralWidget(vista);
    vista->installEventFilter(this);

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(8);
    connect(timer, &QTimer::timeout, this, &MainWindowNivel2::actualizar);

    // Sonidos
    sfxPartida = new QSoundEffect(this);
    sfxPartida->setSource(QUrl("qrc:/nivel2/SonidoPartidaNivel2.wav"));
    sfxPartida->setLoopCount(QSoundEffect::Infinite);
    sfxPartida->setVolume(0.5f);

    sfxBanana = new QSoundEffect(this);
    sfxBanana->setSource(QUrl("qrc:/general/boost.wav"));  // mismo efecto banana del nivel 1
    sfxBanana->setLoopCount(1);
    sfxBanana->setVolume(0.7f);

    sfxLodo = new QSoundEffect(this);
    sfxLodo->setSource(QUrl("qrc:/nivel2/ColisionLodo.wav"));
    sfxLodo->setLoopCount(1);
    sfxLodo->setVolume(0.8f);

    construirEscenaMenu();
    mostrarMenu();
}

MainWindowNivel2::~MainWindowNivel2()
{
    limpiarEscenaJuego();
    delete nivel;
    delete escenaMenu;
    delete escenaJuego;
}

// ── Menú interno ─────────────────────────────────────────────────────────────

void MainWindowNivel2::mostrarMenu()
{
    timer->stop();
    if (sfxPartida) sfxPartida->stop();
    vista->setScene(escenaMenu);
    ajustarVista();
}

void MainWindowNivel2::resetearAlMenu()
{
    liberarRecursos();
    vista->setScene(escenaMenu);
    ajustarVista();
}

void MainWindowNivel2::liberarRecursos()
{
    timer->stop();
    if (sfxPartida) sfxPartida->stop();
    limpiarEscenaJuego();
    delete escenaJuego;
    escenaJuego = nullptr;
    if (nivel) {
        nivel->disconnect();
        delete nivel;
        nivel = nullptr;
    }
    acelerando = frenando = rotando = false;
    camaraX = fondoOffsetX = 0.f;
    tiempoImpacto = tiempoBono = tiempoParpadeo = 0.f;
    frameJugadorAnterior = frameRivalAnterior = -1;
}

void MainWindowNivel2::construirEscenaMenu()
{
    delete escenaMenu;
    escenaMenu = new QGraphicsScene(this);
    escenaMenu->setSceneRect(0, 0, MENU_W, MENU_H);

    // Fondo
    QPixmap fondo(":/nivel2/FondoMenuNivel2.png");
    if (!fondo.isNull()) {
        auto *itemFondo = escenaMenu->addPixmap(
            fondo.scaled(MENU_W, MENU_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        itemFondo->setPos(0, 0);
        itemFondo->setZValue(-1);
    }

    // Título
    auto *titulo = escenaMenu->addText("NIVEL 2", QFont("Arial", 52, QFont::Bold));
    titulo->setDefaultTextColor(QColor(255, 200, 40));
    titulo->setPos(MENU_W/2 - titulo->boundingRect().width()/2, 100);

    auto *subtitulo = escenaMenu->addText("Moto Race", QFont("Arial", 18));
    subtitulo->setDefaultTextColor(QColor(200, 200, 200));
    subtitulo->setPos(MENU_W/2 - subtitulo->boundingRect().width()/2, 185);

    auto *selecciona = escenaMenu->addText("Selecciona la dificultad", QFont("Arial", 14));
    selecciona->setDefaultTextColor(QColor(160, 160, 160));
    selecciona->setPos(MENU_W/2 - selecciona->boundingRect().width()/2, 230);

    // Botón Normal
    btnMenuNormal = new QGraphicsRectItem(165, 320, 350, 80);
    btnMenuNormal->setBrush(QBrush(QColor(30, 100, 30, 200)));
    btnMenuNormal->setPen(QPen(QColor(60, 180, 60), 2));
    btnMenuNormal->setZValue(1);
    escenaMenu->addItem(btnMenuNormal);

    auto *lblNormal = escenaMenu->addText("NORMAL", QFont("Arial", 20, QFont::Bold));
    lblNormal->setDefaultTextColor(Qt::white);
    lblNormal->setZValue(2);
    lblNormal->setPos(MENU_W/2 - lblNormal->boundingRect().width()/2, 337);

    auto *descNormal = escenaMenu->addText("Rival moderado  ·  Ventaja inicial  ·  Daño leve", QFont("Arial", 9));
    descNormal->setDefaultTextColor(QColor(180, 255, 180));
    descNormal->setZValue(2);
    descNormal->setPos(MENU_W/2 - descNormal->boundingRect().width()/2, 368);

    // Botón Difícil
    btnMenuDificil = new QGraphicsRectItem(165, 440, 350, 80);
    btnMenuDificil->setBrush(QBrush(QColor(130, 30, 30, 200)));
    btnMenuDificil->setPen(QPen(QColor(220, 60, 60), 2));
    btnMenuDificil->setZValue(1);
    escenaMenu->addItem(btnMenuDificil);

    auto *lblDificil = escenaMenu->addText("DIFÍCIL", QFont("Arial", 20, QFont::Bold));
    lblDificil->setDefaultTextColor(Qt::white);
    lblDificil->setZValue(2);
    lblDificil->setPos(MENU_W/2 - lblDificil->boundingRect().width()/2, 457);

    auto *descDificil = escenaMenu->addText("Rival rápido  ·  Sin ventaja  ·  Banana más dañina", QFont("Arial", 9));
    descDificil->setDefaultTextColor(QColor(255, 180, 180));
    descDificil->setZValue(2);
    descDificil->setPos(MENU_W/2 - descDificil->boundingRect().width()/2, 488);

    // Controles
    auto *ctrl = escenaMenu->addText("D: Acelerar     A: Frenar     W: Rotar en vuelo", QFont("Arial", 10));
    ctrl->setDefaultTextColor(QColor(140, 140, 140));
    ctrl->setZValue(2);
    ctrl->setPos(MENU_W/2 - ctrl->boundingRect().width()/2, 590);

    // Botón volver
    btnMenuVolver = new QGraphicsRectItem(240, 650, 200, 50);
    btnMenuVolver->setBrush(QBrush(QColor(40, 40, 60, 200)));
    btnMenuVolver->setPen(QPen(QColor(100, 100, 140), 2));
    btnMenuVolver->setZValue(1);
    escenaMenu->addItem(btnMenuVolver);

    auto *lblVolver = escenaMenu->addText("← Volver", QFont("Arial", 14));
    lblVolver->setDefaultTextColor(Qt::white);
    lblVolver->setZValue(2);
    lblVolver->setPos(MENU_W/2 - lblVolver->boundingRect().width()/2, 663);
}

void MainWindowNivel2::onMenuMousePress(QPointF pos)
{
    if (btnMenuNormal && btnMenuNormal->rect().contains(pos)) {
        dificultadSeleccionada = DificultadNivel2::NORMAL;
        iniciar();
    } else if (btnMenuDificil && btnMenuDificil->rect().contains(pos)) {
        dificultadSeleccionada = DificultadNivel2::DIFICIL;
        iniciar();
    } else if (btnMenuVolver && btnMenuVolver->rect().contains(pos)) {
        emit volverAlMenu();
    }
}

// ── Juego ─────────────────────────────────────────────────────────────────────

void MainWindowNivel2::setDificultad(DificultadNivel2 d)
{
    dificultadSeleccionada = d;
    if (nivel) nivel->setDificultad(d);
}

void MainWindowNivel2::iniciar()
{
    timer->stop();
    limpiarEscenaJuego();
    delete nivel;
    nivel = nullptr;

    acelerando = frenando = rotando = false;
    camaraX = fondoOffsetX = 0.f;
    tiempoImpacto = tiempoBono = tiempoParpadeo = 0.f;
    frameJugadorAnterior = frameRivalAnterior = -1;

    nivel = new Nivel2(this);
    nivel->setDificultad(dificultadSeleccionada);
    connect(nivel, &Nivel2::carreraFinalizada, this, &MainWindowNivel2::onCarreraFinalizada);
    connect(nivel, &Nivel2::impactoBanana,     this, &MainWindowNivel2::onImpactoBanana);
    connect(nivel, &Nivel2::bonoVelocidad,     this, &MainWindowNivel2::onBonoVelocidad);
    connect(nivel, &Nivel2::caidaEspalda,      this, &MainWindowNivel2::onCaidaEspalda);
    connect(nivel, &Nivel2::sonidoBanana,      this, &MainWindowNivel2::onSonidoBanana);
    connect(nivel, &Nivel2::sonidoLodo,        this, &MainWindowNivel2::onSonidoLodo);
    nivel->iniciar();
    sfxPartida->play();  // inicia música al comenzar

    construirEscenaJuego();
    vista->setScene(escenaJuego);
    ajustarVista();
    reloj.start();
    timer->start();
}

void MainWindowNivel2::construirEscenaJuego()
{
    delete escenaJuego;
    escenaJuego = new QGraphicsScene(this);
    escenaJuego->setSceneRect(0, 0, SCENE_W, SCENE_H);

    QPixmap fondoPix(":/nivel2/fondo.png");
    if (!fondoPix.isNull()) {
        QPixmap tile = fondoPix.scaled(SCENE_W, SCENE_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        fondoOrigin    = escenaJuego->addPixmap(tile); fondoOrigin->setPos(0,0);       fondoOrigin->setZValue(-2);
        fondoDesfasado = escenaJuego->addPixmap(tile); fondoDesfasado->setPos(SCENE_W,0); fondoDesfasado->setZValue(-2);
    }

    spriteJugador = new Sprite(":/nivel2/SpritesJugador-sf.png",      6, 1);
    spriteRival   = new Sprite(":/nivel2/SpritesRival-sf.png",        6, 1);
    spriteBanana  = new Sprite(":/general/SpriteBanana-sf.png",        1, 1);
    spriteRampa   = new Sprite(":/nivel2/SpriteRampa-sf.png",         1, 1);
    spriteLodo    = new Sprite(":/nivel2/SpriteLodo-sf.png",          1, 1);
    spriteImpacto = new Sprite(":/general/SpriteImpactoBanana-sf.png", 1, 1);

    itemJugador = escenaJuego->addPixmap(frameEscalado(spriteJugador, MOTO_W_PX, MOTO_H_PX));
    itemRival   = escenaJuego->addPixmap(frameEscalado(spriteRival,   RIVAL_W_PX, RIVAL_H_PX));
    itemBanana  = escenaJuego->addPixmap(frameEscalado(spriteBanana,  30, 30));
    itemImpacto = escenaJuego->addPixmap(frameEscalado(spriteImpacto, MOTO_W_PX, MOTO_H_PX));
    itemImpacto->setZValue(5); itemImpacto->setVisible(false);
    itemRampa   = escenaJuego->addPixmap(frameEscalado(spriteRampa,   MOTO_W_PX*4, MOTO_H_PX));
    itemLodo    = escenaJuego->addPixmap(frameEscalado(spriteLodo,    MOTO_W_PX*5, MOTO_H_PX*0.6f));
    itemBanana->setVisible(false);

    itemHUD = escenaJuego->addText("");
    itemHUD->setFont(QFont("Consolas", 14));
    itemHUD->setDefaultTextColor(Qt::white);
    itemHUD->setZValue(10);
    itemHUD->setPos(10, 10);

    itemDanio = escenaJuego->addText("");
    itemDanio->setFont(QFont("Arial", 20, QFont::Bold));
    itemDanio->setZValue(15);
    itemDanio->setVisible(false);

    // Imagen de resultado (oculta hasta terminar la carrera)
    itemResultado = escenaJuego->addPixmap(QPixmap());
    itemResultado->setZValue(10);
    itemResultado->setVisible(false);

    // Tres botones en la zona inferior de la imagen
    auto crearBtn = [&](float x, float y, float w, float h, QColor bg, QColor borde,
                        const QString &texto, QFont f) -> QGraphicsRectItem* {
        auto *btn = new QGraphicsRectItem(x, y, w, h);
        btn->setBrush(QBrush(bg)); btn->setPen(QPen(borde, 2));
        btn->setZValue(11); btn->setVisible(false);
        escenaJuego->addItem(btn);
        auto *lbl = escenaJuego->addText(texto, f);
        lbl->setDefaultTextColor(Qt::white); lbl->setZValue(12); lbl->setVisible(false);
        lbl->setPos(x + w/2 - lbl->boundingRect().width()/2, y + h/2 - lbl->boundingRect().height()/2);
        // guardamos el label en la escena — se controla por zValue
        return btn;
    };

    float bY = SCENE_H * 0.82f, bH = 52.f;
    btnReiniciar     = crearBtn(SCENE_W*0.05f, bY, SCENE_W*0.28f, bH, QColor(40,120,40,220),  QColor(80,200,80),  "Reintentar",     QFont("Arial",12,QFont::Bold));
    btnMenuNivel     = crearBtn(SCENE_W*0.36f, bY, SCENE_W*0.28f, bH, QColor(40,40,120,220),  QColor(80,80,200),  "Menú Nivel 2",   QFont("Arial",12,QFont::Bold));
    btnMenuPrincipal = crearBtn(SCENE_W*0.67f, bY, SCENE_W*0.28f, bH, QColor(120,40,40,220),  QColor(200,80,80),  "Menú Principal", QFont("Arial",12,QFont::Bold));
}

void MainWindowNivel2::limpiarEscenaJuego()
{
    delete spriteJugador; spriteJugador = nullptr;
    delete spriteRival;   spriteRival   = nullptr;
    delete spriteBanana;  spriteBanana  = nullptr;
    delete spriteRampa;   spriteRampa   = nullptr;
    delete spriteLodo;    spriteLodo    = nullptr;
    delete spriteImpacto; spriteImpacto = nullptr;
    fondoOrigin = fondoDesfasado = nullptr;
    itemJugador = itemRival = itemBanana = itemRampa = itemLodo = itemImpacto = nullptr;
    itemResultado = nullptr;
    itemHUD = itemDanio = nullptr;
    btnReiniciar = btnMenuNivel = btnMenuPrincipal = nullptr;
}

// ── Loop de juego ─────────────────────────────────────────────────────────────

void MainWindowNivel2::actualizar()
{
    if (!nivel || nivel->isCarreraTerminada()) return;

    float dt = reloj.elapsed() / 1000.f;
    reloj.restart();
    if (dt > 0.1f) dt = 0.1f;

    nivel->actualizar(dt);

    const JugadorNivel2 *j = nivel->getJugador();
    const RivalNivel2   *r = nivel->getRival();

    fondoOffsetX -= j->getVelocidadX() * dt * ESCALA;
    if (fondoOffsetX <= -SCENE_W) fondoOffsetX += SCENE_W;
    fondoOrigin->setPos(fondoOffsetX, 0);
    fondoDesfasado->setPos(fondoOffsetX + SCENE_W, 0);

    int frame = 0;
    if (!j->getEnSuelo())  frame = 5;
    else if (acelerando)   frame = 1;
    else if (frenando)     frame = 2;
    if (frame != frameJugadorAnterior) {
        spriteJugador->setFrame(frame);
        itemJugador->setPixmap(frameEscalado(spriteJugador, MOTO_W_PX, MOTO_H_PX));
        frameJugadorAnterior = frame;
    }

    camaraX = j->getPosX() * ESCALA - SCENE_W * 0.33f;
    if (camaraX < 0) camaraX = 0;

    float jugPx = logicaAPantallaX(j->getPosX());
    float jugPy = logicaAPantallaY(j->getPosY(), MOTO_H_PX);
    itemJugador->setPos(jugPx, jugPy);
    itemJugador->setRotation(-j->getAnguloSprite());

    if (tiempoParpadeo > 0.f) {
        tiempoParpadeo -= dt;
        itemJugador->setVisible((int)(tiempoParpadeo / FREQ_PARPADEO) % 2 == 0);
        if (tiempoParpadeo <= 0.f) { tiempoParpadeo = 0.f; itemJugador->setVisible(true); }
    }

    if (tiempoImpacto > 0.f) {
        tiempoImpacto -= dt;
        itemImpacto->setPos(jugPx, jugPy);
        itemDanio->setPos(jugPx, jugPy - 40);
        if (tiempoImpacto <= 0.f) { tiempoImpacto = 0.f; itemImpacto->setVisible(false); itemDanio->setVisible(false); }
    } else if (tiempoBono > 0.f) {
        tiempoBono -= dt;
        itemDanio->setPos(jugPx, jugPy - 40);
        if (tiempoBono <= 0.f) { tiempoBono = 0.f; itemDanio->setVisible(false); }
    }

    int frameRival = r->estaAtacando() ? r->getFrameAtaque() : (!r->getEnSuelo() ? 5 : 1);
    if (frameRival != frameRivalAnterior) {
        spriteRival->setFrame(frameRival);
        itemRival->setPixmap(frameEscalado(spriteRival, RIVAL_W_PX, RIVAL_H_PX));
        frameRivalAnterior = frameRival;
    }
    itemRival->setPos(logicaAPantallaX(r->getPosX()), logicaAPantallaY(r->getPosY(), RIVAL_H_PX));

    const Proyectil *b = r->getBanana();
    if (b->getActiva()) {
        itemBanana->setPos(logicaAPantallaX(b->getPosX()), logicaAPantallaY(b->getPosY(), 30));
        itemBanana->setVisible(true);
    } else { itemBanana->setVisible(false); }

    itemHUD->setPlainText(
        QString("Jugador — Vel: %1 px/s\nEnemigo  — Vel: %2 px/s")
            .arg(j->getVelocidadX(), 0, 'f', 1)
            .arg(r->getVelocidadX(), 0, 'f', 1));

    const auto &obs = nivel->getObstaculos();
    if (obs.size() >= 2) {
        itemRampa->setPos(logicaAPantallaX(obs[0]->getPosX()), logicaAPantallaY(0, MOTO_H_PX));
        itemLodo->setPos(logicaAPantallaX(obs[1]->getPosX()), logicaAPantallaY(0, MOTO_H_PX*0.6f));
    }
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void MainWindowNivel2::onImpactoBanana(float velocidadPerdida)
{
    tiempoImpacto = DURACION_IMPACTO; tiempoBono = 0.f;
    itemImpacto->setVisible(true);
    itemDanio->setDefaultTextColor(QColor(255, 80, 0));
    itemDanio->setPlainText(QString("-%1 px/s").arg(velocidadPerdida, 0, 'f', 1));
    itemDanio->setVisible(true);
}

void MainWindowNivel2::onCaidaEspalda()
{
    tiempoParpadeo = DURACION_PARPADEO;
}

void MainWindowNivel2::onBonoVelocidad(float velocidadGanada)
{
    tiempoBono = DURACION_IMPACTO; tiempoImpacto = 0.f;
    itemImpacto->setVisible(false);
    itemDanio->setDefaultTextColor(QColor(0, 230, 80));
    itemDanio->setPlainText(QString("+%1 px/s").arg(velocidadGanada, 0, 'f', 1));
    itemDanio->setVisible(true);
}

void MainWindowNivel2::onSonidoBanana()  { sfxBanana->play(); }
void MainWindowNivel2::onSonidoLodo()   { sfxLodo->play(); }

void MainWindowNivel2::onCarreraFinalizada(bool gano)
{
    timer->stop();
    sfxPartida->stop();

    // Mostrar imagen de resultado a pantalla completa
    QString rutaImg = gano ? ":/nivel2/VictoriaNivel2.png" : ":/nivel2/DerrotaNivel2.png";
    QPixmap img(rutaImg);
    if (!img.isNull()) {
        itemResultado->setPixmap(img.scaled(SCENE_W, SCENE_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        itemResultado->setPos(0, 0);
        itemResultado->setVisible(true);
    }

    // Mostrar botones — recorremos la escena para hacer visibles los labels también
    btnReiniciar->setVisible(true);
    btnMenuNivel->setVisible(true);
    btnMenuPrincipal->setVisible(true);
    for (auto *item : escenaJuego->items()) {
        if (auto *txt = dynamic_cast<QGraphicsTextItem*>(item)) {
            if (txt->zValue() == 12) txt->setVisible(true);
        }
    }
}

// ── Eventos ───────────────────────────────────────────────────────────────────

bool MainWindowNivel2::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == vista && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPointF pos = vista->mapToScene(me->pos());

        // Estamos en el menú
        if (vista->scene() == escenaMenu) {
            onMenuMousePress(pos);
            return false;
        }

        // Botones de resultado
        if (btnReiniciar && btnReiniciar->isVisible() && btnReiniciar->rect().contains(pos)) {
            iniciar();
            return false;
        }
        if (btnMenuNivel && btnMenuNivel->isVisible() && btnMenuNivel->rect().contains(pos)) {
            mostrarMenu();
            return false;
        }
        if (btnMenuPrincipal && btnMenuPrincipal->isVisible() && btnMenuPrincipal->rect().contains(pos)) {
            timer->stop();
            emit volverAlMenuPrincipal();
            return false;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindowNivel2::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (vista->scene() == escenaJuego) { timer->stop(); mostrarMenu(); }
        else emit volverAlMenu();
        return;
    }
    if (event->key() == Qt::Key_F11) { isFullScreen() ? showMaximized() : showFullScreen(); return; }
    if (vista->scene() != escenaJuego) return;
    if (nivel) nivel->procesarTecla(event->key(), true);
    if (event->key() == Qt::Key_D)      acelerando = true;
    else if (event->key() == Qt::Key_A) frenando   = true;
    else if (event->key() == Qt::Key_W) rotando    = true;
}

void MainWindowNivel2::keyReleaseEvent(QKeyEvent *event)
{
    if (vista->scene() != escenaJuego) return;
    if (nivel) nivel->procesarTecla(event->key(), false);
    if (event->key() == Qt::Key_D)      acelerando = false;
    else if (event->key() == Qt::Key_A) frenando   = false;
    else if (event->key() == Qt::Key_W) rotando    = false;
    QMainWindow::keyReleaseEvent(event);
}

void MainWindowNivel2::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); ajustarVista();
}

// ── Helpers ───────────────────────────────────────────────────────────────────

QPixmap MainWindowNivel2::frameEscalado(Sprite *s, int wPx, int hPx) const {
    return s->getPixmap()->copy(s->getRectFrame().toRect())
        .scaled(wPx, hPx, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

float MainWindowNivel2::logicaAPantallaX(float x) const { return x * ESCALA - camaraX; }
float MainWindowNivel2::logicaAPantallaY(float y, float spriteH) const { return SUELO_PX - spriteH - (y * ESCALA); }
void  MainWindowNivel2::ajustarVista() {
    QGraphicsScene *s = vista->scene();
    if (s) vista->fitInView(s->sceneRect(), Qt::KeepAspectRatio);
}
