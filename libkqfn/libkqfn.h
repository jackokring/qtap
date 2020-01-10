#ifndef LIBKQFN_H
#define LIBKQFN_H

#include <QCoreApplication>
#include <QtWebEngineWidgets/QWebEngineView>
#include "libkqfn_global.h"
#define LIBPTR(X) ((void *)X)
#define QWIDGETPTR(X) ((QWidget *)X)
#define HANDLE(X) ((Libkqfn *)X)
#define TYPE_LIBPTR void *

//===================================================
// MAIN PROCESSING CLASS
//===================================================
extern "C" class LIBKQFN_EXPORT Libkqfn {
public:
    Libkqfn();

    TYPE_LIBPTR getJSHost();
};

//===================================================
// HANDLE DYNAMIC LOADER
//===================================================
extern "C" LIBKQFN_EXPORT Libkqfn *handle();
typedef Libkqfn *(*LibHandle)();

#define QLOAD QLibrary L_libkqfn("liblibkqfn.so");if (!L_libkqfn.load())qDebug() << L_libkqfn.errorString();
#define QRESOLVE(X) ((LibHandle)L_libkqfn.resolve(X))();

#endif // LIBKQFN_H
