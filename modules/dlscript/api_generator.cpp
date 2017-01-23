#include "api_generator.h"


#include "class_db.h"
#include "os/file_access.h"
#include "path_utils.h"



#define FUNCTION_PREFIX "godot_"

#define DATATYPE_PREFIX "godot_"


// helper stuff


static Error save_file(const String& p_path, const List<String>& p_content) {

	FileAccessRef file = FileAccess::open(p_path, FileAccess::WRITE);

	ERR_FAIL_COND_V(!file, ERR_FILE_CANT_WRITE);

	for (const List<String>::Element *e = p_content.front(); e != NULL; e = e->next()) {
		file->store_string(e->get());
	}

	file->close();

	return OK;
}

// helper stuff end

struct MethodAPI {
	String method_name;
	String return_type;

	List<String> argument_types;
	List<String> argument_names;

	/*
	struct CNames {
		String method_name;
		String return_type;

		List<String> argument_types;
		List<String> argument_names;

		List<String> special_return_arg_names;
		List<String> special_return_arg_types;

	} c;
	*/
};

struct ClassAPI {
	String class_name;

	bool is_singleton;

	List<MethodAPI> methods;

	// constants
};


//

String cpp_to_c_type(const PropertyInfo& p_return)
{
	switch (p_return.type) {
	case Variant::ARRAY:
	case Variant::BASIS:
	case Variant::BOOL:
	case Variant::COLOR:
	case Variant::DICTIONARY:
	case Variant::IMAGE:
	case Variant::INPUT_EVENT:
	case Variant::INT:
	case Variant::NIL:
	case Variant::NODE_PATH:
		return "void *";
	case Variant::OBJECT:
		return DATATYPE_PREFIX + p_return.hint_string.to_lower();
	case Variant::PLANE:
	case Variant::POOL_BYTE_ARRAY:
	case Variant::POOL_COLOR_ARRAY:
	case Variant::POOL_INT_ARRAY:
	case Variant::POOL_REAL_ARRAY:
	case Variant::POOL_STRING_ARRAY:
	case Variant::POOL_VECTOR2_ARRAY:
	case Variant::POOL_VECTOR3_ARRAY:
	case Variant::QUAT:
	case Variant::REAL:
	case Variant::RECT2:
	case Variant::RECT3:
	case Variant::STRING:
	case Variant::TRANSFORM:
	case Variant::TRANSFORM2D:
	case Variant::VECTOR2:
	case Variant::VECTOR3:
	case Variant::_RID:
		return "void *";
	default:
		return "godot_wat????";
	}
}


String cpp_string_to_c_type(const String& p_type) {

	return DATATYPE_PREFIX + p_type.to_lower();
}


//

List<ClassAPI> generate_c_api_classes(const APIGenConfig& p_config) {

	List<ClassAPI> api;

	List<StringName> classes;
	ClassDB::get_class_list(&classes);


	for (List<StringName>::Element *e = classes.front(); e != NULL; e = e->next()) {

		ClassDB::APIType api_type = ClassDB::get_api_type(e->get());
		if (api_type == ClassDB::API_NONE || (!p_config.tools_enabled && api_type == ClassDB::API_EDITOR)) {
			continue;
		}

		StringName class_name = e->get();

		ClassAPI class_api;
		class_api.class_name = class_name;

		List<MethodInfo> methods;
		ClassDB::get_method_list(class_name, &methods, true);


		for (List<MethodInfo>::Element *m = methods.front(); m != NULL; m = m->next()) {
			MethodAPI method_api;
			MethodBind *method_bind = ClassDB::get_method(class_name, m->get().name);

			method_api.method_name = m->get().name;

			if (method_bind && method_bind->get_return_type() != StringName()) {
				method_api.return_type = method_bind->get_return_type();
			} else if (method_api.method_name.find(":") != -1) {
				method_api.return_type = method_api.method_name.get_slice(":", 1);
				method_api.method_name = method_api.method_name.get_slice(":", 0);
			} else if (m->get().return_val.type != Variant::NIL) {
				method_api.return_type = m->get().return_val.hint == PROPERTY_HINT_RESOURCE_TYPE
						? m->get().return_val.hint_string
						: Variant::get_type_name(m->get().return_val.type);
			} else {
				method_api.return_type = "void";
			}


			List<PropertyInfo> arguments = m->get().arguments;
			for (List<PropertyInfo>::Element *a = arguments.front(); a != NULL; a = a->next()) {
				method_api.argument_names.push_back(a->get().name);
				// TODO: argument types
			}

			class_api.methods.push_back(method_api);
		}

		api.push_back(class_api);
	}

	return api;
}


static List<String> generate_c_api_json(const APIGenConfig &p_config, const List<ClassAPI>& p_api) {

	List<String> source;

	for (const List<ClassAPI>::Element *c = p_api.front(); c != NULL; c = c->next()) {
		ClassAPI api = c->get();

		for (const List<MethodAPI>::Element *m = api.methods.front(); m != NULL; m = m->next()) {
			MethodAPI mapi = m->get();
			print_line(mapi.return_type + "\t" + api.class_name + "." + mapi.method_name);
		}
	}

	return source;
}


//



Error generate_c_api(const APIGenConfig &p_config) {

	List<ClassAPI> api = generate_c_api_classes(p_config);

	if (p_config.generate_json)
	{
		List<String> json_source = generate_c_api_json(p_config, api);
		save_file(p_config.save_path + "/" + p_config.json_name, json_source);
	}

	return OK;
}
