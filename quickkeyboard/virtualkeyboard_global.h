#ifndef MOCKUPVIRTUALKEYBOARD_GLOBAL_H
#define MOCKUPVIRTUALKEYBOARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MOCKUPVIRTUALKEYBOARD_LIBRARY)
#  define MOCKUPVIRTUALKEYBOARDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MOCKUPVIRTUALKEYBOARDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MOCKUPVIRTUALKEYBOARD_GLOBAL_H
