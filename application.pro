QT += core gui network webengine widgets webenginewidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    atextedit.h \
    settings.h \
    statsview.h
SOURCES       = main.cpp \
                atextedit.cpp \
                mainwindow.cpp \
                settings.cpp \
                statsview.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target

FORMS += \
    settings.ui \
    statsview.ui

DISTFILES += \
    doc/Control Code Assign.txt \
    style.css

unix:!macx: LIBS += -L$$PWD/../Builds/ -llibkqfn

INCLUDEPATH += $$PWD/../Builds
DEPENDPATH += $$PWD/../Builds

TRANSLATIONS = QtAp_de.ts \
               QtAp_nl.ts \
               QtAp_en.ts \
               QtAp_fr.ts \
               QtAp_pl.ts
