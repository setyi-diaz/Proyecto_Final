#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>

class Nivel : public QObject {
    Q_OBJECT
public:
    explicit Nivel(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~Nivel() = default;

    virtual void iniciar()                    = 0;
    virtual void actualizar(float dt)         = 0;
    virtual void procesarTecla(int key, bool presionada) = 0;

    bool isJuegoActivo()      const { return juegoActivo; }
    bool isCarreraTerminada() const { return carreraTerminada; }
    bool isGanado()           const { return gano; }

signals:
    void actualizado();
    void carreraFinalizada(bool gano);

protected:
    bool       juegoActivo      = false;
    bool       carreraTerminada = false;
    bool       gano             = false;
};

#endif // NIVEL_H
