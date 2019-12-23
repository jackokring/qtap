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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-libkqfn-Desktop_Qt_5_12_6_GCC_64bit-Debug/release/ -llibkqfn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-libkqfn-Desktop_Qt_5_12_6_GCC_64bit-Debug/debug/ -llibkqfn
else:unix: LIBS += -L$$PWD/../build-libkqfn-Desktop_Qt_5_12_6_GCC_64bit-Debug/ -llibkqfn

INCLUDEPATH += $$PWD/../libkqfn
DEPENDPATH += $$PWD/../libkqfn

FORMS += \
    settings.ui \
    statsview.ui

DISTFILES += \
    style.css
