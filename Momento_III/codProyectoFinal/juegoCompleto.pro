QT += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    juego.cpp \
    menuwidget.cpp \
    mainwindownivel1.cpp \
    mainwindownivel2.cpp \
    personaje.cpp \
    jugadornivel2.cpp \
    rivalnivel2.cpp \
    nivel1.cpp \
    nivel2.cpp \
    corredornivel1.cpp \
    jugadornivel1.cpp \
    rivalnivel1.cpp \
    sprite.cpp \
    item.cpp \
    itembanana.cpp \
    itemobstaculo.cpp \
    proyectil.cpp \
    obstaculo.cpp \
    obstaculofriccion.cpp \
    rampa.cpp

HEADERS += \
    juego.h \
    menuwidget.h \
    mainwindownivel1.h \
    mainwindownivel2.h \
    personaje.h \
    jugadornivel2.h \
    rivalnivel2.h \
    nivel.h \
    nivel1.h \
    nivel2.h \
    corredornivel1.h \
    jugadornivel1.h \
    rivalnivel1.h \
    sprite.h \
    item.h \
    itembanana.h \
    itemobstaculo.h \
    personalidad.h \
    proyectil.h \
    obstaculo.h \
    obstaculofriccion.h \
    rampa.h

RESOURCES += \
    pictures.qrc
