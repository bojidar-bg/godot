#ifndef API_GENERATOR_H
#define API_GENERATOR_H


#if defined(TOOLS_ENABLED) || defined(DEBUG_METHODS_ENABLED)


#include "typedefs.h"
#include "core/ustring.h"

struct APIGenConfig {
	bool   tools_enabled;
	String save_path;

	bool   generate_json;
	String json_name;
};

Error generate_c_api(const APIGenConfig &p_config);

#endif

#endif // API_GENERATOR_H
