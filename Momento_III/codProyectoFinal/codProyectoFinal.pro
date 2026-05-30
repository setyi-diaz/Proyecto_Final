QT += core gui widgets

TARGET = codProyectoFinal
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    personaje.cpp \
    corredornivel1.cpp \
    jugadornivel1.cpp \
    rivalnivel1.cpp \
    pista.cpp \
    personalidad.cpp \
    nivel1.cpp \
    obstaculo.cpp \
    objetopista.cpp \
    objetoefecto.cpp \
    menupersonalidades.cpp

HEADERS += \
    mainwindow.h \
    personaje.h \
    corredornivel1.h \
    jugadornivel1.h \
    rivalnivel1.h \
    pista.h \
    personalidad.h \
    nivel1.h \
    obstaculo.h \
    objetopista.h \
    objetoefecto.h \
    menupersonalidades.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
