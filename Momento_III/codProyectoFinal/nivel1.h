#ifndef NIVEL1_H
#define NIVEL1_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QPair>
#include <QColor>
#include <QGraphicsSceneMouseEvent>

class JugadorNivel1;
class RivalNivel1;
class Pista;
class Obstaculo;
class ObjetoEfecto;
class Personalidad;

struct EstadoRival {
    RivalNivel1 *rival = nullptr;
    float progreso = 0;
    float velBase = 0;
    float impulsoVel = 0;
    float tiempoImpulsoVel = 0;
    QColor color;
};

class Nivel1 : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Nivel1(int numCarriles, int numRivales, QObject *parent = nullptr);
    ~Nivel1();

    void iniciar();
    void setPersonalidades(Personalidad *personalidadJugador, QList<Personalidad*> personalidadesRivales);
    JugadorNivel1* getJugador() const;
    int getNumCarriles() const;
    bool carrilLibre(int carril) const;

    void aumentarVelocidad();
    void disminuirVelocidad();
    void golpearRival(int direccion);

signals:
    void reiniciarSolicitado();
    void volverMenuSolicitado();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void actualizar();
    void iniciarCuentaRegresiva();
    void contarRegresiva();

private:
    void crearRival(int carril, Personalidad *personalidad, const QColor &color);
    void generarObstaculos();
    void generarBanana();
    void actualizarColisiones();
    void verificarVictoria();
    void actualizarTextosFlotantes();
    void spawnTextoFlotante(const QString &texto, float x, float y, const QColor &color);
    void finalizarCarrera(const QString &mensaje, const QColor &color);

    int numCarriles;
    int numRivalesConfig;
    float anchoCarril;
    float margenX;

    JugadorNivel1 *jugador;
    QList<EstadoRival> rivales;
    Pista *pista;
    QList<Obstaculo*> obstaculos;
    QList<ObjetoEfecto*> efectos;

    QGraphicsTextItem *textoControles;
    QGraphicsTextItem *textoInfo;
    QGraphicsTextItem *textoCuenta;
    QGraphicsTextItem *textoTiempo;

    Personalidad *personalidadJugador;

    float progresoJugador;
    float velocidadDrift;
    float velBaseJugador;
    float scrollOffset;
    float impulsoJugador;
    float tiempoImpulsoJugador;
    float frenadaJugador;
    float tiempoFrenadaJugador;

    float tiempoTotal;
    float tiempoRestante;

    QTimer *timerJuego;
    QTimer *timerRegresiva;
    QElapsedTimer reloj;

    int cuentaRegresiva;
    bool juegoActivo;
    float dt;
    float tiempoObstaculo;
    float tiempoBanana;
    float intervaloObstaculos;

    float cooldownGolpe;

    QList<QPair<QGraphicsTextItem*, float>> textosFlotantes;

    // Botones post-carrera
    QGraphicsRectItem *btnReiniciar;
    QGraphicsRectItem *btnMenu;
    bool carreraTerminada;
};

#endif // NIVEL1_H
