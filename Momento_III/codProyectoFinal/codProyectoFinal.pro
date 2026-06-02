QT += core gui widgets

TARGET = codProyectoFinal
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    menupersonalidades.cpp \
    nivel1.cpp \
    personaje.cpp \
    corredornivel1.cpp \
    jugadornivel1.cpp \
    rivalnivel1.cpp \
    pista.cpp \
    personalidad.cpp \
    objetopista.cpp

HEADERS += \
    mainwindow.h \
    menupersonalidades.h \
    nivel1.h \
    personaje.h \
    corredornivel1.h \
    jugadornivel1.h \
    rivalnivel1.h \
    pista.h \
    personalidad.h \
    objetopista.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
