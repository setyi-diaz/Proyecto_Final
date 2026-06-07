#ifndef NIVEL1_H
#define NIVEL1_H

#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QColor>
#include "nivel.h"
#include "jugadornivel1.h"
#include "rivalnivel1.h"
#include "item.h"

enum class DificultadNivel1 { NORMAL, DIFICIL };

struct EstadoRival {
    RivalNivel1 *rival  = nullptr;
    float progreso      = 0;
    float velBase       = 0;
    float velActual     = 0;
    float muExtra       = 0;
    float tiempoLodo    = 0;
    float durLodo       = 0;
    float fBoost        = 0;
    float tiempoBoost   = 0;
    float durBoost      = 0;
    QColor color;
};

class Nivel1 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1(int numCarriles, int numRivales, QObject *parent = nullptr);
    ~Nivel1();

    void iniciar()                              override;
    void actualizar(float dt)                   override;
    void procesarTecla(int key, bool presionada) override;
    void setDificultad(DificultadNivel1 d);
    void setPersonalidades(const CorredorNivel1::Personalidad &pJugador,
                           const QList<CorredorNivel1::Personalidad> &pRivales);

    DificultadNivel1 getDificultad() const { return dificultad; }

    JugadorNivel1*              getJugador()           const;
    const QList<EstadoRival>&   getRivales()           const;
    const QList<Item*>&  getObstaculos()        const;
    const QList<Item*>&  getEfectos()           const;
    float  getProgresoJugador()     const;
    float  getVelActualJugador()    const;
    float  getTiempoRestante()      const;
    float  getCooldownGolpe()       const;
    float  getVelocidadObstaculos() const;
    float  getFondoOffsetY()        const;
    int    getNumCarriles()         const;
    float  getAnchoCarril()         const;
    float  getMargenX()             const;
    int    getCuentaRegresiva()     const;

signals:
    void cuentaRegresivaCambio(int valor);
    void juegoIniciado();
    void obstaculoCreado(int index);
    void efectoCreado(int index);
    void textoFlotanteCreado(const QString &texto, float x, float y, const QColor &color);
    void sonidoRotacion();
    void sonidoBoost();

private slots:
    void tickInterno();
    void contarRegresiva();

private:
    void crearRival(int carril, const CorredorNivel1::Personalidad &p, const QColor &color);
    void generarObstaculos();
    void reciclarEfecto(int index);
    void inicializarPoolEfectos();
    void actualizarColisiones();
    void verificarVictoria();
    void aumentarVelocidad();
    void disminuirVelocidad();
    void golpearRival(int direccion);
    bool carrilLibre(int carril) const;

    int   numCarriles;
    int   numRivalesConfig;
    float anchoCarril;
    float margenX;

    JugadorNivel1       *jugador;
    QList<EstadoRival>   rivales;
    QList<Item*>  obstaculos;
    QList<Item*>  efectos;

    CorredorNivel1::Personalidad personalidadJugador;

    float progresoJugador;
    float velActualJugador;
    float velBaseJugador;
    float muExtraJugador;
    float tiempoLodoJugador;
    float durLodoJugador;
    float fBoostJugador;
    float tiempoBoostJugador;
    float durBoostJugador;

    float tiempoTotal;
    float tiempoRestante;

    QTimer        *timerJuego;
    QTimer        *timerRegresiva;
    QElapsedTimer  reloj;

    int   cuentaRegresiva;
    float dtInterno;
    float tiempoObstaculo;
    float tiempoEsperaBanana = 0.f;
    float intervaloObstaculos;

    static constexpr int POOL_BANANAS = 6;  // pool fijo de bananas
    float cooldownGolpe;
    float velocidadObstaculos;
    float fondoOffsetY;

    DificultadNivel1 dificultad;
};

#endif // NIVEL1_H
