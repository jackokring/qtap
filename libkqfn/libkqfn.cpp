#include "libkqfn.h"

Libkqfn::Libkqfn()
{

}

TYPE_LIBPTR Libkqfn::getJSHost() {
    QWebEngineView *view = new QWebEngineView();
    return view;
}
