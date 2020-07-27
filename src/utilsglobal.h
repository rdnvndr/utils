#include <QtCore/QtGlobal>

#if defined(UTILS_LIBRARY)
#  define UTILSLIB Q_DECL_EXPORT
#else
#  define UTILSLIB Q_DECL_IMPORT
#endif
