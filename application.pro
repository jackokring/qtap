QT += core gui network webengine widgets webenginewidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    adialog.h \
    atextedit.h \
    aviewwidget.h \
    calculus.h \
    classfilter.h \
    finddialog.h \
    sais.h \
    settings.h \
    statsview.h \
    utfdialog.h
SOURCES       = main.cpp \
                adialog.cpp \
                atextedit.cpp \
                aviewwidget.cpp \
                calculus.cpp \
                classfilter.cpp \
                finddialog.cpp \
                mainwindow.cpp \
                sais.cpp \
                settings.cpp \
                statsview.cpp \
                utfdialog.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target

FORMS += \
    finddialog.ui \
    settings.ui \
    statsview.ui \
    utfdialog.ui

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
                QtAp_zh.ts \

