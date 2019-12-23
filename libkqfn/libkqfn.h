#ifndef LIBKQFN_H
#define LIBKQFN_H

#include <QCoreApplication>
#include <QtWebEngineWidgets/QWebEngineView>
#include "libkqfn_global.h"
#define LIBPTR(X) ((void *)X)
#define QWIDGETPTR(X) ((QWidget *)X)
#define HANDLE(X) ((Libkqfn *)X)
#define TYPE_LIBPTR void *

/* extern "C" */ class LIBKQFN_EXPORT Libkqfn
{
public:
    Libkqfn();

    TYPE_LIBPTR getJSHost();
};

#endif // LIBKQFN_H
