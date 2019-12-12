#ifndef LIBKQFN_GLOBAL_H
#define LIBKQFN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBKQFN_LIBRARY)
#  define LIBKQFN_EXPORT Q_DECL_EXPORT
#else
#  define LIBKQFN_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBKQFN_GLOBAL_H
