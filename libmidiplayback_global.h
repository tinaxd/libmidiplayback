#ifndef LIBMIDIPLAYBACK_GLOBAL_H
#define LIBMIDIPLAYBACK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBMIDIPLAYBACK_LIBRARY)
#  define LIBMIDIPLAYBACK_EXPORT Q_DECL_EXPORT
#else
#  define LIBMIDIPLAYBACK_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBMIDIPLAYBACK_GLOBAL_H
