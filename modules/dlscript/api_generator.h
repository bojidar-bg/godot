#ifndef API_GENERATOR_H
#define API_GENERATOR_H


#if defined(TOOLS_ENABLED) || defined(DEBUG_METHODS_ENABLED)


#include "typedefs.h"
#include "core/ustring.h"


Error generate_c_api(const String &p_path);

#endif

#endif // API_GENERATOR_H
