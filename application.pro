QT += core gui network webengine widgets webenginewidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    AlgorithmsKG/calculus.h \
    AlgorithmsKG/sais.h \
    DatatypesTG/maps.h \
    JoineryTS/adialog.h \
    JoineryTS/atextedit.h \
    JoineryTS/aviewwidget.h \
    JoineryTS/finddialog.h \
    JoineryTS/settings.h \
    JoineryTS/statsview.h \
    JoineryTS/utfdialog.h
SOURCES       = main.cpp \
                AlgorithmsKG/calculus.cpp \
                AlgorithmsKG/sais.cpp \
                DatatypesTG/maps.cpp \
                JoineryTS/adialog.cpp \
                JoineryTS/atextedit.cpp \
                JoineryTS/aviewwidget.cpp \
                JoineryTS/finddialog.cpp \
                JoineryTS/settings.cpp \
                JoineryTS/statsview.cpp \
                JoineryTS/utfdialog.cpp \
                mainwindow.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target

FORMS += \
    JoineryTS/finddialog.ui \
    JoineryTS/settings.ui \
    JoineryTS/statsview.ui \
    JoineryTS/utfdialog.ui

DISTFILES += \
    Control Code Assign.txt \
    style.css

unix:!macx: LIBS += -L$$PWD/../Builds/ -llibkqfn

INCLUDEPATH += $$PWD/../Builds
DEPENDPATH += $$PWD/../Builds

TRANSLATIONS =  QtAp_de.ts \
                QtAp_nl.ts \
                QtAp_en.ts \
                QtAp_fr.ts \
                QtAp_pl.ts \
                QtAp_es.ts \
                QtAp_da.ts \
                QtAp_fi.ts \
                QtAp_ja.ts \
                QtAp_zh.ts

