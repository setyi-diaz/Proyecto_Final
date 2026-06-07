#ifndef CORREDORNIVEL1_H
#define CORREDORNIVEL1_H

#include "personaje.h"
#include <QColor>
#include <QString>
#include <QList>

class CorredorNivel1 : public Personaje {
public:
    struct Personalidad {
        QString nombre;
        float   modAceleracion;
        float   modVelMax;
        float   resistenciaGolpe;
        float   fuerzaGolpe;

        static QList<Personalidad> disponibles() {
            return {
                { "Simio Veloz",     1.05f, 1.0f,  0.65f, 0.6f  },
                { "Simio de Acero",  0.95f, 0.93f, 1.6f,  1.05f },
                { "Simio Golpeador", 1.0f,  1.0f,  0.85f, 1.55f },
            };
        }
    };

    CorredorNivel1();
    virtual ~CorredorNivel1();

    virtual void acelerar(float dt) = 0;
    virtual void frenar(float dt) {}
    void actualizarAnimaciones(float dt);

    int  getCarrilActual() const;
    void cambiarCarril(int nuevoCarril);
    void recibirGolpe(float fuerza);
    void aplicarImpulso(float impulso);

    bool estaDesacelerado() const;

    void activarDesaceleracion(float duracion);
    void activarGolpeRecibido(float duracion);
    void activarGolpeAnimacion(float duracion, int indiceSpriteGolpe);
    void activarRotacion(float duracion);
    void activarRotacionAgua(float velocidadAngularInicial, float amortiguacion);

    bool   estaRotandoAgua()   const;
    float  getRotacionActual() const;
    int    getSpriteIndex()    const;
    QColor getColorBase()      const;

    float getFuerzaGolpe()      const;
    float getResistenciaGolpe() const;

    void setColorBase(const QColor &color);

    void         setPersonalidad(const Personalidad &p);
    bool         tienePersonalidad() const;
    Personalidad getPersonalidad()   const;
    void         aplicarPersonalidad();
    QString      getRutaSprite()     const;

    float getVelocidadAvance() const;

protected:
    virtual QString rutaSprite() const = 0;

    float  velocidadAvance  = 0.f;

    int    carrilActual     = 1;
    float  resistenciaGolpe = 1.0f;
    float  fuerzaGolpe      = 50.0f;
    QColor colorBase;

    float tiempoDesaceleracion = 0.f;
    float tiempoGolpeRecibido  = 0.f;
    float tiempoRotacion       = 0.f;
    float duracionRotacion     = 1.2f;
    float anguloRotacion       = 0.f;

    float omegaAgua     = 0.f;
    float kAmortAgua    = 2.5f;
    float anguloAcumAgua = 0.f;

    int spriteGolpe = 0;
    int frameActual = 0;

    bool         hayPersonalidad   = false;
    Personalidad personalidadActual{};
};

#endif // CORREDORNIVEL1_H
