QT += widgets
CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += console
#QMAKE_CXXFLAGS += /std:c++17

TEMPLATE = app

# Include headers from include/ and console/
INCLUDEPATH += include \
               console

# Source files for gui/
SOURCES += \
    gui/jetonfauneitem.cpp \
    gui/piocheitem.cpp \

# Source files in src/
SOURCES += \
    src/carte_marquage.cpp \
    src/enums.cpp \
    src/env_joueur.cpp \
    src/gestion_pieces.cpp \
    src/jeton_faune.cpp \
    src/partie.cpp \
    src/pioche.cpp \
    src/position.cpp \
    src/score.cpp \
    src/tuile.cpp \
    gui/main_qt.cpp

# Source files from console (optional unless reused in GUI)
SOURCES += \
    console/affichage.cpp \
    console/gestion_partie.cpp \
    console/main_console.cpp \
    console/saisie.cpp

# Header files for gui/
HEADERS += \
    gui/jetonfauneitem.h \
    gui/piocheitem.h \

# Header files in include/
HEADERS += \
    include/carte_marquage.h \
    include/enums.h \
    include/env_joueur.h \
    include/gestion_pieces.h \
    include/jeton_faune.h \
    include/partie.h \
    include/pioche.h \
    include/position.h \
    include/score.h \
    include/tuile.h \
    include/json.hpp  # Header-only lib

# Header files from console
HEADERS += \
    console/affichage.h \
    console/gestion_partie.h \
    console/saisie.h

# Qt resource file
RESOURCES += \
    gui/resources.qrc \
