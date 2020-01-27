QT += core gui network webengine widgets webenginewidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    AlgorithmsKG/calculus.h \
    AlgorithmsKG/sais.h \
    DatatypesTG/lists.h \
    DatatypesTG/maps.h \
    DatatypesTG/reducers.h \
    JoineryTS/adialog.h \
    JoineryTS/atextedit.h \
    JoineryTS/aviewwidget.h \
    JoineryTS/bloat.h \
    JoineryTS/elements.h \
    JoineryTS/finddialog.h \
    JoineryTS/hello.h \
    JoineryTS/internut.h \
    JoineryTS/jockey.h \
    JoineryTS/settings.h \
    JoineryTS/statsview.h \
    JoineryTS/turing.h \
    JoineryTS/utfdialog.h \
    discussion.h \
    kringlicence.h
SOURCES       = main.cpp \
                AlgorithmsKG/calculus.cpp \
                AlgorithmsKG/sais.cpp \
                DatatypesTG/lists.cpp \
                DatatypesTG/maps.cpp \
                DatatypesTG/reducers.cpp \
                JoineryTS/adialog.cpp \
                JoineryTS/atextedit.cpp \
                JoineryTS/aviewwidget.cpp \
                JoineryTS/bloat.cpp \
                JoineryTS/elements.cpp \
                JoineryTS/finddialog.cpp \
                JoineryTS/hello.cpp \
                JoineryTS/internut.cpp \
                JoineryTS/jockey.cpp \
                JoineryTS/settings.cpp \
                JoineryTS/statsview.cpp \
                JoineryTS/turing.cpp \
                JoineryTS/utfdialog.cpp \
                mainwindow.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target

FORMS += \
    JoineryTS/bloat.ui \
    JoineryTS/elements.ui \
    JoineryTS/finddialog.ui \
    JoineryTS/hello.ui \
    JoineryTS/internut.ui \
    JoineryTS/jockey.ui \
    JoineryTS/settings.ui \
    JoineryTS/statsview.ui \
    JoineryTS/turing.ui \
    JoineryTS/utfdialog.ui \
    JoineryTS/internut.ui

DISTFILES += \
    Control Code Assign.md \
    Control Code Assign.txt \
    Control Code Assign.txt \
    Control Code Assign.txt \
    Control_Code_Assign.md \
    README.md \
    algorithms (TODO).md \
    algorithms_(TODO).md \
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

