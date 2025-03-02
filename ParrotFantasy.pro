QT       += core gui
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game.cpp \
    main.cpp \
    musicmanager.cpp \
    rule.cpp \
    widget.cpp \
    Attacker.cpp \
    Character.cpp \
    Ghost.cpp \
    Lattice.cpp \
    Map.cpp

HEADERS += \
    game.h \
    musicmanager.h \
    rule.h \
    widget.h \
    Attacker.h \
    Character.h \
    Ghost.h \
    Lattice.h \
    Map.h

FORMS += \
    game.ui \
    rule.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
