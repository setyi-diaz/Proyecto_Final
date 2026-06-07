#ifndef MAINWINDOWNIVEL2_H
#define MAINWINDOWNIVEL2_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QElapsedTimer>
#include <QSoundEffect>
#include "nivel2.h"
#include "sprite.h"

class MainWindowNivel2 : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowNivel2(QWidget *parent = nullptr);
    ~MainWindowNivel2();

    void iniciar();
    void setDificultad(DificultadNivel2 d);
    void resetearAlMenu();
    void liberarRecursos();

signals:
    void volverAlMenu();
    void volverAlMenuPrincipal();

protected:
    void keyPressEvent(QKeyEvent *event)   override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event)  override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void actualizar();
    void onCarreraFinalizada(bool gano);
    void onImpactoBanana(float velocidadPerdida);
    void onBonoVelocidad(float velocidadGanada);
    void onCaidaEspalda();
    void onSonidoBanana();
    void onSonidoLodo();

private:
    QGraphicsView  *vista      = nullptr;
    QGraphicsScene *escenaMenu = nullptr;
    QGraphicsScene *escenaJuego = nullptr;
    Nivel2         *nivel  = nullptr;
    QTimer         *timer  = nullptr;
    QElapsedTimer   reloj;

    QGraphicsPixmapItem *fondoOrigin    = nullptr;
    QGraphicsPixmapItem *fondoDesfasado = nullptr;
    QGraphicsPixmapItem *itemJugador    = nullptr;
    QGraphicsPixmapItem *itemRival      = nullptr;
    QGraphicsPixmapItem *itemBanana     = nullptr;
    QGraphicsPixmapItem *itemRampa      = nullptr;
    QGraphicsPixmapItem *itemLodo       = nullptr;
    QGraphicsPixmapItem *itemImpacto    = nullptr;
    QGraphicsPixmapItem *itemResultado   = nullptr;
    QGraphicsTextItem   *itemHUD        = nullptr;
    QGraphicsTextItem   *itemDanio      = nullptr;
    // Botones pantalla de resultado
    QGraphicsRectItem   *btnReiniciar   = nullptr;
    QGraphicsRectItem   *btnMenuNivel   = nullptr;
    QGraphicsRectItem   *btnMenuPrincipal = nullptr;

    // Audio
    QSoundEffect *sfxPartida = nullptr;
    QSoundEffect *sfxBanana  = nullptr;
    QSoundEffect *sfxLodo    = nullptr;

    Sprite *spriteJugador = nullptr;
    Sprite *spriteRival   = nullptr;
    Sprite *spriteBanana  = nullptr;
    Sprite *spriteRampa   = nullptr;
    Sprite *spriteLodo    = nullptr;
    Sprite *spriteImpacto = nullptr;

    float tiempoImpacto   = 0.f;
    float tiempoBono      = 0.f;
    float tiempoParpadeo  = 0.f;
    static constexpr float DURACION_IMPACTO  = 0.6f;
    static constexpr float DURACION_PARPADEO = 2.0f;
    static constexpr float FREQ_PARPADEO     = 0.1f;

    // Cache de pixmaps para evitar regenerar cada frame
    int    frameJugadorAnterior = -1;
    int    frameRivalAnterior   = -1;

    bool  acelerando  = false;
    bool  frenando    = false;
    bool  rotando     = false;
    float camaraX     = 0.f;
    float fondoOffsetX = 0.f;

    static constexpr float ESCALA    = 16.f;
    static constexpr float SCENE_W   = 1600.f;
    static constexpr float SCENE_H   = 900.f;
    static constexpr float SUELO_PX  = 650.f;
    static constexpr float MOTO_W_PX = 150.f;
    static constexpr float MOTO_H_PX = 160.f;
    static constexpr float RIVAL_W_PX = 175.f;
    static constexpr float RIVAL_H_PX = 185.f;

    // Menú interno
    QGraphicsRectItem *btnMenuNormal  = nullptr;
    QGraphicsRectItem *btnMenuDificil = nullptr;
    QGraphicsRectItem *btnMenuVolver  = nullptr;
    DificultadNivel2   dificultadSeleccionada = DificultadNivel2::NORMAL;

    void    construirEscenaMenu();
    void    construirEscenaJuego();
    void    limpiarEscenaJuego();
    void    mostrarMenu();
    float   logicaAPantallaX(float x) const;
    float   logicaAPantallaY(float y, float spriteH) const;
    QPixmap frameEscalado(Sprite *s, int wPx, int hPx) const;
    void    ajustarVista();
    void    onMenuMousePress(QPointF pos);
};

#endif // MAINWINDOWNIVEL2_H
