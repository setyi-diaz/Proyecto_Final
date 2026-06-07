#ifndef MAINWINDOWNIVEL1_H
#define MAINWINDOWNIVEL1_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QPair>
#include <QColor>
#include <QSoundEffect>
#include "nivel1.h"
#include "sprite.h"
#include "corredornivel1.h"
#include "item.h"

struct EntradaCorredor {
    QGraphicsPixmapItem *item   = nullptr;
    Sprite              *sprite = nullptr;
    int targetW = 60, targetH  = 80;

    void setSpriteIndex(int index) {
        if (!sprite || !item) return;
        sprite->setFrame(index);
        QPixmap frame = sprite->getFrameScaled(targetW, targetH);
        item->setPixmap(frame);
        item->setOffset(-frame.width() / 2.0f, -frame.height() / 2.0f);
    }
};

class MainWindowNivel1 : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowNivel1(QWidget *parent = nullptr);
    ~MainWindowNivel1();

    void liberarRecursos();

signals:
    void volverAlMenu();
    void irAlNivel2();

protected:
    void keyPressEvent(QKeyEvent *event)   override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event)  override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void iniciarNivel();
    void reiniciarNivel();
    void onVolver();
    void onActualizado();
    void onCuentaRegresiva(int valor);
    void onJuegoIniciado();
    void onCarreraFinalizada(bool gano);
    void onObstaculoCreado(int index);
    void onEfectoCreado(int index);
    void onTextoFlotante(const QString &texto, float x, float y, const QColor &color);
    void onSonidoRotacion();
    void onSonidoBoost();

private:
    QGraphicsView  *vista  = nullptr;
    Nivel1         *nivel  = nullptr;

    // Escenas
    QGraphicsScene *escenaMenu  = nullptr;
    QGraphicsScene *escenaNivel = nullptr;

    // Estado del menú
    QList<CorredorNivel1::Personalidad> personalidadesMenu;
    int              numCarrilesMenu = 5;
    int              numRivalesMenu  = 2;
    DificultadNivel1 dificultadMenu  = DificultadNivel1::NORMAL;
    int              indicesMenu[8]  = {};

    struct TarjetaMenu {
        float y = 0;
        QGraphicsTextItem *textoNombre     = nullptr;
        QGraphicsRectItem *barras[4]       = {};
        QGraphicsTextItem *textosBarras[4] = {};
        QGraphicsRectItem *btnIzq = nullptr;
        QGraphicsRectItem *btnDer = nullptr;
    };
    static const int MAX_SLOTS_MENU = 8;
    TarjetaMenu tarjetasMenu[MAX_SLOTS_MENU];

    QGraphicsRectItem *btnNormalMenu      = nullptr;
    QGraphicsRectItem *btnDificilMenu     = nullptr;
    QGraphicsTextItem *textoDificultadMenu= nullptr;
    QGraphicsRectItem *btnCarrilMenosMenu = nullptr;
    QGraphicsRectItem *btnCarrilMasMenu   = nullptr;
    QGraphicsTextItem *textoCarrilesMenu  = nullptr;
    QGraphicsRectItem *btnRivalMenosMenu  = nullptr;
    QGraphicsRectItem *btnRivalMasMenu    = nullptr;
    QGraphicsTextItem *textoRivalesMenu   = nullptr;
    QGraphicsTextItem *textoLateralMenu   = nullptr;
    QGraphicsRectItem *btnIniciarMenu     = nullptr;
    QGraphicsTextItem *lblStartMenu       = nullptr;
    QGraphicsRectItem *btnVolverMenu      = nullptr;

    // Gráficos del nivel
    EntradaCorredor             jugadorGrafico;
    QList<EntradaCorredor>      rivalesGrafico;
    QList<QGraphicsPixmapItem*> itemsObstaculos;
    QList<QGraphicsPixmapItem*> itemsEfectos;
    QGraphicsRectItem   *rectPista    = nullptr;
    QGraphicsPixmapItem *fondoTile1   = nullptr;
    QGraphicsPixmapItem *fondoTile2   = nullptr;
    QGraphicsTextItem   *textoControles = nullptr;
    QGraphicsTextItem   *textoInfo      = nullptr;
    QGraphicsTextItem   *textoCuenta    = nullptr;
    QGraphicsTextItem   *textoTiempo    = nullptr;
    QGraphicsRectItem   *btnReiniciar   = nullptr;
    QGraphicsRectItem   *btnSiguiente   = nullptr;
    QGraphicsRectItem   *btnMenuFin     = nullptr;
    bool carreraTerminada = false;

    QList<QPair<QGraphicsTextItem*, float>> textosFlotantes;
    float dt = 0.016f;

    // Audio
    QSoundEffect *sfxRotacion = nullptr;
    QSoundEffect *sfxBoost    = nullptr;
    QSoundEffect *musicaFondo = nullptr;

    // Constantes
    static constexpr float ESCENA_ANCHO    = 680.0f;
    static constexpr float ESCENA_ALTO     = 850.0f;
    static constexpr float TILE_ALTO       = 850.0f;
    static constexpr float SPRITE_ESCALA_W = 0.75f;
    static constexpr float SPRITE_ESCALA_H = 1.4f;

    // Helpers menú
    void construirEscenaMenu();
    void reconstruirMenuUI();
    void construirPanelIzquierdoMenu();
    void construirPanelDerechoMenu();
    void crearTarjetaMenu(int slot, float y, float tarjetaH);
    void actualizarTarjetaMenu(int slot);
    void actualizarMensajeLateralMenu();
    void onMenuMousePress(QPointF pos);

    // Helpers nivel
    void construirEscenaNivel();
    void limpiarEscenaNivel();
    void actualizarTextosFlotantes();
    QGraphicsPixmapItem* crearItemObstaculo(const Item *obs);
    EntradaCorredor      crearItemCorredor(int tw, int th, const QString &ruta, float x, float y);
    void ajustarVista();
};

#endif // MAINWINDOWNIVEL1_H
